#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"

int create_table(Table *table, int ks_len, int maxSize, char *filename){
    table->ksLen = ks_len;
    table->maxSize = maxSize;
    table->ks = malloc(table->maxSize * sizeof(KeySpace));

    table->file = fopen(filename, "rb+");
    if (!table->file){
        table->file = fopen(filename, "wb+");
        if (!table->file){
            return -1;
        }
    }

    return 0;
}

int check_file(FILE *file){
    char title[10] = {0};

    if (file == NULL){
        printf("Error opening file!\n");
        return -1;
    }

    fseek(file, 0, SEEK_END);

    if (ftell(file) < 12){
        printf("File is empty!\n");
        return -1;
    }

    fseek(file, 0, SEEK_SET);
    fread(title, sizeof(char), 4, file);

    if (strcmp(title, "loxx") != 0){
        printf("This file isn't what we need!\n");
        return -1;
    }

    return 0;
}

int import_table(Table *table){
    FILE *file = table->file;

    if (check_file(table->file) == -1){
        return -1;
    }

    fread(&table->maxSize, sizeof(int), 1, file);
    fread(&table->ksLen, sizeof(int), 1, file);

    if (table->ksLen > table->maxSize){
        printf("ksLen can't be bigger than maxSize!\n");
        return -1;
    }

    table->ks = realloc(table->ks, table->maxSize * sizeof(KeySpace));

    int key_len = 0, par_len = 0;
    char key[256] = {0}, par[256] = {0};

    for (int ksCount = 0; ksCount < table->ksLen; ksCount++){
        table->ks[ksCount].busy = 1;
        fread(&key_len, sizeof(int), 1, file);
        fread(key, sizeof(char), key_len, file);
        fread(&par_len, sizeof(int), 1, file);
        fread(par, sizeof(char), par_len, file);

        table->ks[ksCount].item = malloc(sizeof(Item));
        fread(&(table->ks[ksCount].item->len), sizeof(int), 1, file);
        table->ks[ksCount].item->offset = ftell(file);
        fseek(file, table->ks[ksCount].item->len, SEEK_CUR);

        table->ks[ksCount].key = strdup(key);
        if (strlen(par) >  0){
            table->ks[ksCount].par = strdup(par);
        } else {
            table->ks[ksCount].par = NULL;
        }
    }

    return 0;
}

int read_info(FILE *file, int offset, int len, char *buffer){
    fseek(file, offset, SEEK_SET);
    fread(buffer, sizeof(char), len, file);

    return 0;
}



int find_key(const Table *table, char *key){
    for (int i = 0; i < table->ksLen; i++) {
        if (table->ks[i].busy && strcmp(table->ks[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

int find_par(const Table *table, char **par){
    if ((*par)[0]){
        int parExists = 0;
        for (int i = 0; i < table->ksLen; i++){
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

int add_item(Table *table, char *key, char *par, char *info){
    int offset = 0, index = 0;
    int info_len = strlen(info);

    if (table->ksLen == table->maxSize){
        printf("Table is full!");
        return -1;
    }
    if (find_key(table, key) != -1){
        printf("This key is already in the table!");
        return -1;
    }
    if (find_par(table, &par) == -1){
        return -1;
    }

    index = table->ksLen;
    table->ks[index].busy = 1;
    table->ks[index].key = strdup(key);
    if (par && par[0]){
        table->ks[index].par = strdup(par);
    }  else {
        table->ks[index].par = NULL;
    }

    fseek(table->file, 0, SEEK_END);
    fwrite(&info_len, sizeof(int), 1, table->file);
    offset = ftell(table->file);
    fwrite(info, sizeof(char), strlen(info), table->file);
    table->ks[index].item = malloc(sizeof(Item));
    table->ks[index].item->offset = offset;
    table->ks[index].item->len = info_len;
    table->ksLen++;

    return 0;
}

int delete_item(Table *table, char *key){
    int index = find_key(table, key);
    if (index == -1) {
        printf("No elements with \"%s\" key\n", key);
        return -1;
    }

    table->ks[index].busy = 0;
    for (int i = 0; i < table->ksLen; i++){
        if (table->ks[i].busy && table->ks[i].par && strcmp(table->ks[i].par, key) == 0) {
            free(table->ks[i].par); // ?
            table->ks[i].par = NULL;
        }
    }
    return 0;
}

int delete_item_with_par(Table *table, char *key){
    int index = find_key(table, key);
    printf("*%d*\n", index);
    if (index == -1) {
        printf("No elements with \"%s\" key\n", key);
        return -1;
    }
    table->ks[index].busy = 0;
    for (int i = 0; i < table->ksLen; i++){
        printf("**par: %s\nkey: %s**\n", table->ks[index].key, table->ks[i].par);
        if (table->ks[i].busy && table->ks[i].par && strcmp(table->ks[index].key, table->ks[i].par) == 0) {
            delete_item_with_par(table, table->ks[i].key);
            table->ks[i].busy = 0;
            free(table->ks[i].par); // ?
            table->ks[i].par = NULL;
        }
    }
    return 0;
}

int print_item_by_key(const Table *table, char *key){
    char buffer[256] = {0};
    int index = find_key(table, key);
    if (index == -1) {
        printf("No elements with \"%s\" key\n", key);
        return -1;
    }

    read_info(table->file, table->ks[index].item->offset, table->ks[index].item->len, buffer);
    printf("%s\n", buffer);
    for (int i = 0; i < table->ksLen; i++) {
        if (table->ks[i].busy && table->ks[i].par && strcmp(table->ks[i].par, key) == 0){
            memset(buffer, '\0', strlen(buffer) * sizeof(char));
            read_info(table->file, table->ks[i].item->offset, table->ks[i].item->len, buffer);
            printf("%s (par)\n", buffer);
        }
    }
    return 0;
}

void clear_table(Table *table){
    int first_free = 0;
    for (int i = table->ksLen - 1; i >= 0; i--) {
        if (table->ks[i].busy) {
            // Если элемент на своем месте, то продолжаем поиск следующего элемента
            if (i == first_free) {
                first_free++;
            }
                // Иначе ищем первый свободный элемент и перемещаем элемент туда
            else {
                while (table->ks[first_free].busy) {
                    first_free++;
                    if (first_free >= table->ksLen){
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
                table->ksLen--;
            }
        }
    }
    for (int i = table->ksLen - 1; i >= 0; i--) {
        if (!table->ks[i].busy) {
            free_item(table, i);
            table->ksLen--;
        }
    }
}

void save_table(Table *table, char *filename){
    FILE *newFile = fopen("tempfile.bin", "wb+");
    if (!newFile){
        printf("Couldn't create temp file!\n");
        return;
    }
    fseek(newFile, 0, SEEK_SET);
    fwrite("loxx", sizeof(char), 4, newFile);
    fwrite(&table->maxSize, sizeof(int), 1, newFile);
    fwrite(&table->ksLen, sizeof(int), 1, newFile);

    int check = 0, key_len = 0, par_len = 0;
    char buffer[256] = {0};

    for (int i = 0; i < table->ksLen; i++){
        if (table->ks[i].busy){
            key_len = strlen(table->ks[i].key);
            if (table->ks[i].par){
                par_len = strlen(table->ks[i].par);
            } else {
                par_len = 0;
            }

            fwrite(&key_len, sizeof(int), 1, newFile);
            fwrite(table->ks[i].key, sizeof(char), key_len, newFile);
            fwrite(&par_len, sizeof(int), 1, newFile);
            fwrite(table->ks[i].par, sizeof(char), par_len, newFile);
            fwrite(&(table->ks[i].item->len), sizeof(int), 1, newFile);
            read_info(table->file, table->ks[i].item->offset, table->ks[i].item->len, buffer);
            fwrite(buffer, sizeof(char), strlen(buffer), newFile);

            memset(buffer, '\0', strlen(buffer) * sizeof(char));
        }
    }
    fclose(table->file);
    fclose(newFile);

    check = remove(filename);
    if (check == -1){
        perror("Error removing main file");
        return;
    }

    check = rename("tempfile.bin", filename);
    if (check == -1){
        perror("Error renaming tempfile");
        return;
    }

    table->file = fopen(filename, "rb+");
}



int write_to_file_head(Table *table){
    fseek(table->file, 0, SEEK_SET);
    fwrite("loxx", sizeof(char), 4, table->file);

    return 0;
}

void print_table(const Table *table){
    FILE *file = table->file;
    char buffer[256] = {0};

    printf("Printing table:\n");
    printf("%-4s | %-4s | %-10s | %-10s | %s\n\n", "ind", "busy", "key", "par", "info");

    for (int i = 0; i < table->ksLen; i++) {
        if (table->ks[i].busy){
            read_info(file, table->ks[i].item->offset, table->ks[i].item->len, buffer);
            printf("%-4d | %-4d | %-10s | %-10s | %s\n", i, table->ks[i].busy, table->ks[i].key, table->ks[i].par, buffer);
            memset(buffer, '\0', strlen(buffer) * sizeof(char));
        } else {
            printf("%-4d | 0    | ...\n", i);
        }
    }
}

void free_item(Table *table, int index){
    if (table->ks[index].key){
        free(table->ks[index].key);
        if (table->ks[index].par && strlen(table->ks[index].par) > 0){
            free(table->ks[index].par);
        }
        if (table->ks[index].item){
            free(table->ks[index].item);
        }
    }
}

void free_table(Table* table){
    for (int i = 0; i < table->ksLen; i++){
        free_item(table, i);
    }
    free(table->ks);
    fclose(table->file);
    free(table);
}
