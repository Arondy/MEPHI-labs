#include <stdio.h>
#include "binary_tree.h"
#include "input.h"
#include "tree_time.h"
#include "menu.h"

int add_item_action(Node **root){
    char key[256] = {0};
    unsigned int value = 0;

    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    printf("Enter value:\n");
    if (positive_input(&value) == -1){
        return -1;
    }
    if (insert_node(root, key, value) == -1){
        return -1;
    }

    return 0;
}

int remove_node_action(Node **root){
    char key[256] = {0};
    unsigned int num = 0;
    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    printf("Enter an order number:\n0 - Delete whole node\n1+ - Delete node by index (1 - first and so on)\n");
    if (positive_input(&num) == -1){
        return -1;
    }

    remove_node(root, key, num);
    return 0;
}

int traverse_tree_action(Node **root){
    char key1[256] = {0}, key2[256] = {0};
    printf("Enter key1:\n");
    if (get_string_input(key1) == -1){
        return -1;
    }
    printf("Enter key2:\n");
    if (get_string_input(key2) == -1){
        return -1;
    }

    traverse_tree_in_range(*root, key1, key2);
    return 0;
}

int print_tree_as_tree_action(Node **root){
    print_tree_as_tree(*root, 0, 0);
    return 0;
}

int find_node_by_key_action(Node **root){
    char key[256] = {0};
    unsigned int num = 0;
    printf("Enter key:\n");
    if (get_string_input(key) == -1){
        return -1;
    }
    printf("Enter an order number:\n");
    if (positive_input(&num) == -1){
        return -1;
    }

    find_node_by_key(*root, key, num);
    return 0;
}

int find_max_node_action(Node **root){
    find_max_node(*root);
    return 0;
}

int import_tree_from_file_action(Node **root){
    char filename[100] = {0};

    printf("Enter name of the file:\n");
    if (get_string_input(filename) == -1){
        return -1;
    }

    if (import_tree_from_file(root, filename) == -1){
        return -1;
    }

    return 0;
}

void show_time_menu(){
    printf("\n0. Time \"remove node\"\n");
    printf("1. Time \"traverse tree in range\"\n");
    printf("2. Time \"find node by key\"\n");
    printf("3. Time \"find max node\"\n");
    printf("Enter your choice:\n");
}

function_ptr func_array[] = {
        remove_node_time,
        traverse_tree_in_range_time,
        find_node_by_key_time,
        find_max_node_time
};

int time_tree_action(Node **root){
    unsigned int numOfElemets = 0, numOfRuns = 0, choice = 0;
    double avgTime = 0;
    show_time_menu();
    if (positive_input(&choice) == -1){
        return -1;
    }
    printf("Enter number of elements in tree:\n");
    if (positive_input(&numOfElemets) == -1){
        return -1;
    }
    printf("Enter number of runs:\n");
    if (positive_input(&numOfRuns) == -1){
        return -1;
    }
    avgTime = time_tree(numOfElemets, numOfRuns, func_array[choice]);
    printf("Average time - %f\n", avgTime);
    return 0;
}

int exit_action(Node **root){
    delete_tree(root);
    return 1;
}


MenuAction menu_actions[] = {
        exit_action,
        add_item_action,
        remove_node_action,
        traverse_tree_action,
        print_tree_as_tree_action,
        find_node_by_key_action,
        find_max_node_action,
        import_tree_from_file_action,
        time_tree_action
};

void show_menu(){
    printf("\n1. Add item\n");
    printf("2. Remove node by key\n");
    printf("3. Traverse tree\n");
    printf("4. Print tree as tree\n");
    printf("5. Print node by key\n");
    printf("6. Print max node\n");
    printf("7. Import tree from the file\n");
    printf("8. Time functions\n");
    printf("0. Exit\n\n");
    printf("Enter your choice:\n");
}

int menu(Node **root){
    int choice = -1, check = 0;

    do {
        show_menu();

        if (read_input(&choice) == -1){
            delete_tree(root);
            return -1;
        }

        if (choice < 0 || choice > 8) {
            printf("Invalid choice! Please input a number once more\n");
            continue;
        }

        check = menu_actions[choice](root);
        if (check == -1){
            return -1;
        }
        else if (check == 1){
            return 0;
        }

    } while (1);
}
