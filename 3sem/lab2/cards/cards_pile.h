#ifndef CARDS_PILE_H
#define CARDS_PILE_H

#include <random>
#include <utility>
#include <map>
#include "cards.h"

using namespace Cards;

namespace CardsPiles {
    /// Колода карт
    class CardsPile {
    private:
        Card *array = new Card[52];
        int len = 0;

        /// Сдвиг массива на одно место влево
        /// \param cardArray Массив карт
        /// \param arLen Длина массива карт
        /// \param index Индекс, начиная с которого будет выполняться сдвиг
        static void shift_array_by_one(Card *cardArray, int &arLen, int index);

        /// Проверка колоды на содержание карты
        /// \param card Карта, дубликат которой ищется в колоде
        /// \return 1, если карта уже есть в колоде; 0, если нет
        bool has_duplicate(Card &card) const;
    public:
        // Конструкторы
        /// Инициализация экземпляра заданным количеством случайных карт
        /// \param cardsNum Количество карт в колоде
        explicit CardsPile(int cardsNum);

        /// Инициализация экземпляра с полным набором карт
        explicit CardsPile();

        /// Инициализация экземпляра копированием
        /// \param second Копируемый экземпляр
        CardsPile(CardsPile const &second);

        /// Инициализация экземпляра перемещением
        /// \param second Перемещаемый экземпляр
        CardsPile(CardsPile &&second) noexcept;

        // Деструктор
        ~CardsPile(){ delete[] array; }

        // Геттеры
        /// \return Размер массива
        int getSize() const { return len; }

        /// \return Ссылку на массив
        Card *getPile() const { return array; }

        // Перегрузки
        /// Перенос верхней (первой) карты из одной колоды в другую
        /// \param pile1 Колода-источник
        /// \param pile2 Колода-приемник
        /// \return Ссылку на колоду-источник
        friend CardsPile &operator>>(CardsPile &pile1, CardsPile &pile2);

        /// Обращение к элементам массива напрямую через экземпляр
        /// \param index Индекс элемента
        /// \return Элемент массива, лежащий по заданному индексу
        Card &operator[](int index) const;

        /// Суммирование двух колод
        /// \param second Вторая колода
        /// \return Новую колоду
        CardsPile operator+(const CardsPile &second) const;

        /// Присваивание копированием
        /// \param second Колода, из которой берутся данные
        /// \return Ссылку на левую колоду
        CardsPile &operator=(const CardsPile &second);

        /// Присваивание перемещением
        /// \param second Колода, из которой берутся данные
        /// \return Ссылку на левую колоду
        CardsPile &operator=(CardsPile &&second) noexcept;

        // Методы
        /// \return Возвращает true, если массив пустой; false, если нет
        bool empty() const { return !len; }

        /// Добавляет карту в колоду
        /// \param card Добавляемая карта
        /// \return Ссылку на колоду
        CardsPile &add(Card card);

        /// Удаляет карту из колоды
        /// \param index Индекс удаляемого элемента
        /// \return Ссылку на колоду
        CardsPile &remove(int index);

        /// Перемешивание колоды случайным образом
        /// \return Ссылку на колоду
        CardsPile &myShuffle();

        /// Сортировка колоды
        /// \param direct 1, если по возрастанию, 0, если по убыванию
        /// \param rank 1, если сортировка по рангу, 0, если по масти
        /// \return Ссылку на колоду
        CardsPile &sort(int direct, int rank);

        /// Выделение из группы карт подгруппы карт заданной масти
        /// \param suit Масть, карты с которой попадут в подгруппу
        /// \return Новую колоду
        CardsPile getSubPile(const string &suit) const;

        // Ввод/вывод состояния класса
        /// Ввод экземпляра из входного потока
        /// \param istream Входной поток
        /// \param numOfCards Число вводимых карт
        /// \return Входной поток
        std::istream &input(std::istream &istream, int numOfCards);

        /// Вывод экземпляра в выходной поток
        /// \param ostream Выходной поток
        /// \return Выходной поток
        std::ostream &print(std::ostream &ostream) const;
    };
}

#endif