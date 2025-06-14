import matplotlib.pyplot as plt

birthday = [(8, 0.00064180011395365, 795), (9, 0.0008468999294564128, 1235), (10, 0.001066999975591898, 1942), (11, 0.0013792000245302916, 2784), (12, 0.002092399983666837, 4565), (13, 0.0029920999659225345, 6918), (14, 0.004317900049500167, 10413), (15, 0.005749200005084276, 14593), (16, 0.00854740000795573, 20928), (17, 0.011453600018285215, 30925), (18, 0.01691979996394366, 46212), (19, 0.021863500005565584, 61977), (20, 0.03385000000707805, 97831), (21, 0.047936399932950735, 139303), (22, 0.06595810002181679, 194244), (23, 0.09140699997078627, 277990), (24, 0.11569509992841631, 368445)]
pollard = [(8, 0.30321778035722674, 20932), (9, 0.39961749988142403, 29660), (10, 0.33646857980638745, 32041), (11, 0.37091647973284125, 42382), (12, 0.47145924016367646, 65134), (13, 0.6623682601610199, 124792), (14, 0.8895648000761867, 181432), (15, 0.9335166201926768, 194226), (16, 1.583623700006865, 91510), (17, 2.1434236203087496, 516414), (18, 2.6369915201794356, 633862), (19, 3.779235679726116, 916442), (20, 4.80490319984965, 1188574), (21, 5.535997939831577, 750413), (22, 9.149109039874748, 319101), (23, 11.106746020284481, 194744), (24, 16.771715340111406, 77266)]

def make_graph(data: list, name: str):
    bits = [item[0] for item in data]
    time_data = [item[1] for item in data]
    memory_data = [item[2] // 1024 for item in data]

    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    plt.plot(bits, time_data, marker='o', linestyle='-', color='b')
    plt.title('График времени выполнения')
    plt.xlabel('Кол-во бит')
    plt.ylabel('Время выполнения (сек)')
    plt.grid(True)

    plt.subplot(1, 2, 2)
    plt.plot(bits, memory_data, marker='o', linestyle='-', color='r')
    plt.title('График использования памяти')
    plt.xlabel('Кол-во бит')
    plt.ylabel('Память (КБ)')
    plt.grid(True)

    plt.suptitle(name, fontsize=16)
    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    make_graph(birthday, 'Birthday')
    make_graph(pollard, 'Pollard')
