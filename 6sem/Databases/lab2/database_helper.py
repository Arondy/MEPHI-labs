import json
import sqlite3
from typing import Any

class DatabaseHelper:
    def __init__(self, db_path: str):
        self.db_path = db_path
        self.conn = sqlite3.connect(self.db_path)
        self.cursor = self.conn.cursor()

    def execute_query(self, query: str, params: tuple = ()) -> list[dict[str, Any]]:
        self.cursor.execute(query, params)
        columns = [column[0] for column in self.cursor.description]
        return [dict(zip(columns, row)) for row in self.cursor.fetchall()]

    def get_all_clients(self) -> list[dict[str, Any]]:
        query = "SELECT * FROM clients;"
        return self.execute_query(query)

    def get_recent_clients(self) -> list[dict[str, Any]]:
        query = "SELECT client_id FROM clients WHERE registration_date >= DATE('now', '-30 days');"
        return self.execute_query(query)

    def get_trainers_by_hire_date(self) -> list[dict[str, Any]]:
        query = "SELECT trainer_id, hire_date FROM trainers ORDER BY hire_date;"
        return self.execute_query(query)

    def get_visit_counts_per_client(self) -> list[dict[str, Any]]:
        query = ("SELECT client_id, COUNT(*) AS visit_count "
                 "FROM visits GROUP BY client_id;")
        return self.execute_query(query)

    def get_unique_purchase_dates(self) -> list[dict[str, Any]]:
        query = "SELECT DISTINCT purchase_date FROM purchases;"
        return self.execute_query(query)

    def get_expired_memberships(self) -> list[dict[str, Any]]:
        query = """
        SELECT c.client_id
        FROM clients c
        WHERE NOT EXISTS (
            SELECT 1
            FROM purchases p
            WHERE p.client_id = c.client_id AND p.expiration_date > DATE('now')
        );
        """
        return self.execute_query(query)

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

    def get_average_membership_price_by_type(self) -> list[dict[str, Any]]:
        query = """
        SELECT m.type, AVG(m.price) AS average_price
        FROM memberships m
        JOIN purchases p ON m.membership_id = p.membership_id
        GROUP BY m.type;
        """
        return self.execute_query(query)

    def get_inactive_clients_last_6_month(self) -> list[dict[str, Any]]:
        query = """
        SELECT c.client_id
        FROM clients c
        WHERE NOT EXISTS (
            SELECT 1
            FROM visits v
            WHERE v.client_id = c.client_id AND v.visit_date >= DATE('now', '-6 months')
        );
        """
        return self.execute_query(query)

    def get_membership_purchase_ranking(self) -> list[dict[str, Any]]:
        query = """
        WITH current_year AS (
            SELECT 
                membership_id,
                COUNT(*) AS current_purchases,
                RANK() OVER (ORDER BY COUNT(*) DESC) AS current_rank
            FROM purchases
            WHERE purchase_date >= DATE('now', '-1 year')
            GROUP BY membership_id
        ),
        previous_year AS (
            SELECT 
                membership_id,
                COUNT(*) AS previous_purchases,
                RANK() OVER (ORDER BY COUNT(*) DESC) AS previous_rank
            FROM purchases
            WHERE purchase_date BETWEEN DATE('now', '-2 years') AND DATE('now', '-1 year')
            GROUP BY membership_id
        )
        SELECT 
            cy.membership_id,
            cy.current_rank,
            (py.previous_rank - cy.current_rank) AS rank_change
        FROM current_year cy
        LEFT JOIN previous_year py ON cy.membership_id = py.membership_id
        ORDER BY cy.current_rank;
        """
        return self.execute_query(query)

    def vacuum(self):
        self.conn.execute("VACUUM")
        self.conn.commit()

    def close(self):
        self.conn.close()

def pretty_json(data: dict | list) -> str:
    return "[" + ",\n".join(str(item) for item in data) + "]"

if __name__ == "__main__":
    database_helper = DatabaseHelper("sport_club.db")

    clients = database_helper.get_all_clients()
    print("1. Все клиенты:", json.dumps(clients, ensure_ascii=False, indent=4))

    recent_clients = database_helper.get_recent_clients()
    print("2. Недавно зарегистрированные клиенты:", pretty_json(recent_clients), sep='\n')

    trainers = database_helper.get_trainers_by_hire_date()
    print("3. Тренеры по дате найма:", pretty_json(trainers), sep='\n')

    visit_counts = database_helper.get_visit_counts_per_client()
    print("4. Количество посещений для каждого клиента:", pretty_json(visit_counts), sep='\n')

    unique_purchase_dates = database_helper.get_unique_purchase_dates()
    print("5. Уникальные даты покупок:", pretty_json(unique_purchase_dates), sep='\n')

    expired_clients = database_helper.get_expired_memberships()
    print("6. Клиенты с истекшими абонементами:", pretty_json(expired_clients), sep='\n')

    top_trainers = database_helper.get_top_trainers_last_month()
    print("7. Тренеры с наибольшим количеством проведенных занятий за месяц:", pretty_json(top_trainers), sep='\n')

    avg_prices = database_helper.get_average_membership_price_by_type()
    print("8. Средняя стоимость абонемента по каждому типу:", pretty_json(avg_prices), sep='\n')

    inactive_clients = database_helper.get_inactive_clients_last_6_month()
    print("9. Клиенты, не посещавшие занятия за последний месяц:", pretty_json(inactive_clients), sep='\n')

    database_helper.vacuum()
    print("10. База данных оптимизирована.")

    membership_ranking = database_helper.get_membership_purchase_ranking()
    print("11. Ранжирование покупок абонементов:", pretty_json(membership_ranking), sep='\n')

    database_helper.close()
