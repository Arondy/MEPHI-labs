#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node *next;
}Node;

typedef struct List{
    Node *head;
}List;

void process(List *list){
    Node *item = list->head;
    for (int i = 0; i < 3; i++){
        item = item->next;
    }
    Node *new_last = item;
    item = item->next;
    Node *new_first = item;
    Node *prev_first = list->head;
    while (item->next){
        item = item->next;
    }
    Node *prev_last = item;

    prev_last->next = prev_first;
    new_last->next = NULL;
    list->head = new_first;
}

int main(){
    int array[6] = {1, -2, 3, 4, -5, 6};
    int len = 6;
    Node *first = (Node*)malloc(sizeof(Node));
    first->data = array[0];
    first->next = NULL;
    List *list = (List*)malloc(sizeof(List));
    list->head = first;
    Node *prev = first;
    for (int i = 1; i < len; ++i){
        Node *elem = (Node*)malloc(sizeof(Node));
        elem->data = array[i];
        prev->next = elem;
        //printf("%d %d\n", prev->data, elem->data);
        prev = elem;
    }
    process(list);
    Node *item = list->head;
    while (item){
        printf("%d ", item->data);
        item = item->next;
    }
    free(first);
    free(list);
}
