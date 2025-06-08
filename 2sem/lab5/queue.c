#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue *create_queue(){
    Queue *queue = malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

void enqueue(Queue *queue, Vertex *vertex){
    QueueNode *new_node = (QueueNode *) malloc(sizeof(QueueNode));
    new_node->vertex = vertex;
    new_node->next = NULL;

    if (queue->rear == NULL){
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

Vertex *dequeue(Queue *queue){
    if (queue->front == NULL){
        return NULL;
    }

    QueueNode *temp_node = queue->front;
    Vertex *vertex = temp_node->vertex;

    if (queue->front == queue->rear){
        queue->front = NULL;
        queue->rear = NULL;
    } else {
        queue->front = queue->front->next;
    }

    free(temp_node);
    return vertex;
}

int is_queue_empty(Queue *queue){
    return queue->front == NULL;
}

void destroy_queue(Queue *queue){
    while (!is_queue_empty(queue)){
        dequeue(queue);
    }
    free(queue);
}
