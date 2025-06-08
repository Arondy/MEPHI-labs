import time

import aiohttp
import asyncio
import random

# Конфигурация тестирования
URL = "http://localhost:8080/order"
HEADERS = {"Content-Type": "application/json"}

# Настройки нагрузки
BURST_REQUESTS = 5000  # Количество запросов для всплеска
SUSTAINED_RATE = 135  # Число запросов в секунду при продолжительной нагрузке
SUSTAINED_DURATION = 300  # Продолжительность нагрузки в секундах (5 минут)

# Асинхронная функция для отправки одного запроса
async def send_request(session, order_id):
    data = {"product_id": str(order_id)}
    try:
        async with session.post(URL, headers=HEADERS, json=data) as response:
            response_text = await response.text()
            print(f"[{order_id}] Status: {response.status}, Response: {response_text}")
    except Exception as e:
        print(f"[{order_id}] Error: {e}")

# Единовременный всплеск активности
async def burst_test():
    print("Starting burst test...")
    async with aiohttp.ClientSession() as session:
        tasks = [
            send_request(session, random.randint(1, 4)) for _ in range(BURST_REQUESTS)
        ]
        await asyncio.gather(*tasks)
    print("Burst test completed.")

# Продолжительная нагрузка
async def sustained_test():
    print("Starting sustained test...")
    starting_time = time.time()

    async with aiohttp.ClientSession() as session:

        while time.time() - starting_time < SUSTAINED_DURATION:
            s = time.perf_counter()
            tasks = [
                send_request(session, random.randint(1, 4)) for _ in range(SUSTAINED_RATE)
            ]
            await asyncio.gather(*tasks)
            e = time.perf_counter()
            time.sleep(max(1 - (e - s), 0))

    print("Sustained test completed.")

if __name__ == "__main__":
    # Выполняем тесты
    # asyncio.run(burst_test())
    asyncio.run(sustained_test())
