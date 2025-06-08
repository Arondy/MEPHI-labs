#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void initialize_array(const int random_seed, int *array, const int count){
    srand(random_seed);

    for (int i = 0; i < count; i++){
        array[i] = rand() * rand() * (i % 2 == 0 ? 2 : 3);
    }
}

double find_element_index(const int *array, const int count, const int target, const int threads){
    int num;
    int index = -1;
    int elementIsFound = 0;
    double time_spent = 0;
    double begin = omp_get_wtime();

#pragma omp parallel num_threads(threads) shared(array, count, target, index, elementIsFound) private(num) default(shared)
    {
        num = omp_get_thread_num();
#pragma omp for schedule(auto)
        for (int i = 0; i < count; i++){
            if (elementIsFound){
                i = count;
                continue;
            }
            if (array[i] == target){
                index = i;
                printf("%d\n", index);
                elementIsFound = 1;
            }
        }
    }
    double end = omp_get_wtime();
    time_spent = end - begin;

    return time_spent;
}

void time_algorithm(const int count, int *array, int random_seed, const int threads, const int runs_num,
                    double time_spent, const int target){
    for (int t = 1; t <= 12; t++){
        for (int i = 0; i < runs_num; i++){
            initialize_array(random_seed, array, count);
            random_seed += 12345;
            time_spent += find_element_index(array, count, target, t);
        }
        printf("%lf\n", time_spent / runs_num);
        time_spent = 0;
        random_seed = 920214;
    }
}

int main(int argc, char **argv){
    const int count = 10000000; ///< Number of array elements
    const int random_seed = 920214; ///< RNG seed
    const int target = INT_MAX; ///< Number to look for
    const int threads = 12;
    int *array = malloc(count * sizeof(int));
    int index = -1; ///< The index of the element we need
    const int runs_num = 20;
    const double time_spent = 0;

    printf("OpenMP: %d\n", _OPENMP);

    time_algorithm(count, array, random_seed, threads, runs_num, time_spent, target);

    return 0;
}
