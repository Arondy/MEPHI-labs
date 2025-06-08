#include <stdlib.h>
#include "matrix.h"

int main(){
    Matrix *matrix = malloc(sizeof(Matrix));
    int linesNum = 0, len = 0;

    if (matrix_input(matrix, &linesNum, &len) == -1){
        return -1;
    }

    int *result = (int*) malloc(linesNum * sizeof(int));

    for (int i = 0; i < linesNum; i++){
        result[i] = find_max_duplicates(matrix->lineArray[i]);
    }

    matrix_print(matrix, linesNum);
    vector_print(result, linesNum);

    for (int i = 0; i < linesNum; i++){
        free(matrix->lineArray[i].numArray);
    }

    free(matrix->lineArray);
    free(matrix);
    free(result);
    return 0;
}