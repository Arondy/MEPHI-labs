# Используем базовый образ Python
FROM python:3.11-slim

# Устанавливаем рабочую директорию
WORKDIR /app

# Копируем файл зависимостей
COPY requirements.txt /app/

# Устанавливаем зависимости
RUN pip install -r requirements.txt

# Копируем приложение
COPY main.py /app/

# Команда для запуска сервиса
CMD ["python", "main.py"]