#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <cstdio>
#include <cstring>
#include "sqlite/sqlite3.h"

#define LENGTH_SEED 424242
#define CHAR_SEED 909090

#define MAX_STRING_LENGTH 100000
#define AVERAGE_RECORD_SIZE (MAX_STRING_LENGTH / 2 + 3)
#define NUM_STRINGS (TARGET_BYTES / AVERAGE_RECORD_SIZE)
#define TARGET_BYTES (500 * 1024 * 1024)  // Целевой объём данных
#define CHARSET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define CHARSET_SIZE (sizeof(CHARSET) - 1)

#define FILENAME "../file.txt"
#define DB_FILENAME "../database.db"

using namespace std;

string generate_random_string(const int &len){
	static mt19937 gen(CHAR_SEED);
	static uniform_int_distribution<> char_dist(0, CHARSET_SIZE - 1);

	string str;
	str.reserve(len);

	for (int i = 0; i < len; ++i){
		str.push_back(CHARSET[char_dist(gen)]);
	}

	return str;
}

void generate_data(vector<string> &records){
	auto gen_start = chrono::high_resolution_clock::now();

	records.resize(NUM_STRINGS);
	mt19937 gen(LENGTH_SEED);
	uniform_int_distribution length_dist(1, MAX_STRING_LENGTH);

#pragma omp parallel for num_threads(12)
	for (size_t i = 0; i < NUM_STRINGS; i++){
		int len = length_dist(gen);
		string s = generate_random_string(len);
		string record = to_string(len) + " " + s + "\n";
		records[i] = record;
	}

	auto gen_end = chrono::high_resolution_clock::now();
	chrono::duration<double> gen_duration = gen_end - gen_start;
	cout << "Generated in " << gen_duration.count() << " seconds" << endl;
}

void write_data(const vector<string> &records){
	ofstream ofs(FILENAME, ios::binary);

	if (!ofs){
		cerr << "Ошибка открытия файла для записи." << endl;
		return;
	}

	auto write_start = chrono::high_resolution_clock::now();

	for (const auto &record: records){
		ofs.write(record.c_str(), record.size());
	}

	auto write_end = chrono::high_resolution_clock::now();
	chrono::duration<double> write_duration = write_end - write_start;
	cout << "Wrote in " << write_duration.count() << " seconds" << endl;

	ofs.close();
}

void read_data(){
	ifstream ifs(FILENAME, ios::binary);

	if (!ifs){
		cerr << "Ошибка открытия файла для чтения." << endl;
		return;
	}

	vector<string> records;
	records.reserve(NUM_STRINGS);

	auto read_start = chrono::high_resolution_clock::now();

	while (ifs.good()){
		string str;
		if (std::getline(ifs, str)){
			if (!str.empty()){
				records.push_back(str);
			}
		}
	}

	auto read_end = chrono::high_resolution_clock::now();
	chrono::duration<double> read_duration = read_end - read_start;
	cout << "Read in " << read_duration.count() << " seconds (length: " << records.size() << ")" << endl;

	ifs.close();
}

void write_data_sqlite(const vector<string> &records){
	sqlite3 *db = nullptr;

	if (sqlite3_open(DB_FILENAME, &db) != SQLITE_OK){
		std::cerr << "ERROR opening database" << std::endl;
		return;
	}

	char drop_table_sql[] = "DROP TABLE IF EXISTS records;";
	char *errMsg = nullptr;

	if (sqlite3_exec(db, drop_table_sql, nullptr, nullptr, &errMsg) != SQLITE_OK){
		std::cerr << "ERROR dropping table: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return;
	}

	char create_table_sql[128];

	std::snprintf(create_table_sql, sizeof(create_table_sql),
	              "CREATE TABLE IF NOT EXISTS records ("
	              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
	              "data VARCHAR(%d)"
	              ");", MAX_STRING_LENGTH);

	if (sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errMsg) != SQLITE_OK){
		std::cerr << "ERROR creating table: " << errMsg << std::endl;
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return;
	}

	const char *insert_sql = "INSERT INTO records(data) VALUES (?);";
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) != SQLITE_OK){
		std::cerr << "ERROR preparing insert statement" << std::endl;
		sqlite3_close(db);
		return;
	}

	sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

	auto start_time = std::chrono::high_resolution_clock::now();

	for (const auto &s: records){
		sqlite3_bind_text(stmt, 1, s.c_str(), -1, SQLITE_TRANSIENT);

		if (sqlite3_step(stmt) != SQLITE_DONE){
			std::cerr << "ERROR inserting record" << std::endl;
		}

		sqlite3_reset(stmt);
	}

	sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> insert_duration = end_time - start_time;
	std::cout << "SQL wrote in " << insert_duration.count() << " seconds" << std::endl;

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void read_data_sqlite(){
	sqlite3 *db = nullptr;

	if (sqlite3_open(DB_FILENAME, &db) != SQLITE_OK){
		cerr << "ERROR opening database" << endl;
		return;
	}

	const char *query_sql = "SELECT data FROM records;";
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, query_sql, -1, &stmt, nullptr) != SQLITE_OK){
		cerr << "ERROR preparing query" << endl;
		sqlite3_close(db);
		return;
	}

	vector<string> records;
	records.reserve(NUM_STRINGS);

	auto start_time = chrono::high_resolution_clock::now();

	while (sqlite3_step(stmt) == SQLITE_ROW){
		const unsigned char *data_text = sqlite3_column_text(stmt, 0);
		records.emplace_back(reinterpret_cast<const char *>(data_text));
	}

	auto end_time = chrono::high_resolution_clock::now();
	chrono::duration<double> read_duration = end_time - start_time;
	cout << "SQL read in " << read_duration.count() << " seconds (length: " << records.size() << ")" << endl;

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void read_data_with_substring(const string &substr){
	std::ifstream ifs(FILENAME, std::ios::binary);

	if (!ifs){
		std::cerr << "ERROR opening file" << std::endl;
		return;
	}

	vector<string> selected_records;
	selected_records.reserve(69144);

	auto start_time = std::chrono::high_resolution_clock::now();

	string line;

	while (std::getline(ifs, line)){
		if (std::strstr(line.c_str(), substr.c_str()) != nullptr){
			selected_records.push_back(line);
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end_time - start_time;
	std::cout << "Substring searched in " << duration.count()
			<< " seconds (length: " << selected_records.size() << ")" << std::endl;

	ifs.close();
}

void read_data_sqlite_with_substring(const string &substr){
	sqlite3 *db = nullptr;

	if (sqlite3_open(DB_FILENAME, &db) != SQLITE_OK){
		std::cerr << "ERROR opening database" << std::endl;
		return;
	}

	// Формируем SQL-запрос с оператором LIKE для поиска подстроки
	// Используем шаблон '*<substr>*'
	string glob_pattern = "*" + substr + "*";
	string query_sql = "SELECT data FROM records WHERE data GLOB '" + glob_pattern + "';";
	sqlite3_stmt *stmt = nullptr;

	if (sqlite3_prepare_v2(db, query_sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
		std::cerr << "ERROR preparing query" << std::endl;
		sqlite3_close(db);
		return;
	}

	vector<string> selected_records;
	selected_records.reserve(69144);

	auto start_time = std::chrono::high_resolution_clock::now();

	while (sqlite3_step(stmt) == SQLITE_ROW){
		const unsigned char *data_text = sqlite3_column_text(stmt, 0);
		if (data_text){
			selected_records.emplace_back(reinterpret_cast<const char *>(data_text));
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end_time - start_time;

	std::cout << "SQL substring searched in " << duration.count()
			<< " seconds (length: " << selected_records.size() << ")" << std::endl;

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

int main(){
	vector<string> records;
	records.reserve(NUM_STRINGS);
	generate_data(records);

	write_data(records);
	write_data_sqlite(records);

	// read_data_sqlite();
	// read_data();

	// const string substr = "abc";
	// read_data_with_substring(substr);
	// read_data_sqlite_with_substring(substr);

	return 0;
}
