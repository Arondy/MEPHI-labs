#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "point.h"
#include "sort.h"

int read_input(int *input, int num1, int num2){
    int check = scanf("%d", input);
    if (check == -1){
        printf("Program has been stopped");
        return -1;
    }
    while (check == 0 || (num1 > *input) || (num2 < *input)){
        scanf("%*[^\n]");
        printf("You can't input such symbols.\n");
        check = scanf("%d", input);
    }
    return 0;
}

int menu(int *res, int *len, int *runs){
    int input = 0;

    printf("Choose sorting algorithm: 1 - qsort, 2 - bubble sort, 3 - double selection sort\n");
    if (read_input(&input, 1, 3) == -1){
        return -1;
    }
    res[0] = input;

    printf("Input a number of arrays:\n");
    if (read_input(&input, 1, 100000000) == -1){
        return -1;
    }
    *len = input;

    printf("Input a number of runs:\n");
    if (read_input(&input, 1, 100000000) == -1){
        return -1;
    }
    *runs = input;

    printf("Choose field: 1 - x, 2 - y, 3 - z\n");
    if (read_input(&input, 1, 3) == -1){
        return -1;
    }
    res[1] = input;

    printf("Choose direction: 1 - direct, 2 - inversed\n");
    if (read_input(&input, 1, 2) == -1){
        return -1;
    }
    res[2] = input;

    return 0;
}

int main(){
    int *res = calloc(3, sizeof(int));
    int len, runs, string_len;
    char *str = calloc(101, sizeof(char));
    char **str_arr = NULL;
    double ftime = 0;
    Point *arr = NULL;
    srand(time(NULL));
    if (menu(res, &len, &runs) == -1){
        return -1;
    }

    arr = calloc(len, sizeof(Point));
    str_arr = calloc(len, sizeof(char *));

    for (int i = 0; i < len; i++){
        string_len = rand() % 100 + 1;
        for (int j = 0; j < string_len; j++)
            str[j] = 'a' + rand() % 26;
        str[string_len] = '\0';
        str_arr[i] = strdup(str);
        arr[i] = point_new(rand(), rand() % 5, str_arr[i]);
        memset(str,0,101);
    }

    for (int i = 0; i < runs; i++){
        clock_t t0 = clock();

        if (res[0] == 1){
            qsort(arr, len, sizeof(Point), comp(res[1], res[2]));
        } else if (res[0] == 2){
            bubble_sort(arr, len, comp(res[1], res[2]));
        } else if (res[0] == 3){
            double_selection_sort(arr, len, comp(res[1], res[2]));
        }
        clock_t t1 = clock();
        ftime += (double)(t1 - t0) / CLOCKS_PER_SEC;
    }
    printf("arrays: %d\nsort: %d\nruns: %d\ntime: %lf\n", len, res[0], runs, ftime / runs);

    for (int i = 0; i < len; i++){
        free(str_arr[i]);
    }

    free(str_arr);
    free(str);
    free(arr);
    free(res);
    return 0;
}
