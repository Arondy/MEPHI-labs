from dataclasses import dataclass
from hashlib import sha256
from mylibs import CoolCipher, VirtualMAC

@dataclass
class HMAC(VirtualMAC):
    BLOCK_SIZE: int = 64
    IPAD: bytes = b'\x36' * BLOCK_SIZE
    OPAD: bytes = b'\x5c' * BLOCK_SIZE

    key1: bytes = None
    key2: bytes = None
    ihash = sha256()
    ohash = sha256()

    # auto for tests
    def __post_init__(self):
        self.set_key(b'sixteen-byte-key')
        self.derive_subkeys()

    def set_key(self, key: bytes):
        if len(key) > self.BLOCK_SIZE:
            key = sha256(key).digest()

        self.key = key.ljust(self.BLOCK_SIZE, b'\x00')

    def derive_subkeys(self):
        self.key1 = CoolCipher.xor_bytes(self.key, self.IPAD)
        self.key2 = CoolCipher.xor_bytes(self.key, self.OPAD)
        self.ihash.update(self.key1)
        self.ohash.update(self.key2)

    def _add_padding(self, data_block: bytes) -> bytes:
        pass

    def add_block(self, data_block: bytes):
        self.ihash.update(data_block)

    def finalize(self) -> bytes:
        inner = self.ihash.digest()
        self.ohash.update(inner)
        tag = self.ohash.digest()
        self.clear()

        return tag

    def clear(self):
        super().clear()
        self.ihash = sha256()
        self.ohash = sha256()
        self.derive_subkeys()
