#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"


Node* traverse(Node* node){
    static Node* first = NULL;
    static Node* prevNode = NULL;

    if (node == NULL){
        return NULL;
    }

    Node *right = node->right, *left = node->left;

    if (first == NULL){
        first = node;
    }

    node->left = prevNode;
    if (prevNode != NULL){
        prevNode->right = node;
    }
    prevNode = node;

    traverse(left);
    traverse(right);

    return first;
}

int main(){
    Tree tree;
    tree.root = NULL;
    tree_insert(&tree, 50);
    tree_insert(&tree, 20);
    tree_insert(&tree, 10);
    tree_insert(&tree, 70);
    tree_insert(&tree, 65);
    tree_insert(&tree, 80);
    tree_insert(&tree, 75);
    Node* node = traverse(tree.root);
    return 0;
}