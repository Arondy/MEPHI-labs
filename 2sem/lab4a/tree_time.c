#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"
#include "tree_time.h"

#define NUM 1000
#define DEL_NUM 50

void remove_node_time(Node* root){
    char key[256] = {0};
    srand(time(NULL));
    for (int i = 0; i < DEL_NUM; ++i){
        snprintf(key, 256, "%d", rand());
        remove_node(&root, key, 0);
        memset(key, '\0', sizeof(key));
    }
}

void traverse_tree_in_range_time(Node* root){
    for (int i = 0; i < NUM; ++i){
        traverse_tree_in_range(root, "1", "99999999999999");
    }
}

void find_node_by_key_time(Node* root){
    char key[256] = {0};
    srand(time(NULL));
    for (int i = 0; i < NUM; ++i){
        snprintf(key, 256, "%d", rand());
        find_node_by_key(root, key, 0);
        memset(key, '\0', sizeof(key));
    }
}

void find_max_node_time(Node* root){
    find_max_node(root);
}

void addRandomNodes(Node** root, unsigned int numOfElemets){
    char key[256] = {0};
    unsigned int value = 0;
    srand(time(NULL));
    for (int i = 0; i < numOfElemets; i++){
        snprintf(key, 256, "%d", rand());
        value = rand() % 1000;
        insert_node(root, key, value);
    }
}

double time_tree(unsigned int numOfElemets, unsigned int numOfRuns, function_ptr function){
    Node *root = NULL;
    double time_taken = 0;
    clock_t start, end;
    for (int i = 0; i < numOfRuns; i++){
        addRandomNodes(&root, numOfElemets);
        start = clock();
        function(root);
        end = clock();
        time_taken += ((double)(end - start));
        delete_tree(&root);
        root = NULL;
    }
    time_taken /= numOfRuns;
    return time_taken;
}