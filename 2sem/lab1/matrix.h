#ifndef MATRIX
#define MATRIX

typedef struct Line{
    int len;
    int *numArray;
} Line;

typedef struct Matrix{
    int linesNum;
    Line *lineArray;
} Matrix;

int read_input(int *input);
int non_negative_input(int *input);
int matrix_input(Matrix *matrix, int *linesNum, int *len);
int find_max_duplicates(Line line);
void matrix_print(Matrix *matrix, int linesNum);
void vector_print(int *line, int linesNum);

#endif