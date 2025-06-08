#include <iostream>
#include <limits>
#include <random>
#include <algorithm>
#include "cards_pile.h"
#include "cards.h"
//#include "cards.cpp"

using namespace Cards;

namespace CardsPiles {
    // Вспомогательные функции
    void CardsPile::shift_array_by_one(Card *cardArray, int &arLen, int index){
        if (cardArray == nullptr || arLen <= 0 || index < 0 || index >= arLen){
            return;
        }

        for (int i = index; i < arLen - 1; i++){
            cardArray[i] = cardArray[i + 1];
        }
        arLen--;
    }

    bool CardsPile::has_duplicate(Card &card) const{
        if (len == 0 || std::find(array, &(array[len]), card) == &(array[len])){
            return false;
        } else {
            return true;
        }
    }

    // Конструкторы
    CardsPile::CardsPile(int cardsNum){
        if (cardsNum < 0 || cardsNum > 52){
            delete[] array;
            throw std::runtime_error("Length of the pile can't be more than 52");
        }
        if (!cardsNum){
            return;
        }

        std::random_device randNum;
        std::mt19937 rand(randNum());
        Card *tmpArray = new Card[52];
        int tmpLen = 0;

        for (int myRank: myRanks){
            for (const string &mySuit: mySuits){
                tmpArray[tmpLen++] = Card{myRank, mySuit};
            }
        }

        int index = 0;
        for (; len < cardsNum; len++){
            index = rand() % tmpLen;
            array[len] = tmpArray[index];
            shift_array_by_one(tmpArray, tmpLen, index);
        }
        delete[] tmpArray;
    }

    CardsPile::CardsPile(){
        for (int myRank: myRanks){
            for (const string &mySuit: mySuits){
                array[len++] = Card{myRank, mySuit};
            }
        }
    }

    CardsPile::CardsPile(CardsPile const &second) : len{second.len}{
        if (this != &second){
            std::copy(second.array, second.array + len, array);
        }
    }

    CardsPile::CardsPile(CardsPile &&second) noexcept: len{second.len}, array{second.array}{
        if (this != &second){
            second.len = 0;
            second.array = nullptr;
        }
    }

    // Перегрузки
    CardsPile &operator>>(CardsPile &source, CardsPile &dest){
        if (source.empty()){
            throw std::runtime_error("Source pile is empty");
        }
        if (dest.len == 52){
            throw std::runtime_error("Destination pile is full");
        }
        if (dest.has_duplicate(source.array[source.len - 1])){
            throw std::runtime_error("Destination pile already has this card");
        }
        dest.array[dest.len++] = source.array[--source.len];
        return source;
    }

    Card &CardsPile::operator[](int index) const{
        if (index < 0 || index >= len){
            throw std::out_of_range("Index out of pile's length");
        }
        return array[index];
    }

    CardsPile CardsPile::operator+(const CardsPile &second) const{
        CardsPile res{0};

        std::copy(second.array, second.array + second.len, res.array);
        res.len += second.len;

        for (int i = 0; i < len; i++){
            if (!second.has_duplicate(array[i])){
                res.add(array[i]);
            }
        }
        return res;
    }

    CardsPile &CardsPile::operator=(const CardsPile &second){
        if (this != &second){
            Card *new_array = new Card[52];
            len = second.len;
            delete[] array;
            array = nullptr;
            array = new_array;
            std::copy(second.array, second.array + len, array);
        }
        return *this;
    }

    CardsPile &CardsPile::operator=(CardsPile &&second) noexcept {
        if (this != &second){
            std::swap(len, second.len);
            std::swap(array, second.array);
        }
        return *this;
    }

    // Методы
    CardsPile &CardsPile::add(Card card){
        if (len == 52){
            throw std::runtime_error("The pile is already full");
        } else if (this->has_duplicate(card)){
            throw std::runtime_error("Destination pile already has this card");
        }
        array[len++] = card;
        return *this;
    }

    CardsPile &CardsPile::remove(int index){
        if (index < 0 || index >= len){
            throw std::out_of_range("Index out of pile's length");
        }
        shift_array_by_one(array, len, index);
        return *this;
    }

    CardsPile &CardsPile::myShuffle(){
        if (len > 1){
            std::random_device randNum;
            std::mt19937 rand(randNum());
            std::shuffle(array, array + len, rand);
        }
        return *this;
    }

    CardsPile &CardsPile::sort(int direct, int rank){
        if (len <= 1){
            return *this;
        }

        if (rank){
            if (direct){
                std::sort(array, array + len);
            } else {
                std::sort(array, array + len, std::greater<>());
            }
        } else {
            if (direct){
                std::sort(array, array + len, is_less);
            } else {
                std::sort(array, array + len, is_greater);
            }
        }
        return *this;
    }

    CardsPile CardsPile::getSubPile(const string &suit) const{
        CardsPile res{0};
        for (int i = 0; i < len; i++){
            if (array[i].getSuit() == suit){
                res.add(array[i]);
            }
        }
        return res;
    }

    // Ввод/вывод состояния класса
    std::istream &CardsPile::input(std::istream &istream, int numOfCards){
        while (len < 52 && numOfCards > 0){
            Card tmpCard;
            istream >> tmpCard;
            try {
                this->add(tmpCard);
            } catch (std::runtime_error &re){

            }
            numOfCards--;
        }

        istream.clear();
        istream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return istream;
    }

    std::ostream &CardsPile::print(std::ostream &ostream) const{
        if (len == 0){
            ostream << "Pile is empty";
        } else {
            for (int i = 0; i < len; i++){
                ostream << array[i];
            }
        }
        ostream << std::endl;
        return ostream;
    }
}