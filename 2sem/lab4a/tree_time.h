#ifndef TREE_TIME
#define TREE_TIME
#include "binary_tree.h"

typedef void (*function_ptr)(Node*);

double time_tree(unsigned int numOfElemets, unsigned int numOfRuns, function_ptr function);
void remove_node_time(Node* root);
void traverse_tree_in_range_time(Node* root);
void find_node_by_key_time(Node* root);
void find_max_node_time(Node* root);

#endif