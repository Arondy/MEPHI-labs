#include <iostream>
#include <vector>
#include "menu.h"

namespace Matrix_funcs {
    void input_menu(Matrix &matrix){
        input(matrix);
    }

    void output_menu(const Matrix &matrix){
        output(matrix);
    }

    void process_menu(Matrix &matrix){
        std::vector<int> result = process(matrix);
        sort_array(result);
    }

    void erase_menu(Matrix &matrix){
        erase(matrix);
    }

    void show_menu(){
        std::cout << "1. Input matrix" << std::endl;
        std::cout << "2. Output matrix" << std::endl;
        std::cout << "3. Process matrix" << std::endl;
        std::cout << "4. Erase matrix" << std::endl;
        std::cout << "5. Stop program" << std::endl;
        std::cout << "Enter your choice:" << std::endl;
    }

    void menu(Matrix &matrix){
        std::vector<MenuFunction> menu_functions = {
                input_menu,
                output_menu,
                process_menu,
                erase_menu
        };

        int choice = 0;
        do {
            show_menu();
            choice = getNum<int>();
            if (choice >= 1 && choice <= menu_functions.size()){
                menu_functions[choice - 1](matrix);
            } else if (choice == 5){
                erase(matrix);
                return;
            } else {
                std::cout << "Invalid choice. Try again:" << std::endl;
            }
        } while (true);
    }
}