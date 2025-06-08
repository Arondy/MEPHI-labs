DROP TABLE IF EXISTS visits CASCADE;
DROP TABLE IF EXISTS purchases CASCADE;
DROP TABLE IF EXISTS schedule CASCADE;
DROP TABLE IF EXISTS trainers CASCADE;
DROP TABLE IF EXISTS memberships CASCADE;
DROP TABLE IF EXISTS clients CASCADE;

CREATE TABLE clients (
    client_id SERIAL PRIMARY KEY,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    phone VARCHAR(20),
    email VARCHAR(100),
    registration_date DATE NOT NULL
);

CREATE TABLE trainers (
    trainer_id SERIAL PRIMARY KEY,
    first_name VARCHAR(50) NOT NULL,
    last_name VARCHAR(50) NOT NULL,
    specialization VARCHAR(100),
    hire_date DATE NOT NULL,
    contact_phone VARCHAR(20)
);

CREATE TABLE memberships (
    membership_id SERIAL PRIMARY KEY,
    type VARCHAR(50) NOT NULL,
    duration_months INTEGER NOT NULL,
    price NUMERIC(10,2) NOT NULL
);

CREATE TABLE schedule (
    schedule_id SERIAL PRIMARY KEY,
    trainer_id INTEGER NOT NULL,
    class_name VARCHAR(100) NOT NULL,
    day_of_week VARCHAR(20) NOT NULL,
    start_time TIME NOT NULL,
    end_time TIME NOT NULL,
    FOREIGN KEY (trainer_id) REFERENCES trainers(trainer_id)
        ON UPDATE CASCADE
);

CREATE TABLE visits (
    visit_id SERIAL PRIMARY KEY,
    client_id INTEGER NOT NULL,
    schedule_id INTEGER NOT NULL,
    visit_date DATE NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(client_id)
        ON UPDATE CASCADE,
    FOREIGN KEY (schedule_id) REFERENCES schedule(schedule_id)
        ON UPDATE CASCADE
);

CREATE TABLE purchases (
    purchase_id SERIAL PRIMARY KEY,
    client_id INTEGER NOT NULL,
    membership_id INTEGER NOT NULL,
    purchase_date DATE NOT NULL,
    expiration_date DATE NOT NULL,
    FOREIGN KEY (client_id) REFERENCES clients(client_id)
        ON UPDATE CASCADE,
    FOREIGN KEY (membership_id) REFERENCES memberships(membership_id)
        ON UPDATE CASCADE
);