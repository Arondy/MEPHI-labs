#include <iostream>
#include <vector>
#include <chrono>

void convolution(const float* __restrict__ A,
                 const float* __restrict__ B,
                 float* __restrict__ C,
                 size_t N) {
    for (size_t k = 0; k < N; ++k) {
        float sum = 0.0f;
        for (size_t i = 0; i <= k; ++i) {
            sum += A[i] * B[k - i];
        }
        C[k] = sum;
    }
}

void convolution_naive(const float* A, const float* B, float* C, size_t N) {
    for (size_t k = 0; k < N; ++k) {
        float sum = 0.0f;
        for (size_t i = 0; i <= k; ++i) {
            sum += A[i] * B[k - i];
        }
        C[k] = sum;
    }
}

int main() {
    const size_t N = 1e5;
    std::vector<float> A(N, 1.0f);
    std::vector<float> B(N, 2.0f);
    std::vector<float> C(N);

    auto start = std::chrono::high_resolution_clock::now();
    convolution(A.data(), B.data(), C.data(), N);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "with __restrict__: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    std::fill(C.begin(), C.end(), 0.0f);

    start = std::chrono::high_resolution_clock::now();
    convolution_naive(A.data(), B.data(), C.data(), N);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "without __restrict__: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    return 0;
}