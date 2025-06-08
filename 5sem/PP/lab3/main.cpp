#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>
#include <climits>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <omp.h>
#include <set>

#define SEED 920215
#define SEED_INC 12345
#define RUNS_NUM 1

using std::cout, std::cin, std::endl, std::string, std::vector, std::set, std::swap, std::min;

void generate_random_array(vector<int> &array, const int seed){
    std::mt19937_64 gen(seed);
    std::uniform_int_distribution dist(INT_MIN, INT_MAX);

    for (int &el: array){
        el = dist(gen);
    }
}

void shell_sort(vector<int> &array, const int left, const int right){
    const int size = right - left + 1;

    for (int gap = size / 2; gap > 0; gap /= 2){
        for (int i = left + gap; i < right; ++i){
            for (int j = i - gap; j >= left && array[j] > array[j + gap]; j -= gap){
                swap(array[j], array[j + gap]);
            }
        }
    }
}

void merge(vector<int> &array, const int left, const int mid, const int right){
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right){
        if (array[i] <= array[j]){
            temp[k++] = array[i++];
        } else {
            temp[k++] = array[j++];
        }
    }
    while (i <= mid){
        temp[k++] = array[i++];
    }
    while (j <= right){
        temp[k++] = array[j++];
    }
    for (i = left; i <= right; ++i){
        array[i] = temp[i - left];
    }
}

void parallel_merge_blocks(vector<int> &array, const int size, int num_blocks){
    int block_size = size / num_blocks;

    while (block_size < size){
#pragma omp parallel for num_threads(num_blocks) shared(block_size, size, num_blocks, array) default(none)
        for (int i = 0; i < num_blocks; i += 2){
            const int left = i * block_size;
            const int mid = left + block_size - 1;
            const int right = min(left + 2 * block_size - 1, size - 1);

            if (mid < right){
                merge(array, left, mid, right);
            }
        }
        block_size *= 2;
        num_blocks /= 2;
    }
}

void tile_based_shell_sort(vector<int> &array, const int num_blocks){
    const int size = array.size();
    const int block_size = size / num_blocks;
#pragma omp parallel for num_threads(num_blocks) shared(num_blocks, block_size, array, size) default(none)
    for (int i = 0; i < num_blocks; ++i){
        const int start = i * block_size;
        const int end = (i == num_blocks - 1) ? size : start + block_size;
        shell_sort(array, start, end);
    }
    parallel_merge_blocks(array, size, num_blocks);
}

void time_algorithm(vector<int> &array, const int &threads_num){
    double time_spent = 0;
    int seed = SEED;

    for (int i = 0; i < RUNS_NUM; i++){
        generate_random_array(array, seed);
        seed += SEED_INC;

        const double start = omp_get_wtime();
        tile_based_shell_sort(array, threads_num);
        const double end = omp_get_wtime();

        time_spent += end - start;
    }
    cout << time_spent << endl;
}

int main(){
    constexpr int size = 2e8;
    vector<int> array(size);
    const int threads_num = omp_get_num_procs();

    assert(size >= threads_num);
    cout << "OpenMP: " << _OPENMP << endl;

    cout << "Parallel shell sort:" << endl;
    time_algorithm(array, threads_num);

    return 0;
}
