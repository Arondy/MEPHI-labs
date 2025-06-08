#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "splay_tree.h"
#include "tree_time.h"

#define NUM 1000
#define DEL_NUM 50

void remove_node_time(Node** root){
    char *key = calloc(256, sizeof(char));
    srand(time(NULL));
    for (int i = 0; i < DEL_NUM; ++i){
        snprintf(key, 256, "%d", rand());
        remove_node(root, key);
        memset(key, '\0', strlen(key) * sizeof(char));
    }
    free(key);
}

void traverse_tree_in_range_time(Node** root){
    for (int i = 0; i < NUM; ++i){
        in_order_traversal(*root);
    }
}

void find_node_by_key_time(Node** root){
    char *key = calloc(256, sizeof(char));
    srand(time(NULL));
    for (int i = 0; i < NUM; ++i){
        snprintf(key, 256, "%d", rand());
        find_node(root, key);
        memset(key, '\0', strlen(key) * sizeof(char));
    }
    free(key);
}

void find_max_different_node_time(Node** root){
    char *key = calloc(256, sizeof(char));
    srand(time(NULL));
    for (int i = 0; i < NUM; ++i){
        snprintf(key, 256, "%d", rand());
        find_max_different_node(*root, key);
        memset(key, '\0', strlen(key) * sizeof(char));
    }
    free(key);
}

void add_random_nodes(Node** root, unsigned int numOfElemets){
    char *key = calloc(256, sizeof(char));
    char *str = calloc(256, sizeof(char));
    srand(time(NULL));
    for (int i = 0; i < numOfElemets; i++){
        snprintf(key, 256, "%d", rand());
        snprintf(str, 256, "%d", rand());
        insert_node(root, key, str);
        memset(key, '\0', strlen(key) * sizeof(char));
        memset(str, '\0', strlen(str) * sizeof(char));
    }
    free(key);
    free(str);
}

double time_tree(unsigned int numOfElements, unsigned int numOfRuns, function_ptr function){
    Node *root = NULL;
    double time_taken = 0;
    clock_t start, end;
    for (int i = 0; i < numOfRuns; i++){
        add_random_nodes(&root, numOfElements);
        start = clock();
        function(&root);
        end = clock();
        time_taken += ((double)(end - start));
        delete_tree(&root);
        root = NULL;
    }
    time_taken /= numOfRuns;
    return time_taken;
}