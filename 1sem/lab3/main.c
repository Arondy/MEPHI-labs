#include <stdio.h>
#include "menu.h"

int main(){
    int *A_array = NULL;
    int *B_array = NULL;
    int numA = 0;
    int numB = 0;
    int used = 0;
    menu(A_array, B_array, &numA, &numB, &used);
    free(A_array);
    free(B_array);
    return 0;
}
