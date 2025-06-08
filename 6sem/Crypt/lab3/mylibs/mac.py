from abc import ABC, abstractmethod
from dataclasses import dataclass, field
from mylibs.cipher import CoolCipher

@dataclass
class VirtualMAC(ABC):
    KEY_SIZE: int = 16
    BLOCK_SIZE: int = 16
    BITS_IN_BYTES: int = 8

    cipher: CoolCipher = field(default_factory=CoolCipher)
    key: bytes = None
    current_block: bytes = b''
    prev_block_output: bytes = b'\x00' * BLOCK_SIZE

    def set_key(self, key: bytes):
        if len(key) != self.KEY_SIZE:
            raise ValueError(f"Неверная длина ключа: {len(key)} вместо {self.KEY_SIZE}")

        self.key = key
        self.cipher.set_key(key)

    @abstractmethod
    def _add_padding(self, data_block: bytes) -> bytes:
        pass

    @abstractmethod
    def add_block(self, data_block: bytes):
        pass

    @abstractmethod
    def finalize(self) -> bytes:
        pass

    def compute_mac(self, data: bytes) -> bytes:
        data_size = len(data)

        for i in range(0, data_size, self.BLOCK_SIZE):
            block = data[i:i + self.BLOCK_SIZE]
            self.add_block(block)

        return self.finalize()

    def verify_mac(self, data: bytes, tag: bytes) -> bool:
        computed_tag = self.compute_mac(data)
        return computed_tag == tag

    def clear(self):
        self.current_block = b''
        self.prev_block_output = b'\x00' * self.BLOCK_SIZE
