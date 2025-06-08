#include <cstdlib>
#include <cstdio>
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <fstream>

#define SEED 920215
#define SEED_INC 12345
#define RUNS_NUM 1
#define SIZE static_cast<int>(1e7)

using std::swap, std::min;

void generate_random_array(int *array, const int seed){
    std::mt19937_64 gen(seed);
    std::uniform_int_distribution dist(INT_MIN, INT_MAX);

    for (int i = 0; i < SIZE; i++){
        array[i] = dist(gen);
    }
}

void shell_sort(int *array, const int size){
    int k = 0;
    while (pow(2, k + 1) <= size + 1){
        k++;
    }
    for (int gap = pow(2, k) - 1; gap > 0; --k, gap = pow(2, k) - 1){
        for (int i = gap; i < size; ++i){
            for (int j = i - gap; j >= 0 && array[j] > array[j + gap]; j -= gap){
                swap(array[j], array[j + gap]);
            }
        }
    }
    int gap = 1;
    for (int i = gap; i < size; ++i){
        for (int j = i - gap; j >= 0 && array[j] > array[j + gap]; j -= gap){
            swap(array[j], array[j + gap]);
        }
    }
}

void inplace_merge(int *array, int size){
    int mid = size / 2;
    int left_size = mid + 1;
    int right_size = size - mid;

    // Создаем временные массивы для левого и правого подмассивов
    int *left_array = (int *) malloc(left_size * sizeof(int));
    int *right_array = (int *) malloc(right_size * sizeof(int));

    // Копируем данные во временные массивы
    memcpy(left_array, &array[0], left_size * sizeof(int));
    memcpy(right_array, &array[mid + 1], right_size * sizeof(int));

    // Сливаем два отсортированных временных массива обратно в исходный
    int i = 0, j = 0, k = 0;
    while (i < left_size && j < right_size){
        if (left_array[i] <= right_array[j]){
            array[k++] = left_array[i++];
        } else {
            array[k++] = right_array[j++];
        }
    }

    // Копируем оставшиеся элементы из левого подмассива (если есть)
    while (i < left_size){
        array[k++] = left_array[i++];
    }

    // Копируем оставшиеся элементы из правого подмассива (если есть)
    while (j < right_size){
        array[k++] = right_array[j++];
    }

    // Освобождаем временные массивы
    free(left_array);
    free(right_array);
}

void parallel_merge_blocks(int *array, int proc_num){
    int rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int block_size;
//    double time = 0;
//    double start = 0;
//    double end = 0;

    while (proc_num > 1){
        proc_num /= 2;
        block_size = SIZE / proc_num;

        if (rank >= proc_num){
            break;
        }

        int *local_array = static_cast<int *>(malloc(block_size * sizeof(int)));

        if (!rank){
            for (int i = 1; i < proc_num; ++i){
                MPI_Send(array + i * block_size, block_size, MPI_INT, i, 0, MPI_COMM_WORLD); // S1
            }
//            start = MPI_Wtime();
//            std::sort(array, array + block_size);
            inplace_merge(array, block_size);
//            end = MPI_Wtime();
        } else {
            MPI_Recv(local_array, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // R1
//            start = MPI_Wtime();
//            std::sort(local_array, local_array + block_size);
            inplace_merge(local_array, block_size);
//            end = MPI_Wtime();
        }

//        time += end - start;

        if (!rank){
            for (int i = 1; i < proc_num; ++i){
                MPI_Recv(array + i * block_size, block_size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // R2
            }
        } else {
            MPI_Send(local_array, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD); // S2
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
//    return time;
}

void tile_based_shell_sort(int *array, const int &proc_num){
    int rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int block_size = SIZE / proc_num;
    int *local_array = static_cast<int *>(malloc(block_size * sizeof(int)));


    MPI_Scatter(array, block_size, MPI_INT, local_array, block_size, MPI_INT, 0, MPI_COMM_WORLD);

//    const double start1, start = MPI_Wtime();
    shell_sort(local_array, block_size);
//    const double end1 = MPI_Wtime();
    double start1 = MPI_Wtime();
    MPI_Gather(local_array, block_size, MPI_INT, array, block_size, MPI_INT, 0, MPI_COMM_WORLD);
    double start2 = MPI_Wtime();
    printf("%lf\n", start2 - start1);
//    double merge_time = 0;

    if (proc_num > 1){
//        merge_time = parallel_merge_blocks(array, proc_num);
        parallel_merge_blocks(array, proc_num);
    }

    MPI_Barrier(MPI_COMM_WORLD);

//    return end1 - start1 + merge_time;
}

void save_res(const double time){
    FILE *file = fopen(R"(C:\C++\5sem\PPD\lab6\res.txt)", "a");

    if (!file){
        perror("Error opening file.");
    }

    fprintf(file, "%lf\n", time);
    fclose(file);
}

void time_algorithm(){
    double timeSpent = 0;
    double maxTimeSpent = 0;
    int seed = SEED;
    int proc_num = -1;
    int rank = -1;
    int *array = nullptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

    if (!rank){
        array = static_cast<int *>(malloc(SIZE * sizeof(int)));
    }

    for (int i = 0; i < RUNS_NUM; i++){
        if (!rank){
            generate_random_array(array, seed);
            seed += SEED_INC;
        }
        const double start = MPI_Wtime();
        tile_based_shell_sort(array, proc_num);
        const double end = MPI_Wtime();
        timeSpent += end - start;
//        timeSpent += tile_based_shell_sort(array, proc_num);
    }

    MPI_Reduce(&timeSpent, &maxTimeSpent, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (!rank){
//        for (int i = 0; i < SIZE; i++){
//            printf("%d ", array[i]);
//        }
//        printf("\n");

        save_res(maxTimeSpent / RUNS_NUM);
        printf("%lf", maxTimeSpent / RUNS_NUM);
    }
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