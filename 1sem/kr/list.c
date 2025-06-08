#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *first;
} List;

void process(List *list){

}

void list_print(const List *list){
    Node *ptr = list->first;
    printf("'");
    while (ptr){
        printf("%c", ptr->data);
        ptr = ptr->next;
    }
    printf("'");
    printf("\n");
}

int main(){
    List *list = calloc(1, sizeof(Node));
    process(list);
    list_print(list);
    return 0;
}