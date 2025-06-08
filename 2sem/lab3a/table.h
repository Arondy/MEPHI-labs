#ifndef TABLE
#define TABLE

typedef struct Table {
    struct KeySpace *ks;
    int mSize;
    int cSize;
    int maxSize;
} Table;

typedef struct KeySpace {
    int busy;
    char *key;
    char *par;
    struct Item *info;
} KeySpace;

typedef struct Item {
    char *info;
    // Add other fields for working with file
} Item;

int add_item(Table *table, char *key, char *par, char *info);
int delete_item(Table *table, char *key);
int delete_item_with_par(Table *table, char *key);
int print_item_by_key(Table *table, char *key);
void print_table(Table *table);
void clear_table(Table *table);
void free_table(Table* table);
void free_item(Table *table, int i);
int read_table_from_file(Table *table, const char *filename);

#endif