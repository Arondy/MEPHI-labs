#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *array_input(int *len){
    printf("Enter size of array: ");
    scanf("%d", len);
    int *data = malloc(sizeof(int) * *len);

    if (!data){
        printf("error");
        return NULL;
    }

    for (int i = 0; i < *len; i++){
        printf("data[%d] = ", i);
        scanf("%d", &data[i]);
    }
    return data;
}

void print_array(int *data, int *len){
    for (int i = 0; i < *len; i++){
        printf("%d ", data[i]);
    }
}

void remove_duplicates(int *data, int *len){
    for (int i = 0; i < (*len - 1); i++){
        for (int j = i + 1; j < *len; j++){
            if (data[i] == data[j]){
                for (int k = j; k < (*len - 1); k++){
                    data[k] = data[k + 1];
                }
                (*len)--;
                j--;
            }
        }
    }
}

int main(){
    int num = 0;
    clock_t t1 = 0, t2 = 0;
    int *array = array_input(&num);
    printf("Initial array: ");
    t1 = clock();
    print_array(array, &num);
    printf("\nNew array: ");
    remove_duplicates(array, &num);
    t2 = clock();
    print_array(array, &num);
    printf("\n%lf", (double)(t2 - t1)/CLOCKS_PER_SEC);
    return 0;
}