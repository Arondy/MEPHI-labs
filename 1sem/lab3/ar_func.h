#ifndef AR_FUNC
#define AR_FUNC
#include <stdio.h>
#include <stdlib.h>

int read_input(int *input);
void print_array(int *data, int len);
int *array_input(int **A_data, int *len, int *used);
int *array_rm(int **A_data, int *len);
int *new_array(int *A_data, int A_len, int *B_len);
int *remove_duplicates(int **A_data, int *B_data, int *lenA, int lenB);

#endif
