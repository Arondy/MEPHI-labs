/*
Одной из типовых задач при работе с контейнерами данных является преобразование контейнера путем
наложения на каждый его элемент некоторой функции преобразования.
Условно эта операция может быть представлена следующим образом:
container<T> + func -> container<R>, где func - некоторая функция преобразования T -> R, которая выполняется для каждого элемента контейнера.

#include <functional>
#include <iostream>
#include <vector>

*
тут должна быть функция convert
*

int main() {
    //Тестовые данные 1:
    std::vector<std::string> v1;
    v1.push_back("aaa");
    v1.push_back("bb");
    v1.push_back("cccc");

    //функция преобразования, принимающая строку и возвращающая длину
    auto func1 = [](const std::string& s)-> size_t { return s.size(); };

    std::vector<size_t> v1_result = convert(v1, func1);

    for (const auto& i : v1_result) {
        std::cout << i << std::endl;
    }
    *корректный результат:
    3
    2
    4
    *

    //Тестовые данные 2:
    struct User { std::string name; std::string secondName; };
    std::vector<User> v2 = { User { "Ivan", "Ivanov" }, User { "Petya", "Petrov"} };

    //func2 - функция, принимающая пользователя и возвращающая строку "фамилия-имя", напишите её
    std::vector<std::string> v2_result = convert(v2, func2);

    for (const auto& i : v2_result) {
        std::cout << i << std::endl;
    }
    *корректный результат:
    Ivanov-Ivan
    Petrov-Petya
    *
}

Необходимо реализовать функцию convert, осуществляющую такое преобразование для контейнера std::vector<T>.
Функция convert принимает в качестве аргументов исходный контейнер source элементов типа T и функцию func преобразования элементов T->R.
В качестве результата функция должна возвращать сформированный контейнер элементов типа R, по размеру совпадающий с исходным.
Тип Т и тип R любые и могут отличаться или совпадать. Модификация исходного контейнера запрещена. Избегайте излишнего копирования
Напишите функцию convert и пример её использования в функции main в двух тестах (см. ниже).
 */


#include <functional>
#include <iostream>
#include <vector>

using std::vector, std::string;

// Не работает для лямбда функций с auto, есть какое-то *странное* решение через decltype или чет такое, искать в чате ооп по "convert"
template <typename T, typename R>
vector<R> convert(const vector<T> &vec, std::function<R(const T &)> func){
    vector<R> res;
    for (const T &el: vec){
        res.push_back(func(el));
    }
    return res;
}

struct User {
    string name;
    string secondName;
};

string func2(const User &user){
    return user.secondName + "-" + user.name;
}

int main(){
    //Тестовые данные 1:
    vector<string> v1;
    v1.push_back("aaa");
    v1.push_back("bb");
    v1.push_back("cccc");

    //функция преобразования, принимающая строку и возвращающая длину
    auto func1 = [](const string &s) -> size_t{ return s.size(); };

// Так было в оригинале
//    vector<size_t> v1_result = convert(v1, func1);
    vector<size_t> v1_result = convert(v1, std::function(func1));

    for (const auto &i: v1_result){
        std::cout << i << std::endl;
    }
    /*корректный результат:
    3
    2
    4
    */

    //Тестовые данные 2:
    vector<User> v2 = {User{"Ivan", "Ivanov"}, User{"Petya", "Petrov"}};

    //func2 - функция, принимающая пользователя и возвращающая строку "фамилия-имя", напишите её
// Так было в оригинале
//    vector<string> v2_result = convert(v2, func2);
    vector<string> v2_result = convert(v2, std::function(func2));

    for (const auto &i: v2_result){
        std::cout << i << std::endl;
    }
    /*корректный результат:
    Ivanov-Ivan
    Petrov-Petya
    */
}