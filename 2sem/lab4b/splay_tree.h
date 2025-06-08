#ifndef SPLAY_TREE
#define SPLAY_TREE

typedef struct Node {
    char *key;
    char *str;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

void insert_node(Node **root, char *key, char *str);
void remove_node(Node **root, const char *key);
void in_order_traversal(Node *root);
Node *find_node(Node **root, const char *key);
Node *find_max_different_node(Node *root, const char *key);
void print_tree_as_tree(Node *root, int depth, int indent);
int import_tree_from_file(Node** root, const char* filename);

void delete_tree(Node** root);

#endif