#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <immintrin.h>
#include <omp.h>

#define SIZE 3e8
#define SEED 15032025

using std::vector, std::array, std::cout, std::endl;

double calculateDiscriminant(double a, double b, double c){
    return b * b - 4 * a * c;
}

double calculateRoot1(double a, double b, double discriminant){
    if (discriminant >= 0){
        return (-b + sqrt(discriminant)) / (2 * a);
    }
    return nan("");
}

double calculateRoot2(double a, double b, double discriminant){
    if (discriminant >= 0){
        return (-b - sqrt(discriminant)) / (2 * a);
    }
    return nan("");
}

vector<array<double, 3>> generateRandomVectorsAVX(const int count, const int seed) {
    cout << "Generating " << count << " random vectors" << endl;
    const auto start = std::chrono::high_resolution_clock::now();

    alignas(32) vector<array<double, 3>> result(count);
    const __m256d scale = _mm256_set1_pd(1e6);

#pragma omp parallel
    {
        const int thread_id = omp_get_thread_num();
        std::mt19937 generator(seed + thread_id);
        std::uniform_real_distribution<double> distribution(-1e6, 1e6);

#pragma omp for
        for (int i = 0; i < count; ++i) {
            __m256d rnd = _mm256_set_pd(
                distribution(generator),
                distribution(generator),
                distribution(generator),
                0.0 // Заполнитель для выравнивания
            );
            rnd = _mm256_mul_pd(rnd, scale);
            _mm256_store_pd(result[i].data(), rnd);
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
	cout << "Finished generating after " << elapsed.count() << " seconds" << endl;

    return result;
}

void timeCode(){
    const vector<array<double, 3>> equations = generateRandomVectorsAVX(SIZE, SEED);
	const auto start = std::chrono::high_resolution_clock::now();
    volatile double sum = 0.0;
    const int boundary = 3e8;

    // for (const auto &eq: equations){
    for (int i = 0; i < boundary; i++){
        array<double, 3> eq = equations[i];
        const double a = eq[0];
        const double b = eq[1];
        const double c = eq[2];

        double discriminant = calculateDiscriminant(a, b, c);
        const double root1 = calculateRoot1(a, b, discriminant);
        const double root2 = calculateRoot2(a, b, discriminant);

        if (!std::isnan(root1) && !std::isnan(root2) && std::isfinite(root1) && std::isfinite(root2)){
            sum += root1 - root2;
        } else{
            sum++;
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    // cout << "Elapsed time: " << elapsed.count() << " seconds;" << " Sum: " << sum << endl;
    cout << "Elapsed time: " << elapsed.count() << " seconds;" << endl;
}

int main(){
    timeCode();
    return 0;
}