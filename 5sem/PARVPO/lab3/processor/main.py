import asyncio
import aio_pika
import sqlite3
from datetime import datetime
from aio_pika.abc import AbstractRobustConnection

# Конфигурация RabbitMQ и базы данных
RABBITMQ_URL = "amqp://guest:guest@rabbitmq/"
DATABASE_PATH = "./orders.db"

# Создание таблицы для хранения заказов
def initialize_database():
    conn = sqlite3.connect(DATABASE_PATH)
    cursor = conn.cursor()
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS orders (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            product_id TEXT NOT NULL,
            status TEXT DEFAULT 'pending',
            created_at TEXT
        )
    """)
    conn.commit()
    conn.close()

# Инициализация базы данных
initialize_database()

# Добавление заказа в базу данных
def add_order_to_database(product_id: str, status: str) -> int:
    conn = sqlite3.connect(DATABASE_PATH)
    cursor = conn.cursor()
    cursor.execute(
        "INSERT INTO orders (product_id, status, created_at) VALUES (?, ?, ?)",
        (product_id, status, datetime.now().isoformat()),
    )
    conn.commit()
    order_id = cursor.lastrowid  # Получение ID только что добавленной записи
    conn.close()
    return order_id

# Обновление статуса заказа в базе данных
def update_order_status(order_id: int, status: str):
    conn = sqlite3.connect(DATABASE_PATH)
    cursor = conn.cursor()
    cursor.execute(
        "UPDATE orders SET status = ? WHERE id = ?",
        (status, order_id),
    )
    conn.commit()
    conn.close()

# Эмуляция обработки и обновление статуса заказа
async def imitate_order_work(order_id: int, status: str, seconds: int = 10):
    await asyncio.sleep(seconds)
    update_order_status(order_id=order_id, status=status)
    print(f"Статус заказа №{order_id} обновлён на {status}.", flush=True)

# Обработка сообщения из RabbitMQ
async def process_message(message: aio_pika.IncomingMessage):
    async with message.process():
        print(f"Получено сообщение: {message.body.decode()}", flush=True)
        order = eval(message.body.decode())  # Преобразуем строку в словарь
        product_id = order.get("product_id")

        if not product_id:
            print("Ошибка: product_id отсутствует в сообщении.", flush=True)
            return

        # Добавляем заказ в базу данных
        order_id = add_order_to_database(product_id, "pending")
        print(f"Заказ добавлен в базу данных: {order_id}", flush=True)
        asyncio.create_task(imitate_order_work(order_id, "transferring_to_delivery"))

async def get_rabbitmq_connection(delay: int = 2) -> AbstractRobustConnection:
    print("Подключение к RabbitMQ...", flush=True)

    while True:
        try:
            connection = await aio_pika.connect_robust(RABBITMQ_URL)
            print("RabbitMQ успешно подключен!", flush=True)
            return connection
        except aio_pika.exceptions.AMQPConnectionError:
            # print(f"RabbitMQ недоступен. Повторная попытка через {delay} с...", flush=True)
            await asyncio.sleep(delay)

# Основная функция для запуска обработки очереди RabbitMQ
async def main():
    connection = await get_rabbitmq_connection()

    async with connection:
        channel = await connection.channel()
        queue = await channel.declare_queue("orders")
        print("Ожидание сообщений из очереди RabbitMQ...", flush=True)
        await queue.consume(process_message)

        event = asyncio.Event()
        await event.wait()

if __name__ == "__main__":
    asyncio.run(main())
    # loop = asyncio.get_event_loop()
    # loop.run_until_complete(main())
