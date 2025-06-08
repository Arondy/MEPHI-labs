from dataclasses import dataclass
from Crypto.Util.Padding import pad
from mylibs import CoolCipher, VirtualMAC

@dataclass
class OMAC(VirtualMAC):
    key1: bytes = None
    key2: bytes = None

    # auto for tests
    def __post_init__(self):
        self.set_key(b'sixteen-byte-key')
        self.derive_subkeys()

    def _get_subkey(self, bytes_: bytes, Rn: bytes):
        int_bytes = int.from_bytes(bytes_)

        key = (int_bytes << 1).to_bytes(self.BLOCK_SIZE)

        if int_bytes >> self.BLOCK_SIZE * self.BITS_IN_BYTES != 0:
            key = CoolCipher.xor_bytes(key, Rn * self.KEY_SIZE)

        return key

    def derive_subkeys(self, some_const_Rn: bytes = b'\x87'):
        L = self.cipher.block_cipher_encrypt(b'\x00' * self.BLOCK_SIZE)

        self.key1 = self._get_subkey(L, some_const_Rn)
        self.key2 = self._get_subkey(self.key1, some_const_Rn)

    def _add_padding(self, data_block: bytes) -> bytes:
        return pad(data_block, self.BLOCK_SIZE, style='iso7816')

    def add_block(self, data_block: bytes):
        if self.current_block:
            if len(self.current_block) != self.BLOCK_SIZE:
                raise ValueError(f"Неверный размер предыдущего блока: {len(self.current_block)} вместо {self.BLOCK_SIZE};\n"
                                 f"Сейчас можно использовать только функцию finalize()!")

            self.prev_block_output = self.cipher.block_cipher_encrypt(CoolCipher.xor_bytes(self.prev_block_output, self.current_block))

        self.current_block = data_block


    def finalize(self) -> bytes:
        if len(self.current_block) == self.BLOCK_SIZE:
            first_xor = CoolCipher.xor_bytes(self.prev_block_output, self.current_block)
            second_xor = CoolCipher.xor_bytes(first_xor, self.key1)
        else:
            padded_block = self._add_padding(self.current_block)
            first_xor = CoolCipher.xor_bytes(self.prev_block_output, padded_block)
            second_xor = CoolCipher.xor_bytes(first_xor, self.key2)

        tag = self.cipher.block_cipher_encrypt(second_xor)
        self.clear()

        return tag
