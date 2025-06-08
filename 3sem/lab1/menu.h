#ifndef MENU_H
#define MENU_H
#include "matrix.h"
#include <functional>

namespace Matrix_funcs {
    using MenuFunction = std::function<void(Matrix &)>;

    void input_menu(Matrix &matrix);

    void output_menu(const Matrix &matrix);

    void process_menu(Matrix &matrix);

    void menu(Matrix &matrix);
}

#endif