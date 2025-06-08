#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <mpi.h>

int main(int argc, char **argv){
    int ret = -1;   ///< For return values
    int size = -1;   ///< Total number of processors
    int rank = -1;   ///< This processor's number

    const int count = 1e8; ///< Number of array elements
    const int base_seed = 920215; ///< RNG seed
    const int seed_step = 6928;
    const int runs_amount = 1;

    int *array = 0; ///< The array we need to find the max in
    int lmax = -1;  ///< Local maximums
    int max = -1;   ///< The maximal element

    /* Initialize the MPI */
    ret = MPI_Init(&argc, &argv);
    if (!rank){ printf("MPI Init returned (%d);\n", ret); }

    /* Determine our rank and processor count */
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Allocate the array */
    array = (int *) malloc(count * sizeof(int));

    double total_time = 0.0; // Accumulated time for averaging
    std::vector<int> exper;

    for (int t = 0; t < runs_amount; t++){
        int current_seed = base_seed + t * seed_step;

        /* Master generates the array */
        if (!rank){
            /* Initialize the RNG */
            srand(current_seed);
            /* Generate the random array */
            for (int i = 0; i < count; i++){ array[i] = rand(); }
        }

        /* Broadcast the array to all processors */
        MPI_Bcast(array, count, MPI_INTEGER, 0, MPI_COMM_WORLD);

        /* Define local start and end for each processor */
        const int wstart = (rank) * count / size;
        const int wend = (rank + 1) * count / size;

        /* Start timing */
        double start_time = MPI_Wtime();

        /* Find local maximum */
        lmax = array[wstart];
        for (int i = wstart; i < wend; i++){
            if (array[i] > lmax){ lmax = array[i]; }
        }

        /* Reduce to find the global maximum */
        //MPI_Reduce(&lmax, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);

        /* End timing */
        double end_time = MPI_Wtime();

        /* Calculate the elapsed time for this trial */
        double elapsed_time = end_time - start_time;
        total_time += elapsed_time; // Accumulate the time

        if (!rank){
            printf("Trial %d: Global Maximum = %d, Time = %f seconds;\n", t + 1, max, elapsed_time);
        }
    }

    /* Finalize MPI */
    ret = MPI_Finalize();

    /* Calculate and output the average time on the master processor */
    if (!rank){
        printf("\n*** Average Time for Maximum Search over %d runs_amount: %f seconds ***\n", runs_amount,
               total_time / runs_amount);
    }

    free(array); // Free allocated memory

    return 0;
}
