from datetime import timedelta, date, time, datetime
from random import choice, randint
from faker import Faker
from dataclasses import asdict
from psycopg2.extras import execute_batch
from database_helper import DatabaseHelper
from fitness_center import Client, Trainer, Membership, Schedule, Visit, Purchase

class DatabaseFiller:
    def __init__(self, db_helper: DatabaseHelper = DatabaseHelper()):
        self.db_helper = db_helper
        self.fake = Faker('ru_RU')
        self.specializations = ['Фитнес', 'Бассейн', 'Единоборства', 'Йога', 'Пилатес']
        self.membership_types = ['Базовый', 'Стандарт', 'Премиум', 'С бассейном', 'С тренером']
        self.days_of_week = ['Понедельник', 'Вторник', 'Среда', 'Четверг', 'Пятница', 'Суббота', 'Воскресенье']

    def fill_clients(self, num_clients: int):
        clients = [
            Client(
                first_name=self.fake.first_name(),
                last_name=self.fake.last_name(),
                phone=self.fake.unique.phone_number(),
                email=self.fake.unique.email(),
                registration_date=self.fake.date_between(start_date='-2y', end_date='today')
            )
            for _ in range(num_clients)
        ]
        self._insert_data("clients", clients)

    def fill_trainers(self, num_trainers: int):
        trainers = [
            Trainer(
                first_name=self.fake.unique.first_name(),
                last_name=self.fake.unique.last_name(),
                specialization=choice(self.specializations),
                hire_date=self.fake.date_between(start_date='-5y', end_date='today'),
                contact_phone=self.fake.unique.phone_number()
            )
            for _ in range(num_trainers)
        ]
        self._insert_data("trainers", trainers)

    def fill_memberships(self, num_memberships: int):
        memberships = [
            Membership(
                type=choice(self.membership_types),
                duration_months=randint(1, 12),
                price=randint(10000, 60000)
            )
            for _ in range(num_memberships)
        ]
        self._insert_data("memberships", memberships)

    def fill_schedule(self, trainer_ids: list[int], num_schedule: int):
        schedule = []
        for i in range(num_schedule):
            start = self.fake.time_object()
            end = (datetime.combine(date.today(), start) + timedelta(hours=1)).time()
            schedule.append(
                Schedule(
                    trainer_id=choice(trainer_ids),
                    class_name=choice(self.specializations),
                    day_of_week=choice(self.days_of_week),
                    start_time=start,
                    end_time=end
                )
            )
        self._insert_data("schedule", schedule)

    def fill_purchases(self, client_ids: list[int], membership_ids: list[int], num_purchases: int):
        purchases = [
            Purchase(
                client_id=choice(client_ids),
                membership_id=choice(membership_ids),
                purchase_date=self.fake.date_between(start_date='-2y', end_date='today'),
                expiration_date=self.fake.date_between(start_date='-1y', end_date='+1y')
            )
            for _ in range(num_purchases)
        ]
        self._insert_data("purchases", purchases)

    def fill_visits(self, client_ids: list[int], schedule_ids: list[int], num_visits: int):
        visits = [
            Visit(
                client_id=choice(client_ids),
                schedule_id=choice(schedule_ids),
                visit_date=self.fake.date_between(start_date='-2y', end_date='today')
            )
            for _ in range(num_visits)
        ]
        self._insert_data("visits", visits)

    @staticmethod
    def _convert_value(value):
        if isinstance(value, (datetime, date)):
            return value.strftime('%Y-%m-%d')
        elif isinstance(value, time):
            return value.strftime('%H:%M:%S')
        else:
            return value

    def _insert_data(self, table_name: str, data_list: list):
        # Определяем имя первичного ключа — либо "<table>_id", либо "<table[:-1]>_id" (для множественного числа)
        id_field = f"{table_name[:-1]}_id" if table_name.endswith("s") else f"{table_name}_id"

        # Исключаем только первичный ключ, оставляем все внешние
        fields = [
            field.name for field in data_list[0].__dataclass_fields__.values()
            if field.name != id_field
        ]

        placeholders = ", ".join(["%s"] * len(fields))
        columns = ", ".join(fields)
        query = f"INSERT INTO {table_name} ({columns}) VALUES ({placeholders})"

        data_to_insert = [
            tuple(self._convert_value(asdict(item)[field]) for field in fields)
            for item in data_list
        ]

        execute_batch(self.db_helper.cursor, query, data_to_insert, page_size=100)
        self.db_helper.conn.commit()

    def fill_everything(self, num_clients: int, num_trainers: int, num_memberships: int, num_schedule: int,
                        num_purchases: int, num_visits: int):
        # Заполнение таблиц
        self.fill_clients(num_clients)
        self.fill_trainers(num_trainers)
        self.fill_memberships(num_memberships)

        # Получение ID тренеров для расписания
        trainer_ids = [row['trainer_id'] for row in self.db_helper.execute_query("SELECT trainer_id FROM trainers")]

        self.fill_schedule(trainer_ids, num_schedule)

        # Получение ID клиентов и абонементов для покупок
        client_ids = [row['client_id'] for row in self.db_helper.execute_query("SELECT client_id FROM clients")]
        membership_ids = [row['membership_id'] for row in self.db_helper.execute_query("SELECT membership_id FROM memberships")]

        self.fill_purchases(client_ids, membership_ids, num_purchases)

        # Получение ID расписания для посещений
        schedule_ids = [row['schedule_id'] for row in self.db_helper.execute_query("SELECT schedule_id FROM schedule")]

        self.fill_visits(client_ids, schedule_ids, num_visits)

    def close(self):
        self.db_helper.close()

if __name__ == "__main__":
    NUM_CLIENTS = 100
    NUM_TRAINERS = 15
    NUM_MEMBERSHIPS = 8
    NUM_SCHEDULE = 20
    NUM_PURCHASES = 150
    NUM_VISITS = 200

    filler = DatabaseFiller()
    filler.fill_everything(NUM_CLIENTS, NUM_TRAINERS, NUM_MEMBERSHIPS, NUM_SCHEDULE, NUM_PURCHASES, NUM_VISITS)
    filler.close()

    print("База данных успешно заполнена!")
