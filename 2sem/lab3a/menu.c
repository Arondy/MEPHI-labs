#include <stdio.h>
#include <string.h>
#include "table.h"
#include "input.h"

typedef int (*MenuAction)(Table*, char*, char*, char*);

int add_item_action(Table* table, char* key, char* par, char* info) {
    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    int parChosen = 0;
    printf("0 - no parent key;\n1 - next input is parent key:\n");
    read_input(&parChosen);
    if (parChosen){
        printf("Enter par:\n");
        if (get_string_input(par) == -1){
            return -1;
        }
    }
    printf("Enter info:\n");
    if (get_string_input(info) == -1){
        return -1;
    }
    add_item(table, key, par, info);
    return 0;
}

int delete_item_action(Table* table, char* key, char* par, char* info) {
    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    delete_item(table, key);
    return 0;
}

int delete_item_with_par_action(Table* table, char* key, char* par, char* info) {
    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    delete_item_with_par(table, key);
    return 0;
}

int print_item_by_key_action(Table* table, char* key, char* par, char* info) {
    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    print_item_by_key(table, key);
    return 0;
}

int print_table_action(Table* table, char* key, char* par, char* info) {
    print_table(table);
    return 0;
}

int clear_table_action(Table* table, char* key, char* par, char* info) {
    clear_table(table);
    return 0;
}

int exit_action(Table* table, char* key, char* par, char* info) {
    free_table(table);
    return 1;
}

int read_table_from_file_action(Table* table, char* filename, char* par, char* info) {
    printf("Enter name of the file:\n");
    if (get_string_input(filename) == -1){
        return -1;
    }
    read_table_from_file(table, filename);
    return 0;
}

MenuAction menu_actions[] = {
        exit_action,
        add_item_action,
        delete_item_action,
        delete_item_with_par_action,
        print_item_by_key_action,
        print_table_action,
        clear_table_action,
        read_table_from_file_action
};

void show_menu(){
    printf("\n1. Add item\n");
    printf("2. Delete item only with key\n");
    printf("3. Delete item with key and par\n");
    printf("4. Print item with key\n");
    printf("5. Print table\n");
    printf("6. Clear table\n");
    printf("7. Import table from text file\n");
    printf("0. Exit\n\n");
    printf("Enter your choice:\n");
}

int menu(Table table){
    int choice = -1, check = 0;
    char key[100] = {0}, par[100] = {0}, info[100] = {0};

    do {
        show_menu();

        if (read_input(&choice) == -1){
            return -1;
        }

        if (choice < 0 || choice > 7) {
            printf("Invalid choice! Please input a number once more\n");
            continue;
        }

        check = menu_actions[choice](&table, key, par, info);
        if (check == -1){
            return -1;
        }
        else if (check == 1){
            return 0;
        }

        memset(key, 0, 100);
        memset(par, 0, 100);
        memset(info, 0, 100);
    } while (1);
}
