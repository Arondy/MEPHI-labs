#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int non_negative_input(int *input){
    int check = read_input(input);
    while (*input < 0 && check != -1){
        printf("You can't input negative values.\n");
        scanf("%*[^\n]");
        check = read_input(input);
    }
    if (check == -1){
        return -1;
    }
    return 0;
}

int read_input(int *input){
    int check = scanf("%d", input);

    while (check == 0){
        scanf("%*[^\n]");
        printf("You can't input such symbols.\n");
        check = scanf("%d", input);
    }

    if (check == -1){
        printf("Program has been stopped");
        return -1;
    }
    return 0;
}

int matrix_input(Matrix *matrix, int *linesNum, int *len){
    printf("Enter the number of lines in the matrix:\n");
    if (non_negative_input(linesNum) == -1){
        return -1;
    }

    matrix->linesNum = *linesNum;
    matrix->lineArray = (Line*) malloc(*linesNum * sizeof(Line));

    printf("Enter the number of elements in each line:\n");
    for (int i = 0; i < *linesNum; i++){
        printf("Number of elements in line %d:\n", i + 1);
        if (non_negative_input(len) == -1){
            return -1;
        }
        matrix->lineArray[i].len = *len;
        matrix->lineArray[i].numArray = (int*) malloc(*len * sizeof(int));

        printf("Enter the elements in line %d:\n", i + 1);
        for (int j = 0; j < *len; j++) {
            if (read_input(&matrix->lineArray[i].numArray[j]) == -1){
                return -1;
            }
        }
    }
    return 0;
}

int find_max_duplicates(Line line){
    int max = 0, count = 0;

    for (int i = 0; i < line.len; i++){
        for (int j = i + 1; j < line.len; j++){
            if (line.numArray[i] == line.numArray[j]){
                count++;
            }
        }
        if (count > max){
            max = count;
        }
        count = 1;
    }

    return max;
}

void matrix_print(Matrix *matrix, int linesNum){
    printf("The matrix is:\n[\n");
    for (int i = 0; i < linesNum; i++) {
        for (int j = 0; j < matrix->lineArray[i].len; j++){
            printf("%d ", matrix->lineArray[i].numArray[j]);
        }
        printf("\n");
    }
    printf("]\n");
}

void vector_print(int *line, int linesNum){
    printf("The resulting vector is:\n[");
    for (int i = 0; i < linesNum; i++){
        printf("%d ", line[i]);
    }
    printf("]\n");
}

//int find_max_duplicates_hash(Line line){
//    int max = 0;
//    int count[10000];
//    memset(count, 0, sizeof(count));
//
//    for (int i = 0; i < line.len; i++){
//        count[line.numArray[i]]++;
//        if (count[line.numArray[i]] > max) {
//            max = count[line.numArray[i]];
//        }
//    }
//    return max;
//}