#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"


Node* create_node(char* key, unsigned int value){
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL){
        printf("Unable to allocate memory for new node!\n");
        return NULL;
    }
    new_node->key = malloc(strlen(key) + 1);
    strcpy(new_node->key, key);
    new_node->item = malloc(sizeof(Item));
    new_node->item->value = malloc(sizeof(unsigned int));
    new_node->item->value[0] = value;
    new_node->item->len = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

int insert_node(Node** root, char* key, unsigned int value){
    if (*root == NULL){
        *root = create_node(key, value);
        if (*root == NULL){
            return -1;
        }
    } else {
        int cmp = strcmp(key, (*root)->key);
        if (cmp == 0){
            Node* current = *root;
            current->item->len += 1;
            current->item->value = realloc(current->item->value, current->item->len * sizeof(unsigned int));
            current->item->value[current->item->len - 1] = value;
        } else if (cmp < 0){
            insert_node(&((*root)->left), key, value);
        } else {
            insert_node(&((*root)->right), key, value);
        }
    }
    return 0;
}

void print_tree(Node* root){
    if (root != NULL){
        printf("%s - ", root->key);
        for (int i = 0; i < root->item->len; i++){
            printf("%u ", root->item->value[i]);
        }
        printf("\n");

        print_tree(root->left);
        print_tree(root->right);
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

    printf("--%s:%d", root->key, root->item->value[0]);
    if (root->item->len > 1){
        printf("(%d)", root->item->len);
    }
    printf("\n");
    print_tree_as_tree(root->left, depth + 1, indent);
}

Node* find_node_by_key(Node* root, const char* key, int index){
    if (root == NULL){
        printf("No such key found!\n");
        return NULL;
    }
    int cmp = strcmp(key, root->key);
    if (cmp < 0){
        return find_node_by_key(root->left, key, index);
    } else if (cmp > 0){
        return find_node_by_key(root->right, key, index);
    } else {
        if (index >= root->item->len){
            printf("Wrong index!");
            return NULL;
        } else {
            printf("%s - %u\n", key, root->item->value[index]);
            return root;
        }
    }
}

Node* find_max_node(Node* root){
    if (root == NULL){
        return NULL;
    }
    Node* current = root;
    while (current->right != NULL){
        current = current->right;
    }
    printf("%s - %u\n", current->key, current->item->value[0]);
    return current;
}

Node* find_min_node(Node* root){
    if (root == NULL){
        return NULL;
    }
    Node *current = root;
    while (current->left != NULL){
        current = current->left;
    }
    return current;
}

int shift_array(Item* item, int index) {
    for (int i = index; i < item->len - 1; i++) {
        item->value[i] = item->value[i + 1];
    }
    item->len--;
    item->value = realloc(item->value, item->len * sizeof(unsigned int));
    return 0;
}

Node* remove_node(Node** root, const char* key, int index) {
    Node* temp = NULL;
    if ((*root) == NULL) {
        return NULL;
    }
    int cmp = strcmp(key, (*root)->key);
    if (cmp < 0) {
        temp = remove_node(&((*root)->left), key, index);
        (*root)->left = temp;
    } else if (cmp > 0) {
        temp = remove_node(&((*root)->right), key, index);
        (*root)->right = temp;
    } else {
        if (index > (*root)->item->len) {
            printf("Wrong index!\n");
            return (*root);
        }
        if ((*root)->item->len == 1 || index == 0) {
            if ((*root)->right && (*root)->left) {
                temp = find_min_node((*root)->right);
                (*root)->key = realloc((*root)->key, strlen(temp->key) + 1);
                strcpy((*root)->key, temp->key);
                (*root)->item->len = temp->item->len;
                free((*root)->item->value);
                (*root)->item->value = malloc((*root)->item->len * sizeof(unsigned int));
                memcpy((*root)->item->value, temp->item->value, (*root)->item->len * sizeof(unsigned int));
                (*root)->right = remove_node(&((*root)->right), (*root)->key, 0);
            } else {
                temp = (*root);
                if ((*root)->left == NULL) {
                    (*root) = (*root)->right;
                } else if ((*root)->right == NULL) {
                    (*root) = (*root)->left;
                }
                free(temp->item->value);
                free(temp->item);
                free(temp->key);
                free(temp);
            }
        } else {
            shift_array((*root)->item, index - 1);
        }
    }
    return (*root);
}

void traverse_tree_in_range(Node* root, const char* key1, const char* key2){
    if (root == NULL){
        return;
    }
    if (strcmp(root->key, key1) < 0){
        traverse_tree_in_range(root->right, key1, key2);
    } else if (strcmp(root->key, key2) > 0){
        traverse_tree_in_range(root->left, key1, key2);
    } else {
        traverse_tree_in_range(root->left, key1, key2);
        traverse_tree_in_range(root->right, key1, key2);
        printf("%s ", root->key);
    }
}

int import_tree_from_file(Node** root, const char* filename){
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        printf("Error opening file '%s'!\n", filename);
        return -1;
    }

    char key[256] = {0};
    unsigned int value = 0;
    while (fscanf(file, "%s\n%u", key, &value) == 2){
        insert_node(root, key, value);
    }

    fclose(file);
    return 0;
}

int delete_tree(Node** root){
    if (root){
        if (*root){
            delete_tree(&(*root)->left);
            delete_tree(&(*root)->right);
            free((*root)->item->value);
            free((*root)->item);
            free((*root)->key);
            free(*root);
            *root = NULL;
        }
    }
    return 0;
}

