#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>
#include <limits>
#include <cstring>

namespace Matrix_funcs {
    struct Matrix {
        int linesNum = 0;
        int rowsNum = 0;
        std::vector<int> values;
        std::vector<int> rowsIds;
        std::vector<int> firstNonZeroPtrs;
        std::vector<int> zeroLines;
        int nonZeroNum = 0;
        int ptrNum = 0;
        int zeroLinesNum = 0;
    };

    // шаблонная функция ввода одного числа
    template <class T>
    T getNum(){
        T a;
        while (true){
            std::cin >> a;
            if (std::cin.eof()){ // обнаружен конец файла
                throw std::runtime_error("Failed to read number: EOF");
            }
            else if (std::cin.bad()){ // обнаружена невосстановимая ошибка входного потока
                throw std::runtime_error(std::string("Failed to read number: ") + strerror(errno));
            }
            else if (std::cin.fail()){ // прочие ошибки (неправильный формат ввода)
                std::cin.clear(); // очищаем флаги состояния потока
                // игнорируем все символы до конца строки
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Those symbols are not allowed. Repeat please!" << std::endl;
            } else if (std::cin.good()){
                return a;
            }
        }
    }

    void input(Matrix &matrix);
    void output(const Matrix &matrix);
    void erase(Matrix &matrix);
    std::vector<int> process(const Matrix &matrix);
    void sort_array(std::vector<int> &array);
}

#endif