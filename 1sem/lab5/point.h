#ifndef POINT_H
#define POINT_H

typedef struct {
    int x, y;
    char *z;
} Point;

Point point_new(int x, int y, char *z);
void point_print(const Point *p);
void point_array_print(const char *msg, const Point *arr, int len);
void *comp(int num1, int num2);

#endif
