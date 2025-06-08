from itertools import product
from hashlib import sha256
from time import perf_counter

class BirthdayProblemAttack:
    MY_HASH_MAX_SIZE_IN_BYTES = 3

    def __init__(self, hash_bits_number: int = 24, byte_sequences_size: int = 4):
        self.hash_bits_number = hash_bits_number
        self.byte_sequences_size = byte_sequences_size

    @staticmethod
    def bytes_to_bits(from_bytes: bytes) -> str:
        return ''.join(f'{byte:08b}' for byte in from_bytes)

    @staticmethod
    def generate_all_byte_sequences(seq_size: int):
        if seq_size <= 0:
            raise ValueError(f"Неверная длина последовательности: {seq_size}")

        byte_range = range(256)
        all_sequences = product(byte_range, repeat=seq_size)

        return (bytes(sequence) for sequence in all_sequences)

    def my_hash(self, from_bytes: bytes) -> str:
        hash_ = sha256(from_bytes).digest()
        reduced_hash = hash_[-self.MY_HASH_MAX_SIZE_IN_BYTES:]
        bits = self.bytes_to_bits(reduced_hash)
        return bits[-self.hash_bits_number:]

    def birthday_problem_attack(self, target_collisions_num: int = 1) -> dict:
        sequences = self.generate_all_byte_sequences(self.byte_sequences_size)
        saved_hashes = {}
        collisions_data = []
        counter = 0

        s = perf_counter()

        for bytes_ in sequences:
            hash_ = self.my_hash(bytes_)
            bytes_with_same_hash = saved_hashes.get(hash_, None)

            if bytes_with_same_hash:
                collisions_data.append((bytes_with_same_hash, bytes_))
                counter += 1

                if counter == target_collisions_num:
                    e = perf_counter()
                    return {
                        'collisions': collisions_data,
                        'time': e - s,
                        'memory': len(saved_hashes) * (len(hash_) + len(bytes_) * 8) // 8
                    }

            saved_hashes[hash_] = bytes_

    def save_results(self, filename: str = 'collisions_birthday_3_bytes.txt'):
        data = self.birthday_problem_attack(target_collisions_num=100)
        collisions = data['collisions']

        with open(filename, 'w') as file:
            for pair in collisions:
                line = ' - '.join([bytes_.hex() for bytes_ in pair])
                file.write(f"{line}\n")

    def measure(self):
        results = []

        for bits_number in range(8, 25):
            print(bits_number)
            self.hash_bits_number = bits_number
            data = self.birthday_problem_attack(target_collisions_num=100)
            results.append((bits_number, data['time'], data['memory']))

        print(results)

if __name__ == '__main__':
    bpa = BirthdayProblemAttack()
    # bpa.save_results()
    bpa.measure()
