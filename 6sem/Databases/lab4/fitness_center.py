from dataclasses import dataclass
from datetime import date, time

@dataclass
class Client:
    first_name: str
    last_name: str
    phone: str
    email: str
    registration_date: date
    client_id: int = None  # AUTOINCREMENT, поэтому может быть None при создании

@dataclass
class Trainer:
    first_name: str
    last_name: str
    specialization: str
    hire_date: date
    contact_phone: str
    trainer_id: int = None  # AUTOINCREMENT, поэтому может быть None при создании

@dataclass
class Membership:
    type: str
    duration_months: int
    price: float
    membership_id: int = None  # AUTOINCREMENT, поэтому может быть None при создании

@dataclass
class Schedule:
    trainer_id: int
    class_name: str
    day_of_week: str
    start_time: time
    end_time: time
    schedule_id: int = None  # AUTOINCREMENT, поэтому может быть None при создании

@dataclass
class Visit:
    client_id: int
    schedule_id: int
    visit_date: date
    visit_id: int = None  # AUTOINCREMENT, поэтому может быть None при создании

@dataclass
class Purchase:
    client_id: int
    membership_id: int
    purchase_date: date
    expiration_date: date
    purchase_id: int = None  # AUTOINCREMENT, поэтому может быть None при создании
