#ifndef HASHTABLE
#define HASHTABLE

#define MAXSIZE 10

typedef struct KeySpace{
    unsigned int busy;
    unsigned int *key;
    unsigned int data;
} KeySpace;

int add_item(KeySpace *ks, unsigned int *key, unsigned int data);
int delete_item(KeySpace *ks, const unsigned int *key);
void print_item_by_key(KeySpace *ks, const unsigned int *key);
int read_table_from_file(KeySpace *ks, const char *filename);
void print_table(KeySpace *ks);
void reorganize_table(KeySpace *ks);
void free_table(KeySpace *ks);

#endif