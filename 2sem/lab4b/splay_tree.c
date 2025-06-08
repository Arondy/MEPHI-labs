#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "splay_tree.h"

Node *createNode(char *key, char *str){
    Node *node = malloc(sizeof(Node));
    node->key = strdup(key);
    node->str = strdup(str);
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void insert_node(Node **root, char *key, char *str){
    Node *preInsertPlace = NULL;
    Node *insertPlace = *root;

    while (insertPlace != NULL){
        preInsertPlace = insertPlace;

        if (strcmp(insertPlace->key, key) < 0){
            insertPlace = insertPlace->right;
        }
        else if (strcmp(insertPlace->key, key) > 0){
            insertPlace = insertPlace->left;
        }
        else {
            printf("Node with key \"%s\" already exists in the tree\n", key);
            return;
        }
    }

    Node *insertElement = createNode(key, str);

    if (preInsertPlace == NULL){
        *root = insertElement;
    }
    else if (strcmp(preInsertPlace->key, insertElement->key) < 0){
        preInsertPlace->right = insertElement;
        insertElement->parent = preInsertPlace;
    }
    else if (strcmp(preInsertPlace->key, insertElement->key) >= 0){
        preInsertPlace->left = insertElement;
        insertElement->parent = preInsertPlace;
    }

//    splaying(root, insertElement);
}

void transplant(Node **root, Node *localParent, Node *localChild){
    if (localParent->parent == NULL){
        *root = localChild;
    }
    else if (localParent == localParent->parent->left){
        localParent->parent->left = localChild;
    }
    else if (localParent == localParent->parent->right){
        localParent->parent->right = localChild;
    }

    if (localChild != NULL){
        localChild->parent = localParent->parent;
    }
}

void left_rotate(Node **root, Node *localRoot){
    Node *rightChild = localRoot->right;

    localRoot->right = rightChild->left;
    if (rightChild->left != NULL){
        rightChild->left->parent = localRoot;
    }

    transplant(root, localRoot, rightChild);

    rightChild->left = localRoot;
    rightChild->left->parent = rightChild;

    if (localRoot->right != NULL){
        localRoot->right->parent = localRoot;
    }
}

void right_rotate(Node **root, Node *localRoot){
    Node *leftChild = localRoot->left;

    localRoot->left = leftChild->right;
    if (leftChild->right != NULL){
        leftChild->right->parent = localRoot;
    }

    transplant(root, localRoot, leftChild);

    leftChild->right = localRoot;
    leftChild->right->parent = leftChild;

    if (localRoot->left != NULL){
        localRoot->left->parent = localRoot;
    }
}

void splaying(Node **root, Node *chosenNode){
    while (chosenNode != *root && chosenNode && chosenNode->parent){
        if (chosenNode->parent == *root){
            if (chosenNode == chosenNode->parent->left){
                right_rotate(root, chosenNode->parent);
            } else if (chosenNode == chosenNode->parent->right){
                left_rotate(root, chosenNode->parent);
            }
            *root = chosenNode;
        } else {
            // Zig-Zig step.
            if (chosenNode == chosenNode->parent->left &&
                chosenNode->parent == chosenNode->parent->parent->left){

                right_rotate(root, chosenNode->parent->parent);
                right_rotate(root, chosenNode->parent);

            } else if (chosenNode == chosenNode->parent->right &&
                       chosenNode->parent == chosenNode->parent->parent->right){

                left_rotate(root, chosenNode->parent->parent);
                left_rotate(root, chosenNode->parent);
            }
                // Zig-Zag step.
            else if (chosenNode == chosenNode->parent->right &&
                     chosenNode->parent == chosenNode->parent->parent->left){

                left_rotate(root, chosenNode->parent);
                right_rotate(root, chosenNode->parent);

            } else if (chosenNode == chosenNode->parent->left &&
                       chosenNode->parent == chosenNode->parent->parent->right){

                right_rotate(root, chosenNode->parent);
                left_rotate(root, chosenNode->parent);
            }
        }
    }
}

void print_tree_as_tree(Node *root, int depth, int indent){
    if (root == NULL){
        return;
    }
    print_tree_as_tree(root->right, depth + 1, indent + 1);
    for (int i = 0; i < depth; i++){
        printf("     ");
    }

    printf("--%s", root->key);
    printf("\n");
    print_tree_as_tree(root->left, depth + 1, indent);
}

Node *find_min_node(Node *localRoot){
    Node *minimum = localRoot;

    while (minimum->left != NULL){
        minimum = minimum->left;
    }

    return minimum;
}

Node *find_max_node(Node *localRoot){
    Node *maximum = localRoot;

    while (maximum->right != NULL){
        maximum = maximum->right;
    }

    return maximum;
}

Node *find_node_without_splaying(Node **root, const char *key){
    Node *searchedElement = *root;

    while (searchedElement != NULL){
        if (strcmp(searchedElement->key, key) < 0){
            searchedElement = searchedElement->right;
        }
        else if (strcmp(key, searchedElement->key) < 0){
            searchedElement = searchedElement->left;
        }
        else if (strcmp(searchedElement->key, key) == 0){
            return searchedElement;
        }
    }
    return NULL;
}

Node *find_node(Node **root, const char *key){
    Node *searchedElement = *root;

    while (searchedElement != NULL){
        if (strcmp(searchedElement->key, key) < 0){
            searchedElement = searchedElement->right;
        }
        else if (strcmp(key, searchedElement->key) < 0){
            searchedElement = searchedElement->left;
        }
        else if (strcmp(searchedElement->key, key) == 0){
            splaying(root, searchedElement);
            return searchedElement;
        }
    }
    return NULL;
}

int string_difference(const char *str1, const char *str2){
    int sum1 = 0, sum2 = 0;

    for (int i = 0; i < strlen(str1); i++){
        sum1 += str1[i];
    }
    for (int i = 0; i < strlen(str2); i++){
        sum2 += str2[i];
    }
    return abs(sum1 - sum2);
}

Node *find_max_different_node(Node *root, const char *key){
    Node *minNode = find_min_node(root);
    Node *maxNode = find_max_node(root);

    int cmpMin = string_difference(key, minNode->key);
    int cmpMax = string_difference(key, maxNode->key);
    Node *result = (cmpMin > cmpMax) ? minNode : maxNode;
    printf("%s\n", result->key);
    return result;
}

Node* split(Node* node, const char* key) {
    if (node == NULL) {
        return NULL;
    }
    if (strcmp(node->key, key) <= 0) {
        Node* right = split(node->right, key);
        node->right = right;
        if (right != NULL) {
            right->parent = node;
        }
        return node;
    } else {
        Node* left = split(node->left, key);
        node->left = left;
        if (left != NULL) {
            left->parent = node;
        }
        return left;
    }
}

Node* merge(Node* left, Node* right) {
    if (left == NULL) {
        return right;
    }
    if (right == NULL) {
        return left;
    }
    if (strcmp(left->key, right->key) > 0) {
        left->right = merge(left->right, right);
        if (left->right != NULL) {
            left->right->parent = left;
        }
        return left;
    } else {
        right->left = merge(left, right->left);
        if (right->left != NULL) {
            right->left->parent = right;
        }
        return right;
    }
}

void remove_node(Node **root, const char *key){
    find_node(root, key);
    if ((*root) == NULL) {
        return;
    }
    if ((*root)->left){
        (*root)->left->parent = NULL;
    }
    if ((*root)->right){
        (*root)->right->parent = NULL;
    }
    Node* new_root = merge((*root)->left, (*root)->right);
    free((*root)->key);
    free((*root)->str);
    free(*root);
    *root = new_root;
}

void remove_node_old_and_stupid(Node **root, const char *key){
//    Node *removeElement = find_node(root, key);
    Node *removeElement = find_node_without_splaying(root, key);

    if (removeElement != NULL){
        if (removeElement->right == NULL){
            transplant(root, removeElement, removeElement->left);
        }
        else if (removeElement->left == NULL){
            transplant(root, removeElement, removeElement->right);
        }
        else {
            Node *newLocalRoot = find_min_node(removeElement->right);

            if (newLocalRoot->parent != removeElement){
                transplant(root, newLocalRoot, newLocalRoot->right);
                newLocalRoot->right = removeElement->right;
                newLocalRoot->right->parent = newLocalRoot;
            }

            transplant(root, removeElement, newLocalRoot);
            newLocalRoot->left = removeElement->left;
            newLocalRoot->left->parent = newLocalRoot;
//            splaying(root, newLocalRoot);
        }
        free(removeElement->key);
        free(removeElement->str);
        free(removeElement);
    }
}

void in_order_traversal(Node *root){
    if (root != NULL){
        printf("%s ", root->key);
        in_order_traversal(root->left);
        in_order_traversal(root->right);
    }
}

int import_tree_from_file(Node **root, const char *filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("Error opening file '%s'!\n", filename);
        return -1;
    }

    char key[256] = {0}, str[256] = {0};
    while (fscanf(file, "%s\n%s", key, str) == 2){
        insert_node(root, key, str);
    }

    fclose(file);
    return 0;
}

void delete_tree(Node **root){
    if (root){
        if (*root){
            delete_tree(&((*root)->left));
            delete_tree(&((*root)->right));
            free((*root)->str);
            free((*root)->key);
            free(*root);
            *root = NULL;
        }
    }
}