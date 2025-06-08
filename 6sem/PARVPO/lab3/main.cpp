#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <chrono>
#include <stdexcept>

#define ITERATIONS (int) 1e9
#define SEED (int) 424242
#define L1_SIZE (int) (16 * 1024)
#define MAX_K (40 * 16 * 1024 * 1024)  // Максимальный k = 640 МБ, в 40 раз больше размера L3 кэша

// Линейный конгруэнтный генератор (LCG)
class LCG {
private:
	uint64_t state;
	static constexpr uint64_t a = 1664525;
	static constexpr uint64_t c = 1013904223;
	static constexpr uint64_t m = 1ULL << 32;

public:
	explicit LCG(const uint64_t seed) : state(seed){}

	uint64_t next(){
		state = (a * state + c) % m;
		return state;
	}
};

// Функция для загрузки файла в буфер
std::vector<unsigned char> loadFile(const std::string &filename){
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (!file){
		throw std::runtime_error("Failed to open file");
	}

	const uint64_t file_size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<unsigned char> buffer(file_size);
	file.read(reinterpret_cast<char *>(buffer.data()), file_size);
	return buffer;
}

// Функция для выполнения одного эксперимента с заданным значением k.
// outSum передаёт накопленную сумму прочитанных значений для предотвращения оптимизации.
double runExperiment(const uint64_t &k, const std::vector<unsigned char> &buffer, const uint64_t &initial_offset,
                     LCG &rng, uint64_t &outSum){
	outSum = 0;
	const auto start = std::chrono::high_resolution_clock::now();

	for (uint64_t i = 0; i < ITERATIONS; ++i){
		// Генерация смещения в диапазоне [initial_offset, initial_offset + k)
		uint64_t offset = initial_offset + rng.next() % k;
		offset -= offset % 8; // Выравнивание по 8 байт

		// Проверка границ буфера
		if (offset + 8 > buffer.size()){
			offset = buffer.size() - 8;
		}

		// Чтение 8 байт и суммирование для предотвращения оптимизации
		const uint64_t value = *reinterpret_cast<const uint64_t *>(&buffer[offset]);
		outSum += value;
	}

	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> elapsed = end - start;
	return elapsed.count();
}

// Функция для проведения серии экспериментов с различными значениями k.
void performExperiments(const std::vector<unsigned char> &buffer){
	LCG rng(SEED);
	uint64_t initial_offset = rng.next();
	initial_offset %= (buffer.size() - MAX_K);

	// Выравнивание initial_offset до кратности 8
	initial_offset -= initial_offset % 8;

	uint64_t sum = 0;

	for (uint64_t k = L1_SIZE / 2; k <= MAX_K; k *= 2){
		const double elapsedTime = runExperiment(k, buffer, initial_offset, rng, sum);
		std::cout << "k = " << k << " | Time: " << elapsedTime << " s | Sum: " << sum << "\n";
	}
}

int main(){
	const std::string filename = "Berserk 10.mkv";

	try{
		const std::vector<unsigned char> buffer = loadFile(filename);
		performExperiments(buffer);
	} catch (const std::exception &e){
		std::cerr << e.what() << "\n";
		return 1;
	}

	return 0;
}