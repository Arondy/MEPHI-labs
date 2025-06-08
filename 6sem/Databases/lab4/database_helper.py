import psycopg2
from typing import Any, Dict, List

class DatabaseHelper:
    DB_CONFIG = {
        "dbname": "fitness_center",
        "user": "postgres",
        "password": "1",
        "host": "localhost",
        "port": 5432
    }

    def __init__(self, db_config: dict = None):
        """:param db_config: Словарь с параметрами подключения к БД"""
        if db_config is None:
            db_config = self.DB_CONFIG

        self.conn = psycopg2.connect(**db_config)
        self.cursor = self.conn.cursor()

    def execute_query(self, query: str, params: tuple = ()) -> List[Dict[str, Any]]:
        self.cursor.execute(query, params)
        columns = [desc[0] for desc in self.cursor.description]
        return [dict(zip(columns, row)) for row in self.cursor.fetchall()]

    def execute_non_query(self, query: str, params: tuple = ()):
        self.cursor.execute(query, params)
        self.conn.commit()

    def get_all_clients(self) -> list[dict[str, Any]]:
        query = "SELECT * FROM clients;"
        return self.execute_query(query)

    def get_recent_clients(self) -> list[dict[str, Any]]:
        query = "SELECT client_id FROM clients WHERE registration_date >= CURRENT_DATE - INTERVAL '30 days';"
        return self.execute_query(query)

    def get_trainers_by_hire_date(self) -> list[dict[str, Any]]:
        query = "SELECT trainer_id, hire_date FROM trainers ORDER BY hire_date;"
        return self.execute_query(query)

    def get_visit_counts_per_client(self) -> list[dict[str, Any]]:
        query = """
        SELECT client_id, COUNT(*) AS visit_count 
        FROM visits GROUP BY client_id;
        """
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
            WHERE p.client_id = c.client_id AND p.expiration_date > CURRENT_DATE
        );
        """
        return self.execute_query(query)

    def get_top_trainers_last_month(self) -> list[dict[str, Any]]:
        query = """
        SELECT t.trainer_id, COUNT(v.visit_id) AS total_visits
        FROM trainers t
        JOIN schedule s ON t.trainer_id = s.trainer_id
        JOIN visits v ON s.schedule_id = v.schedule_id
        WHERE v.visit_date >= CURRENT_DATE - INTERVAL '30 days'
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
            WHERE v.client_id = c.client_id AND v.visit_date >= CURRENT_DATE - INTERVAL '6 months'
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
            WHERE purchase_date >= CURRENT_DATE - INTERVAL '1 year'
            GROUP BY membership_id
        ),
        previous_year AS (
            SELECT 
                membership_id,
                COUNT(*) AS previous_purchases,
                RANK() OVER (ORDER BY COUNT(*) DESC) AS previous_rank
            FROM purchases
            WHERE purchase_date BETWEEN CURRENT_DATE - INTERVAL '2 years' 
                                   AND CURRENT_DATE - INTERVAL '1 year'
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
            SELECT DATE_TRUNC('month', CURRENT_DATE) - INTERVAL '12 months' AS month
            UNION ALL
            SELECT month + INTERVAL '1 month'
            FROM months
            WHERE month < DATE_TRUNC('month', CURRENT_DATE)
        )
        SELECT 
            TO_CHAR(month, 'YYYY-MM') AS month,
            COUNT(v.visit_id) AS visits
        FROM months
        LEFT JOIN visits v 
            ON TO_CHAR(v.visit_date, 'YYYY-MM') = TO_CHAR(month, 'YYYY-MM')
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
                AVG(COUNT(*)) OVER () AS avg_visits 
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
                      AND p.expiration_date > CURRENT_DATE
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
            (MIN(v.visit_date) - c.registration_date) AS days_to_first_visit,
            COUNT(v.visit_id) AS visit_count
        FROM clients c
        JOIN visits v ON c.client_id = v.client_id
        WHERE c.registration_date >= CURRENT_DATE - INTERVAL '30 days'
        GROUP BY c.client_id
        HAVING (MIN(v.visit_date) - c.registration_date) <= 30
        ORDER BY days_to_first_visit;
        """
        return self.execute_query(query)

    def get_clients_with_expired_memberships(self) -> list[dict[str, Any]]:
        query = """
        SELECT 
            c.client_id,
            c.first_name || ' ' || c.last_name AS full_name,
            MAX(p.expiration_date) AS expiration_date,
            COUNT(CASE WHEN v.visit_date <= p.expiration_date THEN 1 END) AS visits_before,
            COUNT(CASE WHEN v.visit_date > p.expiration_date THEN 1 END) AS visits_after,
            MAX(v.visit_date) - MAX(p.expiration_date) AS days_after_expiration
        FROM clients c
        JOIN purchases p ON c.client_id = p.client_id
        LEFT JOIN visits v ON c.client_id = v.client_id
        WHERE p.expiration_date < CURRENT_DATE
        GROUP BY c.client_id
        HAVING COUNT(CASE WHEN v.visit_date > p.expiration_date THEN 1 END) > 0
        ORDER BY days_after_expiration DESC;
        """
        return self.execute_query(query)

    def get_most_loyal_clients_per_trainer(self) -> list[dict[str, Any]]:
        query = """
        WITH TrainerClientVisits AS (
            SELECT
                t.trainer_id,
                t.first_name || ' ' || t.last_name AS trainer_name,
                c.client_id,
                c.first_name || ' ' || c.last_name AS client_name,
                COUNT(v.visit_id) AS visit_count
            FROM trainers t
            JOIN schedule s ON t.trainer_id = s.trainer_id
            JOIN visits v ON s.schedule_id = v.schedule_id
            JOIN clients c ON v.client_id = c.client_id
            GROUP BY t.trainer_id, c.client_id
        ),
         RankedClients AS (
             SELECT
                 trainer_name,
                 client_name,
                 visit_count,
                 RANK() OVER (PARTITION BY trainer_id ORDER BY visit_count DESC) AS rank
             FROM TrainerClientVisits
         )
        SELECT
            trainer_name,
            client_name,
            visit_count
        FROM RankedClients
        WHERE rank = 1
        ORDER BY trainer_name;
        """
        return self.execute_query(query)

    def close(self):
        """Закрывает соединение с базой данных."""
        self.cursor.close()
        self.conn.close()

if __name__ == "__main__":
    db = DatabaseHelper()

    # print("1. Все клиенты:")
    # print(db.get_all_clients())
    #
    # print("\n2. Клиенты, зарегистрированные за последний месяц:")
    # print(db.get_recent_clients())
    #
    # print("\n3. Тренеры по дате найма:")
    # print(db.get_trainers_by_hire_date())
    #
    # print("\n4. Количество посещений на клиента:")
    # print(db.get_visit_counts_per_client())
    #
    # print("\n5. Уникальные даты покупок абонементов:")
    # print(db.get_unique_purchase_dates())
    #
    # print("\n6. Клиенты с истекшими абонементами:")
    # print(db.get_expired_memberships())
    #
    # print("\n7. Тренеры с наибольшим количеством занятий за месяц:")
    # print(db.get_top_trainers_last_month())
    #
    # print("\n8. Средняя стоимость абонемента по типам:")
    # print(db.get_average_membership_price_by_type())
    #
    # print("\n9. Клиенты, не посещавшие занятия за последние 6 месяцев:")
    # print(db.get_inactive_clients_last_6_month())
    #
    # print("\n10. Ранжирование абонементов по популярности:")
    # print(db.get_membership_purchase_ranking())
    #
    # print("\n11. Тренеры с наибольшим количеством посещений за последний месяц:")
    # print(db.get_top_trainers_last_month())
    #
    # print("\n12. Топ дней недели по посещениям:")
    # print(db.get_top_popular_days())
    #
    # print("\n13. Клиенты, купившие абонементы, но ни разу не посещавшие занятия:")
    # print(db.get_clients_with_no_visits())
    #
    # print("\n14. Динамика посещений по месяцам за последний год:")
    # print(db.get_monthly_visit_trends())
    #
    # print("\n15. Клиенты, посещающие занятия чаще, чем в среднем по клубу:")
    # print(db.get_clients_above_average_visits())
    #
    # print("\n16. Тренеры, работающие в пиковое время (с 17:00 до 20:00):")
    # print(db.get_trainers_during_peak_hours())
    #
    # print("\n17. Детальная статистика клиентов:")
    # print(db.get_client_statistics())
    #
    # print("\n18. Активность новых клиентов:")
    # print(db.get_new_client_activity())
    #
    # print("\n19. Клиенты с просроченными абонементами и их история посещений:")
    # print(db.get_clients_with_expired_memberships())

    print("\n20. Самые преданные клиенты для каждого тренера:")
    print(db.get_most_loyal_clients_per_trainer())

    db.close()
