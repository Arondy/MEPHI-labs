#ifndef CARDS_H
#define CARDS_H

#include <random>
#include <utility>
#include <map>
#include <stdexcept>

using std::string;

namespace Cards {
    static std::vector<string> mySuits = {"Hearts", "Diamonds", "Clubs", "Pikes"}; // По возрастанию - CDHP
    static std::vector<int> myRanks = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    static std::map<string, string> mySuitIcons{
            {"Hearts",   "♥"},
            {"Diamonds", "♦"},
            {"Clubs",    "♣"},
            {"Pikes",    "♠"}
    };
    static std::vector<string> myRankIcons = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    /// Игральная карта
    class Card {
    private:
        int rank;
        string suit;
    public:
        // Конструкторы
        /// Инициализация случайной карты
        Card();

        /// Инициализация карты с заданными рангом и мастью
        /// \param rank Ранг
        /// \param suit Масть
        Card(int rank, const string &suit);

        // Геттеры
        /// \return Ранг карты
        int getRank() const{ return rank; }

        /// \return Масть карты
        string getSuit() const{ return suit; }

        // Сеттеры
        /// Изменение ранга на заданный
        /// \param rank Ранг, на который меняем
        /// \return Карту
        Card &setRank(int rank);

        /// Изменение масти на заданную
        /// \param suit Масть, на которую меняем
        /// \return Карту
        Card &setSuit(const string &suit);

        /// Сравнение
        /// \param second Карта, которую сравниваем с левой
        /// \return результат сравнения в формате strong_ordering
        auto operator<=>(const Card &second) const{
            return (rank <=> second.rank);
        }

        /// \return Форматирование значения карты для красивого вывода
        string format() const;

        /// Ввод состояния
        /// \return Входной поток
        friend std::istream &operator>>(std::istream &, Card &);

        // Перегрузки
        /// Строгое сравнение на равенство
        /// \param second Карта, которую сравниваем с левой
        /// \return 1, если равна; 0, если нет
        bool operator==(const Card &second) const { return rank == second.rank && suit == second.suit; }

        /// Присваивание копированием
        /// \param second Карта, значения которой копируем
        /// \return Карту
        Card& operator=(const Card& second){
            if (this != &second){
                rank = second.rank;
                suit = second.suit;
            }
            return *this;
        }
    };

    /// Вывод состояния
    /// \return Выходной поток
    std::ostream &operator<<(std::ostream &, const Card &);

    /// Сравнение (для тестирования)
    /// \param cmp Сравниваемое значение
    /// \return 1/0/-1
    int compare(auto cmp){
        if (cmp > 0){
            return 1;
        } else if (cmp == 0){
            return 0;
        } else {
            return -1;
        }
    }

    /// Компаратор для сортировки в массиве по масти
    /// \param card1 Первая сравниваемая карта
    /// \param card2 Вторая сравниваемая карта
    /// \return 1, если масть первой карты меньше, 0, если больше
    bool is_less(const Card& card1, const Card& card2);

    /// Компаратор для сортировки в массиве по масти
    /// \param card1 Первая сравниваемая карта
    /// \param card2 Вторая сравниваемая карта
    /// \return 1, если масть первой карты больше, 0, если меньше
    bool is_greater(const Card& card1, const Card& card2);
}

#endif