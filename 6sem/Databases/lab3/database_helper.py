import sqlite3
from typing import Any

class DatabaseHelper:
    def __init__(self, db_path: str = "sport_club.db"):
        self.db_path = db_path
        self.conn = sqlite3.connect(self.db_path)
        self.cursor = self.conn.cursor()

    def execute_query(self, query: str, params: tuple = ()) -> list[dict[str, Any]]:
        """
        Выполняет SQL-запрос и возвращает результат в виде списка словарей.
        """
        self.cursor.execute(query, params)
        columns = [column[0] for column in self.cursor.description]
        return [dict(zip(columns, row)) for row in self.cursor.fetchall()]

    def get_top_trainers_last_month(self) -> list[dict[str, Any]]:
        query = """
        SELECT t.trainer_id, COUNT(v.visit_id) AS total_visits 
        FROM trainers t 
        JOIN schedule s ON t.trainer_id = s.trainer_id 
        JOIN visits v ON s.schedule_id = v.schedule_id 
        WHERE v.visit_date >= DATE('now', '-30 days') 
        GROUP BY t.trainer_id 
        ORDER BY total_visits DESC;
        """
        return self.execute_query(query)

    def get_top_popular_days(self) -> list[dict[str, Any]]:
        query = """
        SELECT s.day_of_week, COUNT(v.visit_id) AS visits_count 
        FROM schedule s 
        JOIN visits v ON s.schedule_id = v.schedule_id 
        GROUP BY s.day_of_week 
        ORDER BY visits_count DESC;
        """
        return self.execute_query(query)

    def get_clients_with_no_visits(self) -> list[dict[str, Any]]:
        query = """
        SELECT client_id 
        FROM purchases 
        WHERE client_id NOT IN (
            SELECT DISTINCT client_id 
            FROM visits
        );
        """
        return self.execute_query(query)

    def get_monthly_visit_trends(self) -> list[dict[str, Any]]:
        query = """
        WITH RECURSIVE months AS (
            SELECT DATE('now', '-12 months') AS month 
            UNION ALL 
            SELECT DATE(month, '+1 month') 
            FROM months 
            WHERE month < DATE('now')
        )
        SELECT 
            strftime('%Y-%m', month) AS month, 
            COUNT(v.visit_id) AS visits 
        FROM months 
        LEFT JOIN visits v ON strftime('%Y-%m', v.visit_date) = strftime('%Y-%m', month) 
        GROUP BY month 
        ORDER BY month;
        """
        return self.execute_query(query)

    def get_clients_above_average_visits(self) -> list[dict[str, Any]]:
        query = """
        SELECT client_id, visit_count 
        FROM (
            SELECT 
                client_id, 
                COUNT(*) AS visit_count, 
                AVG(COUNT(*)) OVER() AS avg_visits 
            FROM visits 
            GROUP BY client_id
        ) 
        WHERE visit_count > avg_visits;
        """
        return self.execute_query(query)

    def get_trainers_during_peak_hours(self) -> list[dict[str, Any]]:
        query = """
        SELECT DISTINCT t.trainer_id, t.first_name, t.last_name 
        FROM trainers t 
        JOIN schedule s ON t.trainer_id = s.trainer_id 
        WHERE s.start_time BETWEEN '17:00' AND '20:00';
        """
        return self.execute_query(query)

    def get_client_statistics(self) -> list[dict[str, Any]]:
        query = """
        SELECT 
            c.client_id,
            c.first_name || ' ' || c.last_name AS full_name,
            COUNT(v.visit_id) AS total_visits,
            MAX(v.visit_date) AS last_visit,
            CASE 
                WHEN EXISTS (
                    SELECT 1 
                    FROM purchases p 
                    WHERE p.client_id = c.client_id 
                      AND p.expiration_date > DATE('now')
                ) THEN 'Активен'
                ELSE 'Неактивен'
            END AS membership_status
        FROM clients c
        LEFT JOIN visits v ON c.client_id = v.client_id
        GROUP BY c.client_id
        ORDER BY total_visits DESC NULLS LAST;
        """
        return self.execute_query(query)

    def get_new_client_activity(self) -> list[dict[str, Any]]:
        query = """
        SELECT 
            c.client_id,
            c.registration_date,
            MIN(v.visit_date) AS first_visit_date,
            JULIANDAY(MIN(v.visit_date)) - JULIANDAY(c.registration_date) AS days_to_first_visit,
            COUNT(v.visit_id) AS visit_count
        FROM clients c
        JOIN visits v ON c.client_id = v.client_id
        WHERE c.registration_date >= DATE('now', '-30 days')
        GROUP BY c.client_id
        HAVING days_to_first_visit <= 30
        ORDER BY days_to_first_visit;
        """
        return self.execute_query(query)

    def get_clients_with_expired_memberships(self) -> list[dict[str, Any]]:
        query = """
        SELECT 
            c.client_id,
            c.first_name || ' ' || c.last_name AS full_name,
            p.expiration_date,
            COUNT(CASE WHEN v.visit_date <= p.expiration_date THEN 1 END) AS visits_before,
            COUNT(CASE WHEN v.visit_date > p.expiration_date THEN 1 END) AS visits_after,
            JULIANDAY(MAX(v.visit_date)) - JULIANDAY(p.expiration_date) AS days_after_expiration
        FROM clients c
        JOIN purchases p ON c.client_id = p.client_id
        LEFT JOIN visits v ON c.client_id = v.client_id
        WHERE p.expiration_date < DATE('now')
        GROUP BY c.client_id
        HAVING visits_after > 0
        ORDER BY days_after_expiration DESC;
        """
        return self.execute_query(query)

    def close(self):
        self.conn.close()

if __name__ == "__main__":
    db = DatabaseHelper()

    print("1. Тренеры с наибольшим количеством посещений за последний месяц:")
    print(db.get_top_trainers_last_month())

    print("\n2. Топ дней недели по посещениям:")
    print(db.get_top_popular_days())

    print("\n3. Клиенты, купившие абонементы, но ни разу не посещавшие занятия:")
    print(db.get_clients_with_no_visits())

    print("\n4. Динамика посещений по месяцам за последний год:")
    print(db.get_monthly_visit_trends())

    print("\n5. Клиенты, посещающие занятия чаще, чем в среднем по клубу:")
    print(db.get_clients_above_average_visits())

    print("\n6. Тренеры, работающие в пиковое время (с 17:00 до 20:00):")
    print(db.get_trainers_during_peak_hours())

    print("\n7. Детальная статистика клиентов:")
    print(db.get_client_statistics())

    print("\n8. Активность новых клиентов:")
    print(db.get_new_client_activity())

    print("\n9. Клиенты с просроченными абонементами и их история посещений:")
    print(db.get_clients_with_expired_memberships())

    db.close()
