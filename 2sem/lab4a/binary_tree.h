#ifndef BIN_TREE
#define BIN_TREE


typedef struct Item {
    unsigned int *value;
    unsigned int len;
} Item;

typedef struct Node {
    char *key;
    Item *item;
    struct Node* left;
    struct Node* right;
} Node;

Node* create_node(char* key, unsigned int value);
int insert_node(Node** root, char* key, unsigned int value);
Node* remove_node(Node** root, const char* key, int index);
void traverse_tree_in_range(Node* root, const char* key1, const char* key2);
Node* find_node_by_key(Node* root, const char* key, int index);
Node* find_max_node(Node* root);
void print_tree_as_tree(Node *root, int depth, int indent);
void print_tree(Node* root);
int import_tree_from_file(Node** root, const char* filename);
int delete_tree(Node** root);

#endif