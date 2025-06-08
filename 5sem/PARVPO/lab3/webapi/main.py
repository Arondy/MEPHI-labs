import time

import aio_pika
from fastapi import FastAPI, HTTPException, Request
from fastapi.responses import JSONResponse
from pydantic import BaseModel

# Конфигурация RabbitMQ и FastAPI
RABBITMQ_URL = "amqp://guest:guest@rabbitmq/"
app = FastAPI()
req_max_time = 0

@app.middleware("http")
async def log_request_response_time(request: Request, call_next):
    global req_max_time
    start_time = time.time()
    response = await call_next(request)
    end_time = time.time()
    duration_ms = (end_time - start_time) * 1000
    req_max_time = max(req_max_time, duration_ms)
    return response

@app.get("/log")
async def get_logging_info():
    global req_max_time
    max_time = req_max_time
    print(f"{max_time=:.2f} (ms)", flush=True)
    req_max_time = 0.0
    return JSONResponse(status_code=200, content={"time": max_time})

# Модель данных для запроса
class OrderRequest(BaseModel):
    product_id: str

# Функция для отправки сообщений в RabbitMQ
async def send_to_queue(message: dict):
    connection = await aio_pika.connect_robust(RABBITMQ_URL)

    async with connection:
        channel = await connection.channel()
        queue = await channel.declare_queue("orders")
        await channel.default_exchange.publish(
            aio_pika.Message(body=str(message).encode()),
            routing_key=queue.name,
        )

# Эндпоинт для добавления заказа
@app.post("/order")
async def create_order(order: OrderRequest):
    message = {"product_id": order.product_id, "status": "waiting_for_processing"}

    try:
        await send_to_queue(message)
        print("Отправил сообщение в очередь!")
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Failed to send to queue: {e}")

    return {"product_id": order.product_id, "status": "pending"}

print("WebApi is ready!")
