#ifndef TABLE
#define TABLE

#include <stdio.h>

typedef struct Table {
    struct KeySpace *ks;
    int ksLen;
    int maxSize;
    FILE *file;
} Table;

typedef struct KeySpace {
    int busy;
    char *key;
    char *par;
    struct Item *item;
} KeySpace;

typedef struct Item {
    int offset;
    int len;
} Item;

int create_table(Table *table, int ks_len, int maxSize, char *filename);
int import_table(Table *table);
int write_to_file_head(Table *table);



int add_item(Table *table, char *key, char *par, char *info);
int delete_item(Table *table, char *key);
int delete_item_with_par(Table *table, char *key);
int print_item_by_key(const Table *table, char *key);
void print_table(const Table *table);
void clear_table(Table *table);
void save_table(Table *table, char *filename);
void free_item(Table *table, int index);
void free_table(Table* table);

#endif