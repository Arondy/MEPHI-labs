#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "queue.h"

#define MAXLEN 100

void print_processes(Pointer* ptr, int processesNum){
    Process *array = ptr->process_ptr;
    for (int i = 0; i < processesNum; i++){
        printf("%d/%d/%d\n", array[i].id, array[i].arrival_time, array[i].required_time);
    }
    printf("\n");
}

int read_process_input(Pointer* ptr, int* processesNum, int* quant){
    char process_string[255];
    char* token = NULL;
    int id = 0, arrival_time = 0, required_time = 0, len = 0, last_id = 0, check = 0;
    Process process;

    Process** arr = &(ptr->process_ptr);

    printf("Using vector\n");

    printf("Input quant, then id/tc/te:\n");
    if (positive_input(quant) == -1){
        return -1;
    }
    fgets(process_string, sizeof(process_string), stdin);
    len = strlen(process_string);
    process_string[len] = '\0';
    len--;

    for (int i = 0; i < len; i++){
        if (process_string[i] == '/'){
            (*processesNum)++;
        }
    }

    if (*processesNum > MAXLEN){
        printf("You can't input so many processes: max length is %d.\n", MAXLEN);
        free(arr);
        return -1;
    }

    *arr = malloc((*processesNum) / 2 * sizeof(Process));
    *processesNum = 0;

    token = strtok(process_string, " ");
    while (token){
        if (read_input3(token, &id, &arrival_time, &required_time) == -1){
            return -1;
        }


        if (last_id < id){
            last_id = id;
        }

        process.id = id;
        process.arrival_time = arrival_time;
        process.required_time = required_time;
        (*arr)[*processesNum] = process;
        (*processesNum)++;

        token = strtok(NULL, " ");
    }
    return last_id;
}

void bubble_sort(Pointer *ptr, int len){
    Process* array = ptr->process_ptr;
    for (int i = 0; i < len - 1; i++){
        for (int j = 0; j < len - i - 1; j++){
            if (array[j].arrival_time - array[j + 1].arrival_time > 0){
                Process tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
}

int processing(Pointer* ptr, int *processesNum, int quant, char** strarr, int columnNum){
    Process process;
    int time = 0, prev_time = 0, move_to_end = 1, length = 0, noProcessExecuted = 0;
    char nonExecuted[200] = {0}, executed[200] = {0};

    Process **array = &(ptr->process_ptr);
    while (*processesNum){
        process = (*array)[0];
        prev_time = time;

        if (time < process.arrival_time){
            prev_time = time;
            time += 1;
            noProcessExecuted = 1;
        } else {
            if (process.required_time > quant){
                time += quant;
                process.required_time -= quant;
                process.arrival_time = time - 1;

                for (int i = 1; i < *processesNum; i++){
                    (*array)[i - 1] = (*array)[i];

                    if ((*array)[i].arrival_time > process.arrival_time){
                        move_to_end = 0;
                        (*array)[i - 1] = process;
                        break;
                    }
                }
                if (move_to_end){
                    (*array)[*processesNum - 1] = process;
                }
            } else {
                if (process.required_time < quant){
                    time += process.required_time;
                } else {
                    time += quant;
                }
                for (int i = 0; i < *processesNum - 1; i++){
                    (*array)[i] = (*array)[i + 1];
                }
                *processesNum -= 1;
            }
            move_to_end = 1;
        }

        for (int i = prev_time; i < time; i++){
            sprintf(&(strarr[0][strlen(strarr[0])]), "  %d  ", i);
            sprintf(&nonExecuted[strlen(nonExecuted)], "    ");
            sprintf(&executed[strlen(executed)], "  * ");
            for (int j = 0; j < count_digits(time - 1); j++){
                sprintf(&nonExecuted[strlen(nonExecuted)], " ");
                sprintf(&executed[strlen(executed)], " ");
            }
        }

        length = strlen(strarr[1]);
        for (int i = 1; i < columnNum; i++){
            if (!noProcessExecuted && i == process.id){
                sprintf(&(strarr[i][length]), "%s", executed);
            } else {
                sprintf(&(strarr[i][length]), "%s", nonExecuted);
            }
        }
        memset(nonExecuted, '\0', strlen(nonExecuted) * sizeof(char));
        memset(executed, '\0', strlen(executed) * sizeof(char));
        noProcessExecuted = 0;
    }
    return 0;
}