import matplotlib.pyplot as plt
from time import perf_counter
from mylibs import CoolCipher, VirtualMAC
from mymacs import OMAC, HMAC

BYTES_IN_KB = 1024
SIZES_IN_KB = [0.1, 1, 10, 1024]

def time_mac(any_mac: VirtualMAC, runs_num: int = 1000):
    result = []

    for size in SIZES_IN_KB:
        time = 0

        for i in range(runs_num):
            data = CoolCipher.gen_random_bytes_vector(int(size * BYTES_IN_KB))

            s = perf_counter()
            any_mac.compute_mac(data)
            e = perf_counter()

            time += e - s

        result.append((size, time))

    return result

def make_graph(result: list[tuple[float, float]], name: str, directory: str = 'needed/'):
    sizes_kb = [size for size, _ in result]
    times = [time for _, time in result]

    plt.figure(figsize=(10, 6))
    plt.plot(sizes_kb, times, marker='o', linestyle='-', color='b')
    plt.xlabel("Размер сообщения (в КБ)")
    plt.ylabel("Время выполнения (в секундах)")
    plt.title(name)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.xscale('log')
    # plt.show()
    plt.savefig(f"{directory}{name.lower()}.png")
    
def save_plots():
    omac = OMAC()
    omac_results = time_mac(omac)
    make_graph(omac_results, 'OMAC')

    hmac = HMAC()
    hmac_results = time_mac(hmac)
    make_graph(hmac_results, 'HMAC')

if __name__ == "__main__":
    save_plots()
