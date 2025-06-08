#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hashtable.h"

// Равномерное распределение по всем числовым значениям
// Avalanche effect (среднее 16) (зависит от константы (наша - 0x45d9f3b))
// 2^(N/2) - collision rate

uint64_t murmurHash3(const unsigned int *key){
    uint64_t hash = *key;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = (hash >> 16) ^ hash;
    return hash;
}

uint64_t RJhash(const unsigned int *key){
    uint64_t hash = *key;
    hash = (hash + 0x7ed55d16) + (hash << 12);
    hash = (hash ^ 0xc761c23c) ^ (hash >> 19);
    hash = (hash + 0x165667b1) + (hash << 5);
    hash = (hash + 0xd3a2646c) ^ (hash << 9);
    hash = (hash + 0xfd7046c5) + (hash << 3);
    hash = (hash ^ 0xb55a4f09) ^ (hash >> 16);
    return hash;
}

unsigned int combined_hashfunc(int i, const unsigned int *key){
    return (murmurHash3(key) + i * RJhash(key)) % MAXSIZE;
}

int find_key(KeySpace *ks, const unsigned int *key){
    unsigned int index = 0;
    for (int i = 0; i < MAXSIZE; i++){
        index = combined_hashfunc(i, key);
        if (ks[index].busy == 0){
            return -1;
        } else if (ks[index].busy && *(ks[index].key) == *key){
            return index;
        }
    }
    return -1;
}

int find_first_free(KeySpace *ks, const unsigned int *key){
    unsigned int index = 0;
    for (int i = 0; i < MAXSIZE; i++){
        index = combined_hashfunc(i, key);
        if (ks[index].busy == 0){
            return index;
        }
    }
    return -1;
}

int add_item(KeySpace *ks, unsigned int *key, unsigned int data){
    int index = find_key(ks, key);
    if (index >= 0){
        printf("This key is already in the table!\n");
        free(key);
        return -1;
    }

    index = find_first_free(ks, key);
    if (index == -1){
        printf("Can't find non-ocupied place!\n");
        free(key);
        return -1;
    }

    ks[index].busy = 1;
    ks[index].key = key;
    ks[index].data = data;

    return 0;
}

int delete_item(KeySpace *ks, const unsigned int *key){
    int index = find_key(ks, key);
    if (index == -1){
        printf("There is no such key!\n");
        return -1;
    }

    ks[index].busy = 0;
    free(ks[index].key);
    ks[index].key = NULL;

    return 0;
}

void print_item_by_key(KeySpace *ks, const unsigned int *key){
    int index = find_key(ks, key);
    if (index == -1){
        printf("There is no such key!\n");
        return;
    }

    printf("%d\n", (ks[index].data));
}

int read_table_from_file(KeySpace *ks, const char *filename){
    char buffer[100] = {0};
    unsigned int data = 0;
    int count = 0;
    FILE *file = fopen(filename, "r");

    if (!file) {
        printf("Error opening file %s\n", filename);
        return -1;
    }

    while (fgets(buffer, 100, file)){
        if (count > MAXSIZE){
            printf("Table overflow, reading stopped!\n");
            fclose(file);
            return -1;
        }
        unsigned int *key = malloc(sizeof(unsigned int));

        sscanf(buffer, "%u %u", key, &data);
        add_item(ks, key, data);
        memset(buffer, '\0', strlen(buffer) * sizeof(char));
        count++;
    }

    fclose(file);
    return 0;
}

void print_table(KeySpace *ks){
    printf("%-4s | %-4s | %-10s | %-10s\n\n", "ind", "busy", "key", "data");
    for (int i = 0; i < MAXSIZE; i++){
        if (ks[i].busy){
            printf("%-4d | %-4d | %-6d | %d\n", i, ks[i].busy, *(ks[i].key), ks[i].data);
        } else {
            printf("%-4d | %-4d | %-6s | %s\n", i, 0, "null", "-");
        }
    }
}

void reorganize_table(KeySpace *ks){
    KeySpace *new_ks = malloc(MAXSIZE * sizeof(KeySpace));
    for (int i = 0; i < MAXSIZE; i++){
        new_ks[i].busy = 0;
        new_ks[i].key = NULL;
        new_ks[i].data = 0;
    }

    for (int i = 0; i < MAXSIZE; i++){
        if (ks[i].busy){
            int ideal_index = find_first_free(new_ks, ks[i].key);
            if (ideal_index == -1){
                printf("Can't find non-ocupied place!\n");
                continue;
            }
            new_ks[ideal_index].busy = 1;
            new_ks[ideal_index].key = malloc(sizeof(unsigned int));
            *(new_ks[ideal_index].key) = *(ks[i].key);
            new_ks[ideal_index].data = ks[i].data;
        }
    }

    for (int i = 0; i < MAXSIZE; i++){
        if (ks[i].key){
            free(ks[i].key);
        }
        ks[i].busy = new_ks[i].busy;
        ks[i].key = new_ks[i].key;
        ks[i].data = new_ks[i].data;
    }

    free(new_ks);
}

void free_table(KeySpace *ks){
    for (int i = 0; i < MAXSIZE; i++){
        if (ks[i].key){
            free(ks[i].key);
        }
    }
    free(ks);
}