#include <string.h>
#include "point.h"

void swap(void* v1, void* v2, int size){
    char buffer[size];

    memcpy(buffer, v1, size);
    memcpy(v1, v2, size);
    memcpy(v2, buffer, size);
}

void bubble_sort(Point *base, int len, int(*compar)(const Point *, const Point *)){
    for (int i = 0; i < len; i++){
        for (int j = len - 1; j > i; j--){
            if (compar(base + j, base + j - 1) < 0){
                swap(&base[j], &base[j - 1], sizeof(Point));
            }
        }
    }
}

void double_selection_sort(Point *base, int len, int(*compar)(const Point *, const Point *)){
    Point saved;
    for (int i = 0; i < (len / 2); i++){
        Point *min = &base[i];
        Point *max = &base[i];

        for (int j = i; j < len - i; j++){
            if (compar(base + j, min) < 0){
                min = (base + j);
            }
            else if (compar(base + j, max) > 0){
                max = (base + j);
            }
        }

        saved = *max;
        swap(&base[i], min, sizeof(Point));

        if (max->x == saved.x && max->y == saved.y){
            swap(&base[len - 1 - i], max, sizeof(Point));
        }
        else {
            swap(&base[len - 1- i], min, sizeof(Point));
        }
    }
}