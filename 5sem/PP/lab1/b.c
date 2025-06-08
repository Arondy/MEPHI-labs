#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void initialize_array(const int random_seed, int *array, const int count){
    srand(random_seed);

    for (int i = 0; i < count; i++){
        array[i] = rand();
    }
}

double find_max_element(const int *array, const int threads, const int count){
    int max = -1;                   ///< The maximal element
    int num;
    double time_spent;

#pragma omp parallel num_threads(threads) shared(array, count, time_spent) private(num) reduction(max: max) default(none)
    {
        num = omp_get_thread_num();
        printf("%d\n", num);
        clock_t begin = clock();
#pragma omp for
        for (int i = 0; i < count; i++){
            if (array[i] > max){
                max = array[i];
            }
        }
        clock_t end = clock();
        time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
//        printf("-- My lmax is: %d;\n", max);
    }

//    printf("Max is: %d;\n", max);
    return time_spent;
}

int main(int argc, char **argv){
    const int count = 10000000;     ///< Number of array elements
    int random_seed = 920215;       ///< RNG seed
    double time_spent = 0;

    int *array = (int *) malloc(count * sizeof(int));   ///< The array we need to find the max in

    /* Determine the OpenMP support */
    printf("OpenMP: %d;\n======\n", _OPENMP);

    for (int t = 1; t <= 12; t++){
        for (int i = 0; i < 20; i++){
            initialize_array(random_seed, array, count);
            random_seed += 1234;
            time_spent += find_max_element(array, t, count);
        }
        printf("Threads: %d\nSeconds spent: %lf", t, time_spent);
        time_spent = 0;
    }
    return (0);
}