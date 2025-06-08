#include <stdio.h>
#include "list.h"

int input(List *list){
    printf("Input some words:\n");
    int letter = getchar();

    while (letter == ' ' || letter == '\t'){
        letter = getchar();
    }

    while (letter != '\n' && letter != EOF){
        list_push_back(list, letter);
        letter = getchar();
    }
    if (letter == EOF){
        return -1;
    }
    return 0;
}

int main(){
    List *list = list_new();
    int check = input(list);

    while (check != -1){
        printf("Initialized list:\n");
        list_print(list);

        printf("New list:\n");
        modify(list);
        list_print(list);

        list_delete(list);
        list = list_new();
        check = input(list);
    }

    list_delete(list);
    return 0;
}
