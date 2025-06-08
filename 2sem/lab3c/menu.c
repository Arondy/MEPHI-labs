#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "input.h"

typedef int (*MenuAction)(KeySpace*);

int add_item_action(KeySpace *ks){
    unsigned int *key = malloc(sizeof(unsigned int));
    unsigned int data = 0;
    printf("Enter key:\n");
    if (positive_input(key) == -1){
        return -1;
    }

    printf("Enter data:\n");
    if (positive_input(&data) == -1){
        return -1;
    }
    add_item(ks, key, data);
    return 0;
}

int delete_item_action(KeySpace *ks){
    unsigned int *key = malloc(sizeof(unsigned int));
    printf("Enter key:\n");
    if (positive_input(key) == -1){
        return -1;
    }

    delete_item(ks, key);
    free(key);
    return 0;
}

int print_item_by_key_action(KeySpace *ks){
    unsigned int *key = malloc(sizeof(unsigned int));
    printf("Enter key:\n");
    if (positive_input(key) == -1){
        return -1;
    }

    print_item_by_key(ks, key);
    free(key);
    return 0;
}

int print_table_action(KeySpace *ks){
    print_table(ks);
    return 0;
}

int exit_action(KeySpace *ks){
    free_table(ks);
    return 1;
}

int read_table_from_file_action(KeySpace *ks){
    char filename[100] = {0};

    printf("Enter name of the file:\n");
    if (get_string_input(filename) == -1){
        return -1;
    }

    read_table_from_file(ks, filename);
    return 0;
}

int reorganize_table_action(KeySpace *ks){
    reorganize_table(ks);
    return 0;
}

MenuAction menu_actions[] = {
        exit_action,
        add_item_action,
        delete_item_action,
        print_item_by_key_action,
        print_table_action,
        read_table_from_file_action,
        reorganize_table_action
};

void show_menu(){
    printf("\n1. Add item\n");
    printf("2. Delete item by key\n");
    printf("3. Print item by key\n");
    printf("4. Print table\n");
    printf("5. Import table from text file\n");
    printf("6. Reorganize table\n");
    printf("0. Exit\n\n");
    printf("Enter your choice:\n");
}

int menu(KeySpace *ks){
    int choice = -1, check = 0;

    do {
        show_menu();

        if (read_input(&choice) == -1){
            free_table(ks);
            return -1;
        }

        if (choice < 0 || choice > 6) {
            printf("Invalid choice! Please input a number once more\n");
            continue;
        }

        check = menu_actions[choice](ks);
        if (check == -1){
            return -1;
        }
        else if (check == 1){
            return 0;
        }

    } while (1);
}
