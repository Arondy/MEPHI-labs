#ifndef TREE_H
#define TREE_H

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Tree{
    Node *root;
} Tree;

Node *tree_find(Tree *, int);
int tree_insert(Tree *, int);
int tree_delete(Tree *, int);

#endif