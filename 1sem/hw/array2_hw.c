#include <stdio.h>
#include <stdlib.h>

int *array_input(int *len){
    int *data = malloc(sizeof(int));
    data[0] = 0;
    int index = 0;
    int value = 0;
    printf("Throw me some numbers:\n");
    scanf("%d", &index);

    while (index){
        scanf("%d", &value);
        if (index + 1 > *len){
            data = realloc(data, (index + 1) * sizeof(int));
            for (int i = *len; i <= index; i++){
                data[i] = 0;
            }
            *len = index + 1;
        } else if (data[index] != 0){
            int prev = data[index];
            data = realloc(data, (*len) * sizeof(int));
            *len = *len + 1;

            for (int i = index + 1; i <= (*len - 1); i++){
                int curr = data[i];
                data[i] = prev;
                prev = curr;
            }
        }
        data[index] = value;
        scanf("%d", &index);
    }
    return data;
}

void print_array(int *data, int len){
    for (int i = 0; i < len; i++){
        printf("%d ", data[i]);
    }
}

int main(){
    int num = 0;
    int *array = array_input(&num);
    printf("Array: ");
    print_array(array, num);
    return 0;
}