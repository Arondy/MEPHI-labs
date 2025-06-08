#include <stdio.h>
#include <stdlib.h>

void process(int **array, int *len){
    int *new_arr = calloc(*len, sizeof(int));
    int new_len = 1;
    new_arr[0] = (*array)[0];
    for (int i = 1; i < *len; i++){
        if ((*array)[i]<=(*array)[i-1]){
            new_arr[new_len] = (*array)[i];
            new_len += 1;
        }
    }
    *array = new_arr;
    *len = new_len;
}

int main(){
    int *array = calloc(4, sizeof(int));
    int len = 4;
    array[0] = 0;
    array[1] = 9;
    array[2] = 3;
    array[3] = 7;
    process(&array, &len);
    for (int i = 0; i < len; i++){
        printf("%d ", array[i]);
    }
    return 0;
}