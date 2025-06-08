#ifndef SORT_H
#define SORT_H

void bubble_sort(Point *base, int len, int(*compar)(const Point *, const Point *));
void double_selection_sort(Point *base, int len, int(*compar)(const Point *, const Point *));

#endif