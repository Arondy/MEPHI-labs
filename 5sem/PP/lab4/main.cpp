#include <iostream>
#include <omp.h>
#include <random>
#include <algorithm>
#include <vector>

#define SEED 920215

using std::cout, std::endl, std::vector;

void algo_with_lock(){
    std::mt19937_64 gen(SEED);
    std::uniform_int_distribution dist(1, 6);
    std::vector<int> array;
    omp_lock_t lock;

    omp_init_lock(&lock);

#pragma omp parallel num_threads(12) shared(gen, dist, array)
    {
        const int thread_num = omp_get_thread_num();
        const int num = dist(gen);

        omp_set_lock(&lock);
        const bool isInArray = std::ranges::find(array, num) != array.end();

        if (!isInArray){
            cout << "Thread " << thread_num << " adds number " << num << endl;
            array.push_back(num);
        }
        omp_unset_lock(&lock);
    }
    omp_destroy_lock(&lock);
}

int main(){
    // 1
    cout << "OpenMP version: " << _OPENMP << endl;
    cout << "Year: " << _OPENMP / 100 << endl;

    // 2
    const int num_procs = omp_get_num_procs();
    const int max_threads = omp_get_max_threads();

    cout << "Num of processors: " << num_procs << endl;
    cout << "Max num of threads: " << max_threads << endl;

    // 3
    const int is_dynamic = omp_get_dynamic();
    cout << "Dynamic threading is " << (is_dynamic ? "on" : "off") << endl;

    // 4
    const double tick = omp_get_wtick();
    cout << "Timer resolution: " << tick << " (in seconds)" << endl;

    // 5
    const int nested = omp_get_nested();
    const int max_levels = omp_get_max_active_levels();

    cout << "Nested parallelism is " << (nested ? "on" : "off") << endl;
    cout << "Max active levels: " << max_levels << endl;

    // 6
    omp_sched_t schedule;
    int chunk_size;

    omp_get_schedule(&schedule, &chunk_size);

    cout << "Schedule: ";
    switch (schedule){
        case omp_sched_static:
            cout << "static";
            break;
        case omp_sched_dynamic:
            cout << "dynamic";
            break;
        case omp_sched_guided:
            cout << "guided";
            break;
        case omp_sched_auto:
            cout << "auto";
            break;
        default:
            cout << "unknown";
    }
    cout << endl;
    cout << "Chunk size: " << chunk_size << endl;

    // 7
    algo_with_lock();

    return 0;
}
