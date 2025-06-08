from dataclasses import dataclass
from Crypto.Util.Padding import pad
from mylibs import CoolCipher, VirtualMAC

@dataclass
class TMAC(VirtualMAC):
    output_tag_size: int = VirtualMAC.BLOCK_SIZE // 2

    # auto for tests
    def __post_init__(self):
        self.set_key(b'sixteen-byte-key')

    def _add_padding(self, data_block: bytes) -> bytes:
        return pad(data_block, self.BLOCK_SIZE, style='pkcs7')

    def add_block(self, data_block: bytes):
        if self.current_block:
            if len(self.current_block) != self.BLOCK_SIZE:
                raise ValueError(f"Неверный размер предыдущего блока: {len(self.current_block)} вместо {self.BLOCK_SIZE};\n"
                                 f"Сейчас можно использовать только функцию finalize()!")

            self.prev_block_output = self.cipher.block_cipher_encrypt(CoolCipher.xor_bytes(self.prev_block_output, self.current_block))

        self.current_block = data_block

    def finalize(self) -> bytes:
        if len(self.current_block) != self.BLOCK_SIZE:
            self.current_block = self._add_padding(self.current_block)

        dummy_data = b'\x00' * self.BLOCK_SIZE
        self.add_block(dummy_data)
        tag = self.prev_block_output[:self.output_tag_size]
        self.clear()

        return tag
