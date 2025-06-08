import random
from mylibs import CoolCipher, VirtualMAC
from mymacs import OMAC, TMAC, HMAC

def flip_random_bit(bytes_: bytes) -> bytes:
    bit_string = ''.join(f'{byte:08b}' for byte in bytes_)
    bit_position = random.randint(0, len(bit_string) - 1)
    flipped_bit = '1' if bit_string[bit_position] == '0' else '0'
    bits = f"{bit_string[:bit_position]}{flipped_bit}{bit_string[bit_position + 1:]}"
    bytes_ = int(bits, 2).to_bytes((len(bits) + 7) // 8, byteorder='big')
    return bytes_

def template_test_mac(any_mac: VirtualMAC, data: bytes = b'some-random-text', corrupted_data: bytes = b'some-rand0m-text'):
    bit_corrupted_data = flip_random_bit(data)
    tag = any_mac.compute_mac(data)
    assert any_mac.verify_mac(data, tag)
    assert not any_mac.verify_mac(corrupted_data, tag)
    assert not any_mac.verify_mac(bit_corrupted_data, tag)

def template_test_2_5B(any_mac: VirtualMAC):
    data = CoolCipher.gen_random_bytes_vector(int(2.5 * any_mac.BLOCK_SIZE))
    corrupted_data = CoolCipher.gen_random_bytes_vector(int(2.5 * any_mac.BLOCK_SIZE))

    while data == corrupted_data:
        corrupted_data = CoolCipher.gen_random_bytes_vector(int(2.5 * any_mac.BLOCK_SIZE))

    template_test_mac(any_mac, data, corrupted_data)

def test_omac():
    omac = OMAC()
    template_test_mac(omac)
    template_test_2_5B(omac)

def test_tmac():
    tmac = TMAC()
    template_test_mac(tmac)
    template_test_2_5B(tmac)

def test_hmac():
    hmac = HMAC()
    template_test_mac(hmac)
    template_test_2_5B(hmac)
