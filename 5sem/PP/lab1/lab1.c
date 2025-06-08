#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void create_array(int *array, const int count, const int random_seed){
    srand(random_seed);

    for (int i = 0; i < count; i++){
        array[i] = rand();
    }
}


double find_max_elem(const int *array, const int count, const int threads){
    int thr_number;
    double time_required;
    int max = -1;

#pragma omp parallel num_threads(threads) shared(array, count, time_required)  private(thr_number) reduction(max: max) default(none)
    {
        thr_number = omp_get_thread_num();
        //printf("%d\n", thr_number);

        double start = omp_get_wtime();

#pragma omp for
        for (int i = 0; i < count; i++){
            if (array[i] > max){
                max = array[i];
            }
        }
        double end = omp_get_wtime();
        time_required = end - start;
        //printf("-- My lmax is: %d;\n", max);
    }

    return time_required;
}


int main(){
    const int count = 1e7; ///< Number of array elements
    int random_seed = 920215; ///< RNG seed
    double time_required = 0;
    const int runs_amount = 15;
    int *array = (int *) malloc(count * sizeof(int));

    /* Determine the OpenMP support */
    printf("OpenMP: %d;\n======\n", _OPENMP);

    //printf("Threads amount %d\n",omp_get_num_procs());


    for (int i = 1; i <= 12; i++){
        for (int k = 0; k < runs_amount; k++){
            create_array(array, count, random_seed);
            time_required += find_max_elem(array, count, i);
            random_seed += 6928;
        }

        printf("Threads amount: %d Time: %lf\n", i, time_required / runs_amount);

        random_seed = 920215;
        time_required = 0;
    }

    return 0;
}
