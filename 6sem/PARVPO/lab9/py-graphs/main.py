import matplotlib.pyplot as plt
import matplotlib.cm as cm

# Данные из таблицы
# labels = ['Обычный int', 'const', 'constexpr', 'define', 'iterating', 'const & iterating', 'everything const']
# values = [17.735, 17.8677, 17.9043, 16.8626, 19.0882, 19.0414, 18.1255]
labels = ['-O3', 'volatile sum -O3']
values = [0, 1.40787]

n = len(labels)
colors = [cm.tab10(i) for i in range(n)]

# Создаем график
plt.figure(figsize=(10, 5))
bars = plt.bar(labels, values, color=colors)

# Добавляем значения над столбцами
for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2, yval + 0.005, round(yval, 4), ha='center', va='bottom')

# Подписи и заголовок
plt.title('Время выполнения для бесполезного значения')
plt.ylabel('Время (с)')
plt.ylim(0, max(values) * 1.1)  # Устанавливаем масштаб оси Y для лучшего вида

# Показываем график
plt.tight_layout()
plt.savefig("1.png", dpi=300)
