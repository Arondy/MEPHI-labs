#include <stdio.h>
#include <string.h>
#include "point.h"

Point point_new(int x, int y, char *z){
    Point p = {x, y, z};
    return p;
}

void point_print(const Point *p) {
    printf("{%d, %d, %s}", p->x, p->y, p->z);
}

void point_array_print(const char *msg, const Point *arr, int len) {
    printf("%s", msg);

    for (int i = 0; i < len; ++i) {
        printf("a[%d] = ", i);
        point_print(arr + i);
        printf("\n");
    }
}

int point_cmp_x(const Point *p1, const Point *p2) {
    return p1->x - p2->x;
}

int point_cmp_x_inv(const Point *p1, const Point *p2) {
    return p2->x - p1->x;
}

int point_cmp_y(const Point *p1, const Point *p2) {
    return p1->y - p2->y;
}

int point_cmp_y_inv(const Point *p1, const Point *p2) {
    return p2->y - p1->y;
}

int point_cmp_z(const Point *p1, const Point *p2) {
    return strcmp(p1->z, p2->z);
}

int point_cmp_z_inv(const Point *p1, const Point *p2) {
    return strcmp(p2->z, p1->z);
}

void *comp(int num1, int num2){
    if (num1 == 1 && num2 == 1){
        return (int (*)(const void *, const void *)) point_cmp_x;
    }
    else if (num1 == 1 && num2 == 2){
        return (int (*)(const void *, const void *)) point_cmp_x_inv;
    }
    else if (num1 == 2 && num2 == 1){
        return (int (*)(const void *, const void *)) point_cmp_y;
    }
    else if (num1 == 2 && num2 == 2){
        return (int (*)(const void *, const void *)) point_cmp_y_inv;
    }
    else if (num1 == 3 && num2 == 1){
        return (int (*)(const void *, const void *)) point_cmp_z;
    }
    else if (num1 == 3 && num2 == 2){
        return (int (*)(const void *, const void *)) point_cmp_z_inv;
    }
    else {
        return NULL;
    }
}