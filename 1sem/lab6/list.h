#ifndef LIST_H
#define LIST_H

typedef struct Item {
    char symbol;
    struct Item *next;
} Item;

typedef struct List {
    Item *first;
    Item *last;
} List;

List *list_new();
void list_print(const List *list);
int modify(List *list);
int list_push_back(List *list, char symbol);
void list_delete(List *list);

#endif
