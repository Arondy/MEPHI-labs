#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <immintrin.h>
#include <omp.h>

#define SIZE 3e8
#define SEED 15032025

using std::vector, std::array, std::cout, std::endl;

std::vector<std::array<double, 3>> generateRandomVectorsAVX(int count, int seed) {
    std::cout << "Generating " << count << " random vectors" << std::endl;
    const auto start = std::chrono::high_resolution_clock::now();

    alignas(32) std::vector<std::array<double, 3>> result(count);
    __m256d scale = _mm256_set1_pd(1e6);

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
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
    std::cout << "Finished generating after " << elapsed.count() << " seconds" << std::endl;

    return result;
}

inline double calculateDiscriminant(double a, double b, double c){
    return b * b - 4 * a * c;
}

inline double calculateRoot1(double a, double b, double discriminant){
    if (discriminant >= 0){
        return (-b + sqrt(discriminant)) / (2 * a);
    }
    return nan("");
}

inline double calculateRoot2(double a, double b, double discriminant){
    if (discriminant >= 0){
        return (-b - sqrt(discriminant)) / (2 * a);
    }
    return nan("");
}

void timeCode(){
    vector<array<double, 3>> equations = generateRandomVectorsAVX(SIZE, SEED);
    const auto start = std::chrono::high_resolution_clock::now();

    for (const auto &eq: equations){
        const double a = eq[0];
        const double b = eq[1];
        const double c = eq[2];

        const double discriminant = calculateDiscriminant(a, b, c);
        const double root1 = calculateRoot1(a, b, discriminant);
        const double root2 = calculateRoot2(a, b, discriminant);

        // cout << a << "x^2 + " << b << "x + " << c << " = 0" << endl;
        // if (discriminant < 0){
        //     cout << "D < 0" << endl;
        // } else{
        //     cout << "Roots: x1 = " << root1 << ", x2 = " << root2 << endl;
        // }
        // cout << endl;
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
}

int main(){
    timeCode();
    return 0;
}
