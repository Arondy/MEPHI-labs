#include <iostream>
#include <vector>
#include "matrix.h"
#include "menu.h"

using namespace Matrix_funcs;

int main(){
    Matrix matrix;
    try {
        menu(matrix);
    }
    catch(const std::bad_alloc &ba) {
        std::cerr << "Not enough memory" << std::endl;
        erase(matrix);
        return 1;
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        erase(matrix);
        return 1;
    }
    return 0;
}