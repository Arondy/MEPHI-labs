#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "list.h"


List *list_new(){
    return (List *) calloc(1, sizeof(List));
}

void list_print(const List *list){
    Item *ptr = list->first;
    printf("'");
    while (ptr){
        printf("%c", ptr->symbol);
        ptr = ptr->next;
    }
    printf("'");
    printf("\n");
}

int list_push_back(List *list, char symbol){
    Item *ptr = (Item *) malloc(sizeof(Item));
    ptr->symbol = symbol;
    ptr->next = NULL;

    if (!list->first){
        list->first = ptr;
        list->last = ptr;
    }
    else {
        list->last->next = ptr;
        list->last = ptr;
    }
    return 0;
}

int list_insert(Item *ptr, Item **ptr_prev, List *list, const char symbol){
    Item *new = (Item *) malloc(sizeof(Item));
    new->symbol = symbol;
    new->next = ptr;

    if (*ptr_prev){
        (*ptr_prev)->next = new;
        *ptr_prev = new;
    }
    else {
        list->first = new;
        *ptr_prev = new;
    }
    if (!ptr){
        list->last = new;
    }
    return 0;
}

int list_insert_chunk(Item *ptr, Item **ptr_prev, List *list, char *text, int len){
    List *new_list = list_new();

    for (int i = 0; i < len; i++){
        list_push_back(new_list, text[i]);
    }

    if (!(*ptr_prev)){
        *ptr_prev = new_list->first;
        list->first = *ptr_prev;
    }
    else {
        (*ptr_prev)->next = new_list->first;
    }

    new_list->last->next = ptr;
    *ptr_prev = new_list->last;
    return 0;
}

int list_remove(Item *ptr, Item *ptr_prev, List *list){
    if (ptr == list->first){
        list->first = ptr->next;
    }
    if (ptr == list->last){
        list->last = ptr_prev;
    }
    if (ptr_prev){
        ptr_prev->next = ptr->next;
    }
    free(ptr);
    return 0;
}

int is_sep(const char letter){
	return (letter == ' ' || letter == '\t');
}

int is_consonant(const char letter){
    char vowels[6] = "AEIOUY";
    int check = 1;
    for (int i = 0; i < 6; i++){
        char up = toupper(letter);
        char comp = vowels[i];

        if (up == comp){
            check = 0;
            break;
        }
    }
    return check;
}

int modify(List *list){
    char pref[] = "pref";
    int len = 4;
    Item *ptr_prev = NULL;
    Item *ptr_prev_2 = NULL;
    Item *ptr = list->first;
    int count = 0;
    int flag = 0;
    char letter;
    if (!ptr){
        return -1;
    }
    while (ptr){
        letter = ptr->symbol;
        if (!is_sep(letter) && count == 0){
            count = 1;
            if (is_consonant(letter)){
                #ifdef chunk
                list_insert_chunk(ptr, &ptr_prev, list, pref, len);
                #else
                for (int i = 0; i < len; i++){
                    list_insert(ptr, &ptr_prev, list, pref[i]);
                }
                #endif
            }
            flag = 1;
        }
        else if (is_sep(letter)){
            if (!count){
                list_remove(ptr, ptr_prev, list);
                ptr = ptr_prev->next;
                flag = 0;
            }
            else {
                flag = 1;
            }
            count = 0;
        }
        if (flag){
            ptr_prev_2 = ptr_prev;
            ptr_prev = ptr;
            ptr = ptr->next;
        }
    }
    if (is_sep(ptr_prev->symbol)){
        list_remove(ptr_prev, ptr_prev_2, list);
    }
    return 0;
}

void list_delete(List *list){
    Item *ptr = list->first, *ptr_prev;
    while (ptr){
        ptr_prev = ptr;
        ptr = ptr->next;
        free(ptr_prev);
    }
    free(list);
}
