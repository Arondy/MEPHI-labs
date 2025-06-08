#ifndef QUEUE
#define QUEUE
#include "graph.h"

typedef struct QueueNode {
    Vertex *vertex;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

Queue *create_queue();
void enqueue(Queue *queue, Vertex *vertex);
Vertex *dequeue(Queue *queue);
int is_queue_empty(Queue *queue);
void destroy_queue(Queue *queue);

#endif