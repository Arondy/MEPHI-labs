CREATE TABLE clients (
    client_id INTEGER PRIMARY KEY AUTOINCREMENT,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    phone VARCHAR(15),
    email VARCHAR(100),
    registration_date DATE NOT NULL
);

CREATE TABLE trainers (
    trainer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    specialization VARCHAR(100),
    hire_date DATE NOT NULL,
    contact_phone VARCHAR(15)
);

CREATE TABLE memberships (
    membership_id INTEGER PRIMARY KEY AUTOINCREMENT,
    type VARCHAR(50) NOT NULL,
    duration_months INTEGER NOT NULL,
    price DECIMAL(10,2) NOT NULL
);

CREATE TABLE schedule (
    schedule_id INTEGER PRIMARY KEY AUTOINCREMENT,
    trainer_id INTEGER NOT NULL,
    class_name VARCHAR(100) NOT NULL,
    day_of_week VARCHAR(15) NOT NULL,
    start_time TIME NOT NULL,
    end_time TIME NOT NULL,
    FOREIGN KEY (trainer_id) REFERENCES trainers(trainer_id)
        ON UPDATE CASCADE
);

CREATE TABLE visits (
    visit_id INTEGER PRIMARY KEY AUTOINCREMENT,
    client_id INTEGER NOT NULL,
    schedule_id INTEGER NOT NULL,
    visit_date DATE NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(client_id)
        ON UPDATE CASCADE,
    FOREIGN KEY (schedule_id) REFERENCES schedule(schedule_id)
        ON UPDATE CASCADE
);

CREATE TABLE purchases (
    purchase_id INTEGER PRIMARY KEY AUTOINCREMENT,
    client_id INTEGER NOT NULL,
    membership_id INTEGER NOT NULL,
    purchase_date DATE NOT NULL,
    expiration_date DATE NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(client_id)
        ON UPDATE CASCADE,
    FOREIGN KEY (membership_id) REFERENCES memberships(membership_id)
        ON UPDATE CASCADE
);

-- Индексы для оптимизации запросов на будущее
--CREATE INDEX idx_client_name ON clients(last_name, first_name);
--CREATE INDEX idx_trainer_name ON trainers(last_name, first_name);
--CREATE INDEX idx_schedule_day ON schedule(day_of_week, start_time);
--INDEX idx_visit_date ON visits(visit_date);