#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "queue.h"

int full_char(char** strarr, int last_id){
    int spaces = count_digits(last_id) + 1, digitsNum = 0;
    strarr[0] = malloc(1000 * sizeof(char));
    memset(strarr[0], '\0', 1000 * sizeof(char));
    sprintf(strarr[0], "%s","ID");

    for (int i = 1; i < last_id + 1; i++){
        strarr[i] = malloc(1000 * sizeof(char));
        memset(strarr[i], '\0', 1000 * sizeof(char));
        sprintf(strarr[i], "%d", i);
        digitsNum = count_digits(i);
        memset(&(strarr[i][digitsNum]), ' ', (spaces - digitsNum) * sizeof(char));
    }
    return spaces;
}

void print_timetable(char** strarr, int columnNum){
    for (int i = 0; i < columnNum; i++){
        printf("%s\n", strarr[i]);
    }
    printf("\n");
}

int main(){
    char** strarr = NULL;
    int processesNum = 0, quant = 0, last_id = 0;

    do {
        Pointer* pointer = malloc(sizeof(Pointer));
        last_id = read_process_input(pointer, &processesNum, &quant);

        if (last_id == -1){
            if (pointer->process_ptr){
                free(pointer->process_ptr);
            } else if (pointer->queue_ptr){
                free(pointer->queue_ptr);
            }
            free(pointer);
            return 0;
        }

        strarr = malloc((last_id + 1) * sizeof(char*));
        full_char(strarr, last_id);

        bubble_sort(pointer, processesNum);
        print_processes(pointer, processesNum);
        processing(pointer, &processesNum, quant, strarr, last_id + 1);
        print_timetable(strarr, last_id + 1);

        if (pointer->process_ptr){
            free(pointer->process_ptr);
        } else if (pointer->queue_ptr){
            QueueNode *ptr = pointer->queue_ptr->first, *ptr_prev;
            while (ptr){
                ptr_prev = ptr;
                ptr = ptr->next;
                free(ptr_prev->process);
                free(ptr_prev);
            }
            free(pointer->queue_ptr);
        }
        free(pointer);

        for (int i = 0; i < last_id + 1; i++){
            free(strarr[i]);
        }
        free(strarr);
    } while (1);
}