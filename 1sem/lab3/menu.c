#include <stdio.h>
#include "ar_func.h"

int bubble_sort(int *array, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d", i);
        for (int j = len - 1; j > i; j--) {
            if (array[j] < array[j - 1]) {
                int t = array[j - 1];
                array[j - 1] = array[j];
                array[j] = t;
            }
        }
    }
    return 0;
}

void menu(int *A_array, int *B_array, int *lenA, int *lenB, int *used){
    int input = 0;

    while (input != -100){
        printf("Input 0 to initialize array from the beginning.\nInput 1 to add some elements to the array.\nInput 2 to delete some elements from the array.\nInput 3 to start the main function.\nInput -1 to stop program:\n");
        read_input(&input);
        switch (input){
        	case 0:
        		free(A_array);
        		free(B_array);
        		*lenA = 0;
        		*lenB = 0;
        		*used = 0;
        		A_array = NULL;
        		B_array = NULL;
        		break;
            case 1:
                array_input(&A_array, lenA, used);
                bubble_sort(A_array, *lenA);
                break;
            case 2:
                array_rm(&A_array, lenA);
                break;
            case 3:
            	free(B_array);
                B_array = new_array(A_array, *lenA, lenB);
                printf("B_array: ");
                print_array(B_array, *lenB);
                remove_duplicates(&A_array, B_array, lenA, *lenB);
                printf("Res_array: ");
                print_array(A_array, *lenA);
                break;
            case -1:
                printf("Program has been stopped");
                input = -100;
//                free(A_array);
//                free(B_array);
                break;
            default:
                printf("Incorrect value!\n");
        }
    }
}
