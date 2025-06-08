#include <stdio.h>
#include <string.h>
#include "table.h"
#include "input.h"

typedef int (*MenuAction)(Table*, char*, char*, char*, int*);

int create_table_action(Table* table, char* filename, char* par, char* info, int* char_offset) {
    int maxSize = 0;

    printf("Enter maxSize of the table:\n");
    if (positive_input(&maxSize) == -1){
        return -1;
    }

    printf("Enter filename:\n");
    if (get_string_input(filename) == -1){
        return -1;
    }

    *char_offset = (3 + 4 * maxSize) * 4;

    create_table(table, 0, maxSize, filename);
    write_to_file_head(table);

    return 0;
}

int add_item_action(Table* table, char* key, char* par, char* info, int* char_offset) {
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

int delete_item_action(Table* table, char* key, char* par, char* info, int* char_offset) {
    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    delete_item(table, key);
    return 0;
}

int delete_item_with_par_action(Table* table, char* key, char* par, char* info, int* char_offset) {
    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    delete_item_with_par(table, key);
    return 0;
}

int print_item_by_key_action(Table* table, char* key, char* par, char* info, int* char_offset) {
    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    print_item_by_key(table, key);
    return 0;
}

int print_table_action(Table* table, char* key, char* par, char* info, int* char_offset) {
    print_table(table);
    return 0;
}

int clear_table_action(Table* table, char* key, char* par, char* info, int* char_offset) {
    clear_table(table);
    return 0;
}

int save_table_action(Table* table, char* filename, char* par, char* info, int* char_offset) {
    clear_table(table);
    save_table(table, filename);
    return 0;
}

int import_table_action(Table* table, char* filename, char* par, char* info, int* char_offset) {
    printf("Enter filename:\n");
    if (get_string_input(filename) == -1){
        return -1;
    }

    create_table(table, 0, 1, filename);
    import_table(table);
    return 0;
}

int exit_action(Table* table, char* key, char* par, char* info, int* char_offset) {
    clear_table(table);
    free_table(table);
    return 1;
}


MenuAction menu_actions[] = {
        exit_action,
        create_table_action,
        add_item_action,
        delete_item_action,
        delete_item_with_par_action,
        print_item_by_key_action,
        print_table_action,
        clear_table_action,
        save_table_action,
        import_table_action
};

void show_menu(){
    printf("\n1. Create table\n");
    printf("2. Add item\n");
    printf("3. Delete item only with key\n");
    printf("4. Delete item with key and par\n");
    printf("5. Print item with key\n");
    printf("6. Print table\n");
    printf("7. Clear table\n");
    printf("8. Save table\n");
    printf("9. Import table\n");
    printf("0. Exit\n\n");
    printf("Enter your choice:\n");
}

int menu(Table *table){
    int choice = -1, check = 0, char_offset = 0;
    char key[100] = {0}, par[100] = {0}, info[100] = {0}, filename[100] = {0};

    do {
        show_menu();

        if (read_input(&choice) == -1){
        	clear_table(table);
        	free_table(table);
            return -1;
        }

        if (choice < 0 || choice > 9) {
            printf("Invalid choice! Please input a number once more\n");
            continue;
        }
        if (choice == 8){
            strcpy(key, filename);
        }

        check = menu_actions[choice](table, key, par, info, &char_offset);

        if (check == -1){
            return -1;
        }
        else if (check == 1){
            return 0;
        }
        if (choice == 1 || choice == 9){
            strcpy(filename, key);
        }

        memset(key, 0, 100);
        memset(par, 0, 100);
        memset(info, 0, 100);
    } while (1);
}
