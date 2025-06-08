#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "input.h"

void print_processes(Pointer* ptr, int _){
    Queue *queue = ptr->queue_ptr;
    QueueNode* current_node = queue->first;
    while (current_node){
        Process* current_process = current_node->process;
        printf("%d/%d/%d\n", current_process->id, current_process->arrival_time, current_process->required_time);
        current_node = current_node->next;
    }
    printf("\n");
}

void list_remove_node(Queue* queue, QueueNode* node){
    if (node->prev == NULL) {
        queue->first = node->next;
    } else {
        node->prev->next = node->next;
    }
    if (node->next == NULL) {
        queue->last = node->prev;
    } else {
        node->next->prev = node->prev;
    }
    free(node);
}

void list_add_to_end(Queue* queue, Process* process){
    QueueNode* node = malloc(sizeof(QueueNode));
    node->process = process;
    node->next = NULL;
    node->prev = queue->last;

    if (queue->last) {
        queue->last->next = node;
    } else {
        queue->first = node;
    }

    queue->last = node;
}

void list_move_before(Queue* queue, QueueNode* before, QueueNode* after){
    if (before == NULL || after == NULL || before == after) {
        return;
    }
    if (before->prev == NULL) {
        queue->first = before->next;
    } else {
        before->prev->next = before->next;
    }
    if (before->next == NULL) {
        queue->last = before->prev;
    } else {
        before->next->prev = before->prev;
    }
    if (after->prev == NULL) {
        queue->first = before;
    } else {
        after->prev->next = before;
    }
    before->prev = after->prev;
    after->prev = before;
    before->next = after;
}

int read_process_input(Pointer* ptr, int *processesNum, int* quant){
    char process_string[255];
    char* token = NULL;
    int id = 0, arrival_time = 0, required_time = 0, last_id = 0, check = 0;
    Process* process = NULL;

    Queue **queue = &(ptr->queue_ptr);

    *queue = malloc(sizeof(Queue));
    (*queue)->first = NULL;
    (*queue)->last = NULL;

    printf("Using list\n");
    printf("Input quant, then id/tc/te:\n");
    if (positive_input(quant) == -1){
        return -1;
    }

    fgets(process_string, sizeof(process_string), stdin);
    process_string[strlen(process_string)] = '\0';

    token = strtok(process_string, " ");

    while (token){
        if (read_input3(token, &id, &arrival_time, &required_time) == -1){
            return -1;
        }

        if (last_id < id){
            last_id = id;
        }

        process = malloc(sizeof(Process));
        process->id = id;
        process->arrival_time = arrival_time;
        process->required_time = required_time;

        list_add_to_end((*queue), process);
        *processesNum += 1;
        token = strtok(NULL, " ");
    }
    return last_id;
}

int comparator(QueueNode* node_a, QueueNode* node_b){
    int aTime = node_a->process->arrival_time;
    int bTime = node_b->process->arrival_time;
    if (aTime != bTime){
        return aTime - bTime;
    } else {
        return node_a->process->id - node_b->process->id;
    }
}

void swap_nodes(QueueNode* a, QueueNode* b){
    Process* temp_process = a->process;
    a->process = b->process;
    b->process = temp_process;
}

void bubble_sort(Pointer* ptr, int _){
    Queue **queue = &(ptr->queue_ptr);
    if ((*queue)->first == NULL || (*queue)->first->next == NULL) {
        return;
    }

    int swapped = 0;
    QueueNode *left = NULL, *right = NULL;

    do {
        swapped = 0;
        left = (*queue)->first;

        while (left->next != right) {
            if (comparator(left, left->next) > 0) {
                swap_nodes(left, left->next);
                swapped = 1;
            }
            left = left->next;
        }
        right = left;
    } while (swapped);
}

int processing(Pointer* ptr, int* _, int quant, char **strarr, int columnNum){
    Queue **queue = &(ptr->queue_ptr);
    QueueNode *node = (*queue)->first;
    int time = 0, prev_time = 0, move_to_end = 1, id = 0, arr_time = 0, req_time = 0, length = 0, noProcessExecuted = 0;
    char nonExecuted[200] = {0}, executed[200] = {0};

    while (node){
        Process *process = node->process;
        id = process->id;
        prev_time = time;

        if (time < process->arrival_time){
            prev_time = time;
            time += 1;
            noProcessExecuted = 1;
        } else {
            if (process->required_time > quant){
                time += quant;
                process->required_time -= quant;
                process->arrival_time = time - 1;

                QueueNode *next_node = node->next;
                while (next_node){
                    if (next_node->process->arrival_time > process->arrival_time) {
                        move_to_end = 0;
                        break;
                    }
                    next_node = next_node->next;
                }

                if (move_to_end){
                    list_remove_node((*queue), node);
                    list_add_to_end((*queue), process);
                } else {
                    list_move_before((*queue), node, next_node);
                }
            } else {
                time += process->required_time;
                free(node->process);
                list_remove_node((*queue), node);
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
            if (!noProcessExecuted && i == id){
                sprintf(&(strarr[i][length]), "%s", executed);
            } else {
                sprintf(&(strarr[i][length]), "%s", nonExecuted);
            }
        }
        memset(nonExecuted, '\0', strlen(nonExecuted) * sizeof(char));
        memset(executed, '\0', strlen(executed) * sizeof(char));
        noProcessExecuted = 0;
        node = (*queue)->first;
    }
    return 0;
}
