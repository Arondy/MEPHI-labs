from hashlib import sha256
from itertools import product, islice
from math import log2
from time import perf_counter
from multiprocessing import Manager, Pool
from multiprocessing.pool import ThreadPool
from secrets import token_bytes

class PollardAttack:
    MY_HASH_MAX_SIZE_IN_BYTES = 3
    INT_SIZE_IN_BYTES = 4

    def __init__(self, hash_bits: int = 24, processes_num: int = 2, k: int = 3, iv: list = None):
        self._hash_bits_number = hash_bits
        self._processes_num = processes_num
        self.k = k
        self._q = (self._hash_bits_number // 2) - int(log2(processes_num))
        self._initial_values = iv if iv else [self.gen_random_bytes_vector(8) for _ in range(processes_num)]

    @staticmethod
    def bytes_to_bits(from_bytes: bytes) -> str:
        return ''.join(f'{byte:08b}' for byte in from_bytes)

    def my_hash(self, from_bytes: bytes) -> bytes:
        hash_ = sha256(from_bytes).digest()
        reduced_hash = hash_[-self.MY_HASH_MAX_SIZE_IN_BYTES:]
        bits = self.bytes_to_bits(reduced_hash)[-self._hash_bits_number:]
        bytes_ = int(bits, 2).to_bytes((len(bits) + 7) // 8, byteorder='big')
        return bytes_

    def set_hash_bits_number(self, number: int):
        self._hash_bits_number = number
        self._q = int(number / 2 - log2(self._processes_num))

    @staticmethod
    def gen_random_bytes_vector(number: int) -> bytes:
        return token_bytes(number)

    def _get_padded_bytes(self, bytes_) -> bytes:
        bits = self.bytes_to_bits(bytes_)
        bits += '0' * self.k
        bytes2 = int(bits, 2).to_bytes((len(bits) + 7) // 8, byteorder='big')
        return bytes2
        # bytes_int = int.from_bytes(bytes_, 'big')
        # p = bytes_int << self.k
        # p_size = (self._hash_bits_number + self.k + 7) // 8
        # return p.to_bytes(p_size, 'big')

    def _is_dpoint(self, bytes_):
        # bits = self.bytes_to_bits(bytes_)
        # return bits.startswith('0' * self._q)
        bytes_int = int.from_bytes(bytes_, 'big')
        total_bits = len(bytes_) * 8
        shifted = bytes_int >> (total_bits - self._q)
        is_dpoint = (shifted == 0)
        return is_dpoint

    def _find_shared_dpoint(self, y0, shared_dpoints, shared_collision):
        bytes_ = y0
        counter = 0
        shared_collision_len = len(shared_collision)

        while shared_collision_len == len(shared_collision):
            hash_ = self.my_hash(bytes_)
            bytes_ = self._get_padded_bytes(hash_)
            counter += 1

            if not self._is_dpoint(bytes_):
                continue

            same = shared_dpoints.get(bytes_, None)

            if same:
                y01, counter01 = same
                shared_collision.append((y0, y01, counter, counter01))
                return

            shared_dpoints[bytes_] = (y0, counter)

    def _iterate_needed_bytes(self, shared_collision):
        y00, y01, c00, c01 = shared_collision[0]

        if c00 > c01:
            bytes1 = y00
            bytes2 = y01
        else:
            bytes1 = y01
            bytes2 = y00

        for i in range(abs(c00 - c01)):
            hash_ = self.my_hash(bytes1)
            bytes1 = self._get_padded_bytes(hash_)

        return bytes1, bytes2

    def _find_collision(self, shared_collision):
        bytes1, bytes2 = self._iterate_needed_bytes(shared_collision)

        while True:
            hash1 = self.my_hash(bytes1)
            hash2 = self.my_hash(bytes2)

            if hash1 == hash2:
                return bytes1, bytes2

            bytes1 = self._get_padded_bytes(hash1)
            bytes2 = self._get_padded_bytes(hash2)

    def pollard_attack(self, shared_dpoints=None, shared_collision=None):
        if shared_dpoints is None or shared_collision is None:
            manager = Manager()
            shared_dpoints = manager.dict()
            shared_collision = manager.list()

        s = perf_counter()

        with ThreadPool(processes=self._processes_num) as pool:
            processes = [
                pool.apply_async(self._find_shared_dpoint,
                                 args=(y0, shared_dpoints, shared_collision))
                for y0 in self._initial_values
            ]
            results = []

            for process in processes:
                results.append(process.get())

            pool.close()

        collision = self._find_collision(shared_collision)
        e = perf_counter()

        return {
            'collision': collision,
            'time': e - s,
            'memory': (len(shared_dpoints) *
                       (4 + (self._hash_bits_number + self.k) // 8 + self.INT_SIZE_IN_BYTES))
        }

def generate_all_byte_sequences(seq_size: int):
    if seq_size <= 0:
        raise ValueError(f"Неверная длина последовательности: {seq_size}")

    byte_range = range(256)
    all_sequences = product(byte_range, repeat=seq_size)

    return (bytes(sequence) for sequence in all_sequences)

def save_results(filename: str = 'collisions_pollard_3_bytes.txt', target_collisions_num: int = 100):
    collisions = []

    for i in range(target_collisions_num):
        pa = PollardAttack()
        data = pa.pollard_attack()
        collision = data['collision']
        collisions.append(collision)

    with open(filename, 'w') as file:
        for pair in collisions:
            line = ' - '.join([bytes_.hex() for bytes_ in pair])
            file.write(f"{line}\n")

def measure(target_collisions_num: int = 100):
    results = []
    manager = Manager()
    # shared_dpoints = manager.dict()
    shared_dpoints = {}
    shared_collision = manager.list()
    processes_num = 2
    seq = generate_all_byte_sequences(6)
    runs_num = 5

    for bits_number in range(8, 25):
        print(bits_number)
        time = 0
        memory = 0

        # for j in range(runs_num):
        for i in range(target_collisions_num):
            pa = PollardAttack(hash_bits=bits_number, iv=list(islice(seq, processes_num)))
            data = pa.pollard_attack(shared_dpoints, shared_collision)
            shared_dpoints.clear()
            shared_collision[:] = []
            time += data['time']
            memory += data['memory']

        results.append((bits_number, time / runs_num, memory // runs_num))

    print(results)

if __name__ == '__main__':
    measure()
