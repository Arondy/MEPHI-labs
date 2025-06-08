import matplotlib.pyplot as plt

# Данные
levels = ['O0', 'O1', 'O2', 'O3', 'Ofast', 'Os', 'Oz']
build_time = [1.12, 1.27, 1.24, 1.24, 1.23, 1.22, 1.20]
exec_time = [42.28, 3.09, 3.12, 2.71, 2.83, 10.57, 10.34]
exec_time_no_gen = [16.3, 0.9, 0, 0, 0, 6.74, 6.78]
size_kb = [93.78, 50.89, 51.24, 51.42, 56.78, 52.13, 52.13]

# Функция для сохранения графика
def save_bar_chart(x, y, title, ylabel, filename, color):
    plt.figure(figsize=(8, 5))
    plt.bar(x, y, color=color)
    plt.title(title)
    plt.xlabel('Уровень оптимизации')
    plt.ylabel(ylabel)
    plt.ylim(bottom=0)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.savefig(filename, dpi=300)
    plt.close()

if __name__ == '__main__':
    # Сохранение каждого графика отдельно
    save_bar_chart(levels, build_time, 'Время сборки', 'Секунды', 'build_time.png', 'steelblue')
    save_bar_chart(levels, exec_time, 'Время выполнения', 'Секунды', 'execution_time.png', 'salmon')
    save_bar_chart(levels, exec_time_no_gen, 'Время выполнения (без генерации)', 'Секунды', 'execution_time_no_gen.png', 'mediumseagreen')
    save_bar_chart(levels, size_kb, 'Занимаемое место', 'Килобайты', 'binary_size.png', 'orange')
