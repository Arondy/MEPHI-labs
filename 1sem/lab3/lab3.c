#include <stdio.h>
#include <stdlib.h>

void read_input(int *input){
    char c;
    while (!scanf("%d", input) || (getchar() != '\n')){
        while ((c = getchar()) != '\n' && (c != EOF));
        printf("You can't input letters.\n");
    }
}

void print_array(int *data, int len){
    for (int i = 0; i < len; i++){
        printf("%d ", data[i]);
    }
    printf("\n");
}

int *array_input(int *A_data, int *len){
    int index = 0;
    int value = 0;
    printf("Input an index of a new element, or -1 to stop this input:\n");

    do {
        read_input(&index);
        while (index < -1){
            printf("Please input an index in positive numbers:\n");
            read_input(&index);
        }
        if (index == -1){
            break;
        }

        read_input(&value);
        if (index + 1 > *len){
            A_data = realloc(A_data, (index + 1) * sizeof(int));
            for (int i = *len; i <= index; i++){
                A_data[i] = 0;
            }
            *len = index + 1;
        }
        else if (A_data[index] != 0){
            *len += 1;
            A_data = realloc(A_data, (*len) * sizeof(int));

            for (int i = (*len - 1); i > index; i--){
                A_data[i] = A_data[i - 1];
            }
        }
        A_data[index] = value;
    }
    while (index >= 0);

    return A_data;
}

int *array_rm(int *A_data, int *len){
    int index = 0;

    do {
        printf("Input index of an element you want to remove. Otherwise, input -1.\n");
        read_input(&index);
        if ((index >= 0) && (index < *len)){
            for (int i = index; i < (*len - 1); i++){
                A_data[i] = A_data[i + 1];
            }
            *len = (*len) - 1;
            A_data = realloc(A_data, *len * sizeof(int));
            printf("Your array: ");
            print_array(A_data, *len);
        }
        else if (index >= *len){
            printf("Index must be less than a length of an array!\n");
        }
    }
    while (index != -1);

    return A_data;
}

int *new_array(int *A_data, int A_len, int *B_len){
    *B_len = A_len / 3;
    int *B_data = malloc((*B_len) * sizeof(int));

    for (int i = 0; i < *B_len; i++){
        B_data[i] = A_data[3 * i] + A_data[3 * i + 1] + A_data[3 * i + 2];
    }

    return B_data;
}

int *remove_duplicates(int *A_data, int *B_data, int lenA, int lenB, int *lenC){
    int *C_data = NULL;
    int flag = 1;
    *lenC = 0;

    for (int i = 0; i < lenA; i++){
        flag = 1;

        for (int j = 0; j < lenB; j++){
            if (A_data[i] == B_data[j]){
                flag = 0;
                break;
            }
        }
        if (flag){
            *lenC += 1;
            C_data = realloc(C_data, (*lenC) * sizeof(int));
            C_data[*lenC - 1] = A_data[i];
        }
    }

    return C_data;
}

void menu(int *A_array, int *B_array, int *C_array, int *lenA, int *lenB, int *lenC){
	int input = 0;

	while (input != -100){
		printf("Input 1 to add some elements to the array. Input 2 to delete some elements from the array. Input 3 to start the main function. Input -1 to stop program:\n");
        read_input(&input);
		switch (input){
			case 1:
				A_array = array_input(A_array, lenA);
				printf("A_array: ");
				print_array(A_array, *lenA);
				break;
			case 2:
				A_array = array_rm(A_array, lenA);
				break;
			case 3:
				B_array = new_array(A_array, *lenA, lenB);
				printf("B_array: ");
				print_array(B_array, *lenB);
				C_array = remove_duplicates(A_array, B_array, *lenA, *lenB, lenC);
				printf("C_array: ");
				print_array(C_array, *lenC);
				break;
			case -1:
				printf("Program has been stopped");
				input = -100;
				break;
			default:
				printf("Incorrect value!\n");
		}
	}
}

int main(){
    int *A_array = NULL;
    int *B_array = NULL;
    int *C_array = NULL;
    int numA = 0;
    int numB = 0;
    int numC = 0;
    menu(A_array, B_array, C_array, &numA, &numB, &numC);
    return 0;
}