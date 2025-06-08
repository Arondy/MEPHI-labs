#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"

int find_key(Table *table, char *key) {
    for (int i = 0; i < table->mSize; i++) {
        if (table->ks[i].busy && strcmp(table->ks[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

int find_par(Table *table, char **par) {
    if ((*par)[0]){
        int parExists = 0;
        for (int i = 0; i < table->mSize; i++){
            if (table->ks[i].busy && strcmp(table->ks[i].key, *par) == 0){
                parExists = 1;
                break;
            }
        }
        if (parExists == 0){
            printf("This par is not in the table!\n");
            return -1;
        }
    } else {
        *par = NULL;
    }
    return 0;
}

int add_item(Table *table, char *key, char *par, char *info) {
    if (find_key(table, key) != -1){
        printf("This key is already in the table!");
        return -1;
    }
    if (table->mSize == table->maxSize){
        printf("Max size is reached. Try to reorganize table or delete some elements before adding one more.\n");
        return -1;
    }

    if (find_par(table, &par) == -1){
        return -1;
    }

    int index = table->mSize;
    table->ks[index].busy = 1;
    table->ks[index].key = strdup(key);
    if (par){
        table->ks[index].par = strdup(par);
    }  else {
        table->ks[index].par = NULL;
    }
    table->ks[index].info = malloc(sizeof(Item));
    table->ks[index].info->info = strdup(info);
    table->mSize++;
    table->cSize++;

    return 0;
}

int delete_item(Table *table, char *key) {
    int index = find_key(table, key);
    if (index == -1) {
        printf("No elements with \"%s\" key\n", key);
        return -1;
    }
    table->ks[index].busy = 0;
    table->cSize--;
    for (int i = 0; i < table->mSize; i++) {
        if (table->ks[i].busy && table->ks[i].par && strcmp(table->ks[i].par, key) == 0) {
            free(table->ks[i].par); // ?
            table->ks[i].par = NULL;
        }
    }
    return 0;
}

int delete_item_with_par(Table *table, char *key) {
    int index = find_key(table, key);
    if (index == -1) {
        printf("No elements with \"%s\" key\n", key);
        return -1;
    }
    table->ks[index].busy = 0;
    table->cSize--;
    for (int i = 0; i < table->mSize; i++) {
        if (table->ks[index].par && strcmp(table->ks[index].par, table->ks[i].key) == 0) {
            table->ks[i].busy = 0;
            table->cSize--;
            free(table->ks[i].par); // ?
            table->ks[i].par = NULL;
        }
    }
    return 0;
}

int print_item_by_key(Table *table, char *key) {
    int index = find_key(table, key);
    if (index == -1) {
        printf("No elements with \"%s\" key\n", key);
        return -1;
    }
    printf("%s\n", table->ks[index].info->info);
    for (int i = 0; i < table->mSize; i++) {
        if (table->ks[i].busy && table->ks[i].par && strcmp(table->ks[i].par, key) == 0){
            printf("%s (par)\n", table->ks[i].info->info);
        }
    }
    return 0;
}

void print_table(Table *table) {
    printf("Printing table:\n");
    printf("%-4s | %-4s | %-10s | %-10s | %s\n", "ind", "busy", "key", "par", "info");

    for (int i = 0; i < table->mSize; i++) {
        if (table->ks[i].busy){
            printf("%-4d | %-4d | %-10s | %-10s | %s\n", i, table->ks[i].busy, table->ks[i].key, table->ks[i].par, table->ks[i].info->info);
        } else {
            printf("%-4d | 0    | ...\n", i);
        }
    }
}

void clear_table(Table *table) {
    int first_free = 0;
    for (int i = table->mSize - 1; i >= 0; i--) {
        if (table->ks[i].busy) {
            // Если элемент на своем месте, то продолжаем поиск следующего элемента
            if (i == first_free) {
                first_free++;
            }
                // Иначе ищем первый свободный элемент и перемещаем элемент туда
            else {
                while (table->ks[first_free].busy) {
                    first_free++;
                    if (first_free >= table->mSize){
                        break;
                    }
                }
                if (i < first_free){
                    break;
                }
                free_item(table, first_free);
                table->ks[first_free] = table->ks[i];
                table->ks[i].busy = 0;
                first_free++;
                table->mSize--;
            }
        }
    }

//    table->mSize = table->cSize;
}

void free_item(Table *table, int i) {
    if (table->ks[i].key) {
        free(table->ks[i].key);
        if (table->ks[i].par){
            free(table->ks[i].par);
        }
        if (table->ks[i].info){
            free(table->ks[i].info->info);
            free(table->ks[i].info);
        }
    }
}

void free_table(Table* table) {
    for (int i = 0; i < table->mSize; i++) {
        free_item(table, i);
    }
    free(table->ks);
}

int read_table_from_file(Table *table, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return -1;
    }

    char line[100];
    if (!fgets(line, 100, file)) {
        printf("Error reading maxSize from file\n");
        fclose(file);
        return -1;
    }
    int maxSize = strtol(line, NULL, 10);
    if (maxSize <= 0) {
        printf("Invalid maxSize value: %d\n", maxSize);
        fclose(file);
        return -1;
    }

    table->ks = realloc(table->ks, maxSize * sizeof(KeySpace));
    table->mSize = 0;
    table->cSize = 0;
    table->maxSize = maxSize;

    int busy, mSize = 0, cSize = 0;
    char key[100], par[100], info[100];
    while (fgets(line, 100, file)) {
        sscanf(line, "%d %s %s %s", &busy, key, par, info);
        if (mSize >= maxSize) {
            printf("Table overflow, reading stopped!\n");
            fclose(file);
            return -1;
        }
        KeySpace *ks = &(table->ks[mSize]);
        ks->busy = busy;
        ks->key = strdup(key);

        if (strcmp(par, "_") == 0) {
            ks->par = NULL;
        } else {
            ks->par = strdup(par);
        }

        if (busy) {
            cSize++;
            ks->info = malloc(sizeof(Item));
            ks->info->info = strdup(info);
        } else {
            ks->info = NULL;
        }
        mSize++;
    }

    fclose(file);
    table->mSize = mSize;
    table->cSize = cSize;

    return 0;
}