#ifndef MENU_H
#define MENU_H
#include "cards/cards_pile.h"
#include <functional>

using namespace CardsPiles;
using std::cout, std::cin, std::endl;

namespace Menu {
    using MenuFunction = std::function<void(CardsPile &)>;
    using InitMenuFunc = std::function<void(CardsPile &)>;

    // шаблонная функция ввода одного числа
    template <class T>
    static T getNum(int num1, int num2){
        T a;
        while (true){
            cin >> a;
            if (cin.eof()){ // обнаружен конец файла
                throw std::runtime_error("EOF - Program has been stopped");
            }
            else if (cin.bad()){ // обнаружена невосстановимая ошибка входного потока
                throw std::runtime_error("Failed to read number");
            }
            else if (cin.fail()){ // прочие ошибки (неправильный формат ввода)
                cin.clear(); // очищаем флаги состояния потока
                // игнорируем все символы до конца строки
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << "Those symbols are not allowed. Repeat please!" << endl;
            } else if (a < num1 || a > num2){
                cout << "You can input only values between " << num1 << " and " << num2 << endl;
            } else if (cin.good()){
                return a;
            }
        }
    }

    static string inputAndCheckSuit(){
        string suit;
        do {
            cout << "Input desired suit: " << endl;
            cin >> suit;
            if (std::find(mySuits.begin(), mySuits.end(), suit) == mySuits.end()){
                cout << "Your input doesn't match any suit" << endl;
            } else {
                return suit;
            }
        } while (true);
    }

    void menu(CardsPile &pile);
}

#endif