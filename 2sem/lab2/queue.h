#ifndef VECTOR
#define VECTOR

typedef struct Process {
    int id;
    int arrival_time;
    int required_time;
} Process;

typedef struct QueueNode {
    Process* process;
    struct QueueNode* next;
    struct QueueNode* prev;
} QueueNode;

typedef struct Queue {
    QueueNode* first;
    QueueNode* last;
} Queue;

typedef union Pointer{
    Queue* queue_ptr;
    Process* process_ptr;
} Pointer;

void print_processes(Pointer* ptr, int processesNum);
void bubble_sort(Pointer *ptr, int len);
int read_process_input(Pointer* ptr, int* processesNum, int* quant);
int processing(Pointer* ptr, int *processesNum, int quant, char** strarr, int columnNum);

#endif