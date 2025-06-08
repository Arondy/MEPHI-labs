from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
from cipher_lib import CoolCipher, CipherMode

cipher = CoolCipher()

def get_simple_data():
    plain_text = 'some-plain-text!'
    data = plain_text.encode()
    cipher.set_key(cipher.gen_random_bytes_vector(cipher.KEY_LENGTH_BYTES))
    iv = cipher.gen_random_bytes_vector(cipher.KEY_LENGTH_BYTES)
    return plain_text, data, iv

def check_my_cbc_encrypt_with_lib_decrypt():
    plain_text, data, iv = get_simple_data()

    cipher.set_mode(CipherMode.CBC)
    en = cipher.encrypt(data, iv)
    aes = AES.new(cipher.key, AES.MODE_CBC, iv=iv)
    de = aes.decrypt(en)
    de = unpad(de, cipher.BLOCK_SIZE_BYTES)
    assert de.decode() == plain_text

def check_my_cbc_decrypt_with_lib_encrypt():
    plain_text, data, iv = get_simple_data()

    cipher.set_mode(CipherMode.CBC)
    data = pad(data, cipher.BLOCK_SIZE_BYTES)
    aes = AES.new(cipher.key, AES.MODE_CBC, iv=iv)
    en = aes.encrypt(data)
    de = cipher.decrypt(en, iv)
    assert de.decode() == plain_text

def check_2_5_task():
    check_my_cbc_encrypt_with_lib_decrypt()
    check_my_cbc_decrypt_with_lib_encrypt()
    print("Задание 2.5 успешно выполнено")

def check_3_task():
    # CBC
    cipher.set_mode(CipherMode.CBC)
    cipher.set_key(bytes.fromhex("140b41b22a29beb4061bda66b6747e14"))

    data = bytes.fromhex("4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81")
    d1 = cipher.decrypt(data)
    print(f"\nПервый текст:\n{d1.decode()}")

    data = bytes.fromhex("5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253")
    d2 = cipher.decrypt(data)
    print(f"Второй текст:\n{d2.decode()}")

    # CTR
    cipher.set_mode(CipherMode.CTR)
    cipher.set_key(bytes.fromhex("36f18357be4dbd77f050515c73fcf9f2"))

    data = bytes.fromhex("69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329")
    d3 = cipher.decrypt(data)
    print(f"Третий текст:\n{d3.decode()}")

    data = bytes.fromhex("770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451")
    d4 = cipher.decrypt(data)
    print(f"Четвертый текст:\n{d4.decode()}\n")

    print("Задание 3 успешно выполнено")

def check_4_task():
    plain_text, data, iv = get_simple_data()
    data = cipher.gen_random_bytes_vector(int(cipher.BLOCK_SIZE_BYTES * 2.5))

    for mode in CipherMode:
        cipher.set_mode(mode)
        en = cipher.encrypt(data, iv)
        de = cipher.decrypt(en, iv)
        assert de == data

    print("Задание 4 успешно выполнено")

if __name__ == "__main__":
    check_2_5_task()
    check_3_task()
    check_4_task()
