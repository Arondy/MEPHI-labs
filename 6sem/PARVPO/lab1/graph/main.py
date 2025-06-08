import matplotlib.pyplot as plt

# Данные
x = [1, 2, 3, 4]  # Метки для столбцов
y = [16.35, 16.62, 16.64, 17.12]  # Высота столбцов

# Цвета для каждого столбца
colors = ['red', 'green', 'blue', 'orange']

# Создание гистограммы
plt.figure(figsize=(8, 5))  # Размер графика
bars = plt.bar(x, y, color=colors, edgecolor='black')  # Столбцы с разными цветами и границами

# Настройка осей и заголовков
plt.title("Графики времени выполнения программы при разных вариантах", fontsize=14)  # Заголовок графика
plt.xlabel("Вариант", fontsize=12)  # Подпись оси X
plt.ylabel("Время выполнения (с)", fontsize=12)  # Подпись оси Y

# Установка целочисленных меток по оси X
plt.xticks(x)  # Только целые числа (1, 2, 3, 4)

# Включение сетки только по оси Y
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Добавление аннотаций для столбцов
offset = 0.2  # Смещение чисел вверх по оси Y
for i, value in enumerate(y):
    plt.text(x[i], value + offset, f"{value}", ha='center', fontsize=10)

# Показать график
plt.show()
