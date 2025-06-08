#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>
#include <stdbool.h>

#define RUNS_NUM 3
#define NUMBER (unsigned int) 3e8

void save_many_res(const double *times, const int size){
    FILE *file = fopen("C:\\C++\\5sem\\PPD\\lab7\\res.txt", "a");

    if (!file){
        perror("Error opening file");
    }
    for (int i = 0; i < size; i++){
        fprintf(file, "%lf ", times[i]);
    }
    fprintf(file, "\n");

    fclose(file);
}

int *findPrimesInRangePre(unsigned int S, unsigned int N, int *primeCount){
    if (N < 2 || S > N){
        *primeCount = 0;
        return NULL;
    }

    bool *isPrime = (bool *) malloc((N - S + 1) * sizeof(bool));

    isPrime[0] = isPrime[1] = false;

#pragma omp parallel for num_threads(6) default(shared)
    for (unsigned int i = 2; i <= N; ++i){
        isPrime[i] = true;
    }

    unsigned int sqrtN = sqrt(N);

    for (unsigned int i = 2; i <= sqrtN; ++i){
        if (isPrime[i]){
            for (unsigned int j = i * i; j <= N; j += i){
                isPrime[j] = false;
            }
        }
    }

    unsigned int *primes = (unsigned int *) malloc((N / log(N) * 1.5) * sizeof(unsigned int));
    *primeCount = 0;

    for (unsigned int i = (S < 2 ? 2 : S); i <= N; ++i){
        if (isPrime[i]){
            primes[(*primeCount)++] = i;
        }
    }
    primes = realloc(primes, *primeCount * sizeof(unsigned int));

    free(isPrime);

    return primes;
}

int *findPrimesInRange1(unsigned int S, unsigned int N, const int *divisors, int divisorCount, int *primeCount, double *time){
    if (N < 2 || S > N){
        *primeCount = 0;
        return NULL;
    }

    bool *isPrime = (bool *) malloc((N - S + 1) * sizeof(bool));

    double s1 = omp_get_wtime();
    isPrime[0] = isPrime[1] = false;

#pragma omp parallel for num_threads(2) default(shared)
    for (unsigned int i = 2; i < N - S + 1; ++i){
        isPrime[i] = true;
    }

    for (unsigned int i = 0; i < divisorCount; ++i){
        unsigned int p = divisors[i];
        unsigned int start = fmax(p * p, ((S + p - 1) / p) * p);
#pragma omp parallel for num_threads(2) default(shared)
        for (unsigned int j = start - S; j <= N - S; j += p){
            isPrime[j] = false;
        }
    }
    double e1 = omp_get_wtime();

    unsigned int *primes = (unsigned int *) malloc((N / log(N) * 1.5) * sizeof(unsigned int));
    *primeCount = 0;

    double s2 = MPI_Wtime();
    for (unsigned int i = (S < 2 ? 2 : S); i <= N; ++i){
        if (isPrime[i - S]){
            primes[(*primeCount)++] = i;
        }
    }
    double e2 = MPI_Wtime();

    (*time) = e2 - s2 + e1 - s1;

    primes = realloc(primes, *primeCount * sizeof(unsigned int));
    free(isPrime);

    return primes;
}

int *findPrimesInRange(unsigned int S, unsigned int N, const int *divisors, int divisorCount, int *primeCount, double *time){
    if (N < 2 || S > N){
        *primeCount = 0;
        return NULL;
    }

    unsigned int *primes = (unsigned int *) malloc((N / log(N) * 2) * sizeof(unsigned int));
    *primeCount = 0;

    double s1 = MPI_Wtime();
#pragma omp parallel for num_threads(1) default(shared)
    for (unsigned int n = S; n <= N; n++){
        bool isPrime = true;
        for (unsigned int i = 0; i < divisorCount; i++){
            if (!(n % divisors[i])){
                isPrime = false;
                break;
            }
        }
        if (isPrime){
            primes[(*primeCount)++] = n;
        }
    }
    double e1 = MPI_Wtime();

    (*time) += e1 - s1;

    primes = realloc(primes, *primeCount * sizeof(unsigned int));

    return primes;
}

double combineResults(const unsigned int S, const unsigned int N, const int *sharedPrimes, int sharedPrimesCount, const int rank,
                    const int proc_num){
    double time = 0;
    int localPrimesCount = 0;
    int *localPrimes = findPrimesInRange(S, N, sharedPrimes, sharedPrimesCount, &localPrimesCount, &time);

    int totalCount;
    int *array = NULL;
    int *recvcounts = NULL;
    int *displs = NULL;

    double s1 = MPI_Wtime();
    if (!rank){
        recvcounts = (int *) malloc(proc_num * sizeof(int));
        displs = (int *) malloc(proc_num * sizeof(int));
    }

    MPI_Gather(&localPrimesCount, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (!rank){
        totalCount = 0;
        for (int i = 0; i < proc_num; ++i){
            displs[i] = totalCount;
            totalCount += recvcounts[i];
        }

        array = (int *) malloc(totalCount * sizeof(int));
    }

    MPI_Gatherv(localPrimes, localPrimesCount, MPI_INT, array, recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
    double e1 = MPI_Wtime();

    if (!rank){
        free(recvcounts);
        free(displs);
        free(array);
    }

    time += e1 - s1;

    free(localPrimes);
    return time;
}

void time_algorithm(){
    double timeSpent = 0;
    double maxTimeSpent = 0;
    int proc_num = -1;
    int rank = -1;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

    int sharedPrimesCount = 0;
    int *sharedPrimes = NULL;
    unsigned int sqrtN = sqrt(NUMBER);
    unsigned int workingRange = NUMBER - sqrtN;

    if (!rank){
        sharedPrimes = findPrimesInRangePre(0, sqrtN, &sharedPrimesCount);
    }

    double s1 = MPI_Wtime();
    MPI_Bcast(&sharedPrimesCount, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
    double e1 = MPI_Wtime();

    if (rank){
        sharedPrimes = malloc(sharedPrimesCount * sizeof(int));
    }
    double s2 = MPI_Wtime();
    MPI_Bcast(sharedPrimes, sharedPrimesCount, MPI_INTEGER, 0, MPI_COMM_WORLD);
    double e2 = MPI_Wtime();

//    timeSpent += e2 - s2 + e1 - s1;

    unsigned int S = sqrtN + workingRange / proc_num * rank;
    unsigned int N = sqrtN + workingRange / proc_num * (rank + 1);

    for (int i = 0; i < RUNS_NUM; i++){
        timeSpent += combineResults(S, N, sharedPrimes, sharedPrimesCount, rank, proc_num);
    }

    double *timeArray = NULL;

    if (!rank){
        timeArray = malloc(proc_num * sizeof(double));
    }

    MPI_Gather(&timeSpent, 1, MPI_DOUBLE, timeArray, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Reduce(&timeSpent, &maxTimeSpent, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (!rank){
        printf("%lf\n\n", maxTimeSpent / RUNS_NUM);

        for (int i = 0; i < proc_num; i++){
            printf("%lf ", timeArray[i] / RUNS_NUM);
        }
        save_many_res(timeArray, proc_num);
        free(timeArray);
    }
    free(sharedPrimes);
}

int main(int argc, char **argv){
    int ret = -1; ///< For return values
    int rank = -1; ///< This processor's number

    ret = MPI_Init(&argc, &argv);
    if (!rank){ printf("MPI Init returned (%d)\n", ret); }

    time_algorithm();

    ret = MPI_Finalize();

    return 0;
}
