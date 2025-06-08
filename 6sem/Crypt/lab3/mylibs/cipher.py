import enum
from dataclasses import dataclass, field
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from secrets import token_bytes

@enum.unique
class CipherMode(enum.IntEnum):
    ECB = 0
    CBC = 1
    CFB = 2
    OFB = 3
    CTR = 4

@dataclass
class CoolCipher:
    KEY_LENGTH_BYTES: int = 16
    BLOCK_SIZE_BYTES: int = 16
    BITS_IN_BYTES: int = 8
    NONCE_LENGTH_BYTES: int = 3 * BLOCK_SIZE_BYTES // 4
    COUNTER_LENGTH_BYTES: int = BLOCK_SIZE_BYTES - NONCE_LENGTH_BYTES

    key: bytes = b''
    cipher_mode: CipherMode = CipherMode.ECB
    chained_vector: bytes = b''
    aes: AES = field(init=False)

    def set_key(self, new_key: bytes):
        if not isinstance(new_key, bytes):
            raise ValueError(f"Неверный тип переменной ключа: {type(new_key)} вместо {bytes}")
        if len(new_key) != self.KEY_LENGTH_BYTES:
            raise ValueError(f"Неверная длина ключа: {len(new_key)} вместо {self.KEY_LENGTH_BYTES}")

        self.key = new_key
        self.aes = AES.new(new_key, AES.MODE_ECB)

    def set_mode(self, new_cipher_mode: CipherMode):
        if not isinstance(new_cipher_mode, CipherMode):
            raise ValueError(f"Неверный тип переменной режима шифрования: {type(new_cipher_mode)} вместо {CipherMode}")

        self.cipher_mode = new_cipher_mode

    @staticmethod
    def gen_random_bytes_vector(nbytes: int = BLOCK_SIZE_BYTES) -> bytes:
        return token_bytes(nbytes)

    @staticmethod
    def xor_bytes(bytes1: bytes, bytes2: bytes) -> bytes:
        xored_bytes = bytes(b1 ^ b2 for b1, b2 in zip(bytes1, bytes2))
        return xored_bytes

    def __pkcs7_padding(self, data: bytes, is_final_block: bool) -> bytes:
        if is_final_block:
            data = pad(data, self.BLOCK_SIZE_BYTES, style='pkcs7')
        return data

    def __pkcs7_unpadding(self, data: bytes, is_final_block: bool) -> bytes:
        if is_final_block:
            data = unpad(data, self.BLOCK_SIZE_BYTES, style='pkcs7')
        return data

    def __increase_ctr_counter(self):
        counter = int.from_bytes(self.chained_vector[self.NONCE_LENGTH_BYTES:], 'big')
        counter += 1

        if counter >= 2 ** (self.COUNTER_LENGTH_BYTES * self.BITS_IN_BYTES):
            raise ValueError("Переполнение счетчика!")

        self.chained_vector = (
                self.chained_vector[:self.NONCE_LENGTH_BYTES] +
                counter.to_bytes(self.COUNTER_LENGTH_BYTES, 'big')
        )

    def __check_key_and_data(self, data):
        if not self.key:
            raise ValueError(f"Неверный ключ: {self.key}")
        if not isinstance(data, bytes):
            raise ValueError(f"Неверный тип переменной шифруемых данных: {type(data)} вместо {bytes}")

    def block_cipher_encrypt(self, plain_data_block: bytes) -> bytes:
        cipher = self.aes.encrypt(plain_data_block)
        return cipher

    def __ecb_encrypt(self, plain_data: bytes) -> bytes:
        cipher = self.block_cipher_encrypt(plain_data)
        return cipher

    def __cbc_encrypt(self, plain_data: bytes) -> bytes:
        plain_data = self.xor_bytes(plain_data, self.chained_vector)
        cipher = self.block_cipher_encrypt(plain_data)
        self.chained_vector = cipher
        return cipher

    def __cfb_encrypt(self, plain_data: bytes) -> bytes:
        cipher = self.block_cipher_encrypt(self.chained_vector)
        cipher = self.xor_bytes(cipher, plain_data)
        self.chained_vector = cipher
        return cipher

    def __ofb_encrypt(self, plain_data: bytes) -> bytes:
        cipher = self.block_cipher_encrypt(self.chained_vector)
        self.chained_vector = cipher
        cipher = self.xor_bytes(cipher, plain_data)
        return cipher

    def __ctr_encrypt(self, plain_data: bytes) -> bytes:
        cipher = self.block_cipher_encrypt(self.chained_vector)
        cipher = self.xor_bytes(cipher, plain_data)
        return cipher

    def process_block_encrypt(self, data: bytes, is_final_block: bool) -> bytes:
        self.__check_key_and_data(data)

        if not self.chained_vector:
            self.chained_vector = self.gen_random_bytes_vector()

        match self.cipher_mode:
            case CipherMode.ECB:
                data = self.__pkcs7_padding(data, is_final_block)
                cipher = b''

                for i in range(0, len(data), self.BLOCK_SIZE_BYTES):
                    cipher += self.__ecb_encrypt(data[i:i + self.BLOCK_SIZE_BYTES])
            case CipherMode.CBC:
                data = self.__pkcs7_padding(data, is_final_block)
                cipher = b''

                for i in range(0, len(data), self.BLOCK_SIZE_BYTES):
                    cipher += self.__cbc_encrypt(data[i:i + self.BLOCK_SIZE_BYTES])
            case CipherMode.CFB:
                cipher = self.__cfb_encrypt(data)
            case CipherMode.OFB:
                cipher = self.__ofb_encrypt(data)
            case CipherMode.CTR:
                cipher = self.__ctr_encrypt(data)
                self.__increase_ctr_counter()
            case _:
                raise ValueError(f"Неверный режим шифрования: {self.cipher_mode}")

        return cipher

    def encrypt(self, data: bytes, initialization_vector: bytes = None) -> bytes:
        cipher = b''

        if initialization_vector:
            self.chained_vector = initialization_vector
        elif not self.chained_vector:
            self.chained_vector = self.gen_random_bytes_vector()

        for i in range(0, len(data), self.BLOCK_SIZE_BYTES):
            is_final_block = (i + self.BLOCK_SIZE_BYTES >= len(data))
            cipher += self.process_block_encrypt(data[i:i + self.BLOCK_SIZE_BYTES], is_final_block)

        self.chained_vector = b''
        return cipher

    def __block_cipher_decrypt(self, encrypted_data_block: bytes) -> bytes:
        open_text = self.aes.decrypt(encrypted_data_block)
        return open_text

    def __ecb_decrypt(self, encrypted_data: bytes) -> bytes:
        plain_data = self.__block_cipher_decrypt(encrypted_data)
        return plain_data

    def __cbc_decrypt(self, encrypted_data: bytes) -> bytes:
        plain_data = self.__block_cipher_decrypt(encrypted_data)
        plain_data = self.xor_bytes(plain_data, self.chained_vector)
        self.chained_vector = encrypted_data
        return plain_data

    def __cfb_decrypt(self, encrypted_data: bytes) -> bytes:
        plain_data = self.block_cipher_encrypt(self.chained_vector)
        plain_data = self.xor_bytes(plain_data, encrypted_data)
        self.chained_vector = encrypted_data
        return plain_data

    def __ofb_decrypt(self, encrypted_data: bytes) -> bytes:
        plain_data = self.block_cipher_encrypt(self.chained_vector)
        self.chained_vector = plain_data
        plain_data = self.xor_bytes(plain_data, encrypted_data)
        return plain_data

    def __ctr_decrypt(self, encrypted_data: bytes) -> bytes:
        plain_data = self.block_cipher_encrypt(self.chained_vector)
        plain_data = self.xor_bytes(plain_data, encrypted_data)
        return plain_data

    def process_block_decrypt(self, data: bytes, is_final_block: bool) -> bytes:
        self.__check_key_and_data(data)

        if not self.chained_vector:
            self.chained_vector = data
            return b''

        match self.cipher_mode:
            case CipherMode.ECB:
                plain_data = self.__ecb_decrypt(data)
                plain_data = self.__pkcs7_unpadding(plain_data, is_final_block)
            case CipherMode.CBC:
                plain_data = self.__cbc_decrypt(data)
                plain_data = self.__pkcs7_unpadding(plain_data, is_final_block)
            case CipherMode.CFB:
                plain_data = self.__cfb_decrypt(data)
            case CipherMode.OFB:
                plain_data = self.__ofb_decrypt(data)
            case CipherMode.CTR:
                plain_data = self.__ctr_decrypt(data)
                self.__increase_ctr_counter()
            case _:
                raise ValueError(f"Неверный режим шифрования: {self.cipher_mode}")
        return plain_data

    def decrypt(self, data: bytes, initialization_vector: bytes = None) -> bytes:
        open_data = b''

        if initialization_vector:
            self.chained_vector = initialization_vector

        for i in range(0, len(data), self.BLOCK_SIZE_BYTES):
            is_final_block = (i + self.BLOCK_SIZE_BYTES >= len(data))
            open_data += self.process_block_decrypt(data[i:i + self.BLOCK_SIZE_BYTES], is_final_block)

        self.chained_vector = b''
        return open_data

if __name__ == "__main__":
    pass
