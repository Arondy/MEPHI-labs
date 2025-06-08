#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "point.h"
#include "sort.h"

int read_file(const char *path, int *len, Point **array, char ***str_arr){
    char buff[255];
    char *word;
    int x, y;
    int count = 0;
    Point new;

    FILE *file = fopen(path, "r");

    if (file == NULL){
        printf("Wrong filepath!\n");
        return -1;
    }

    fgets(buff, 255, file);
    *len = strtol(buff, NULL, 10);

    if (*len == 0){
        printf("File is empty!\n");
        return -1;
    }

    *array = calloc(*len, sizeof(Point));
    *str_arr = calloc(*len, sizeof(char *));

    while (fgets(buff, 255, file)){
        word = strtok(buff, " ");
        x = strtol(word, NULL, 10);
        word = strtok(NULL, " ");
        y = strtol(word, NULL, 10);
        word = strtok(NULL, " \n");
        printf("%d, %d, %s\n", x, y, word);
        (*str_arr)[count] = strdup(word);
        new = point_new(x, y, (*str_arr)[count]);
        (*array)[count] = new;
        count++;
    }

    point_array_print("Array from the file:\n", *array, *len);
    fclose(file);
    return 0;
}

int write_to_file(const char *path, int len, Point *array){
    FILE *file = fopen(path, "w");

    if (file == NULL){
        printf("Wrong filepath!\n");
        return -1;
    }

    for (int i = 0; i < len; i++){
        fprintf(file, "%d %d %s\n", array[i].x, array[i].y, array[i].z);
    }

    fclose(file);
    return 0;
}

int str_input(char *str){
    int check = scanf("%s", str);
    if (check == -1){
        printf("Program has been stopped");
        _Exit (EXIT_SUCCESS);
    }
    while (check == 0){
        scanf("%*[^\n]");
        printf("You can't str these symbols.\n");
        check = scanf("%s", str);
    }
    return 0;
}

int read_input(int *input, int num1, int num2){
    int check = scanf("%d", input);
    if (check == -1){
        printf("Program has been stopped");
        _Exit (EXIT_SUCCESS);
    }
    while (check == 0 || (num1 > *input) || (num2 < *input)){
        scanf("%*[^\n]");
        printf("You can't input such symbols.\n");
        check = scanf("%d", input);
    }
    return 0;
}

int menu(char *input_file, char *output_file, int *res){
    int input = 0;

    printf("Choose sorting algorithm: 1 - qsort, 2 - bubble sort, 3 - double selection sort\n");
    read_input(&input, 1, 3);
    res[0] = input;

    printf("Choose field: 1 - x, 2 - y, 3 - z\n");
    read_input(&input, 1, 3);
    res[1] = input;

    printf("Choose direction: 1 - direct, 2 - inversed\n");
    read_input(&input, 1, 2);
    res[2] = input;

    printf("Choose an input file:\n");
    str_input(input_file);

    printf("Choose an output file:\n");
    str_input(output_file);

    return 0;
}

int main(){
    char input_file[255];
    char output_file[255];
    int *res = calloc(3, sizeof(int));
    int len;
    Point *arr = NULL;
    char **str_arr = NULL;
    menu(input_file, output_file, res);

    int check = read_file(input_file, &len, &arr, &str_arr);

    if (check >= 0){
        point_array_print("Before sort:\n", arr, len);
        void *comp_func = comp(res[1], res[2]);

        if (res[0] == 1){
            qsort(arr, len, sizeof(Point), comp_func);
        }
        else if (res[0] == 2){
            bubble_sort(arr, len, comp_func);
        }
        else if (res[0] == 3){
            double_selection_sort(arr, len, comp_func);
        }

        point_array_print("After sort:\n", arr, len);
        write_to_file(output_file, len, arr);
    }

    for (int i = 0; i < len; i++){
        free(str_arr[i]);
    }

    free(str_arr);
    free(arr);
    free(res);
    return 0;
}
