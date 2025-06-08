#include <stdio.h>
#include <stdlib.h>

int read_input(int *input){
	int check = scanf("%d", input);
	if (check == -1){
		printf("Program has been stopped");
		return -1;
	}
	while (check == 0){
		scanf("%*[^\n]");
		printf("You can't input letters.\n");
		check = scanf("%d", input);
	}
    return 0;
}

int print_array(int *data, int len){
    for (int i = 0; i < len; i++){
        printf("%d ", data[i]);
    }
    printf("\n");
    return 0;
}

int *array_input(int **A_data, int *len, int *used){
    int index = 0;
    int value = 0;
    int chunk = 5;
    printf("A_array: ");
    print_array(*A_data, *len);
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
        
        if (*used >= *len){
        	*len += chunk;
        	*A_data = realloc(*A_data, (*len) * sizeof(int));
        	for (int i = (*len - chunk); i < *len; i++){
        		(*A_data)[i] = 0;
        	}
        }
                	
        if (index > *used){
        	printf("Adding element to the end of the real-used memory!\n");
        	(*A_data)[*used] = value;
        }
        else if (index < *used){
            for (int i = *used; i > index; i--){
                (*A_data)[i] = (*A_data)[i - 1];
            }
            (*A_data)[index] = value;
        }
        else if (index == *used){
        	(*A_data)[index] = value;
        }
        *used += 1;
        printf("A_array: ");
        print_array(*A_data, *len);
    }
    while (index >= 0);
    *len = *used;
    *A_data = realloc(*A_data, *used * sizeof(int));

    return 0;
}

int *array_rm(int **A_data, int *len){
    int index = 0;

    do {
        printf("Input index of an element you want to remove. Otherwise, input -1.\n");
        read_input(&index);
        if ((index >= 0) && (index < *len)){
            for (int i = index; i < (*len - 1); i++){
                (*A_data)[i] = (*A_data)[i + 1];
            }
            *len -= 1;
            *A_data = realloc(*A_data, *len * sizeof(int));
            printf("A_array: ");
            print_array(*A_data, *len);
        }
        else if (index >= *len){
            printf("Index must be less than a length of an array!\n");
        }
    }
    while (index != -1);
    return 0;
}

int *new_array(int *A_data, int A_len, int *B_len){
	int *B_data = NULL;
    *B_len = A_len / 3;
    if (*B_len != 0){
    	B_data = malloc((*B_len) * sizeof(int));
    }
    
    for (int i = 0; i < *B_len; i++){
        B_data[i] = A_data[3 * i] + A_data[3 * i + 1] + A_data[3 * i + 2];
    }

    return B_data;
}

int *remove_duplicates(int **A_data, int *B_data, int *lenA, int lenB){
    for (int i = 0; i < *lenA; i++){
        for (int j = 0; j < lenB; j++){
            if ((*A_data)[i] == B_data[j]){
            	for (int k = i; k <= *lenA; k++){
            		(*A_data)[k] = (*A_data)[k + 1];
            	}
            	i -= 1;
            	*lenA -= 1;
            	break;
            }
        }
    }
    *A_data = realloc(*A_data, (*lenA) * sizeof(int));
    
    return 0;
}
