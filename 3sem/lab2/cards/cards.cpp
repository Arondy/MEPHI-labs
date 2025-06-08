#include <iostream>
#include <format>
#include <algorithm>
#include "cards.h"

namespace Cards {
    // Конструкторы
    Card::Card(){
        std::random_device randNum;
        std::mt19937 gen(randNum());
        std::uniform_int_distribution<> rankDistribution(2, 14);
        std::uniform_int_distribution<> suitDistribution(0, 3);
        rank = rankDistribution(gen);
        suit = mySuits[suitDistribution(gen)];
    }

    Card::Card(int rank, const std::string &suit){
        if (rank < 2 || rank > 14){
            throw std::runtime_error("Your number isn't a valid rank!");
        }
        if (std::find(mySuits.begin(), mySuits.end(), suit) == mySuits.end()){
            throw std::runtime_error("Your string isn't one of valid suits!");
        }
        this->rank = rank;
        this->suit = suit;
    }

    // Сеттеры
    Card &Card::setSuit(const string &newSuit){
        if (std::find(mySuits.begin(), mySuits.end(), newSuit) == mySuits.end()){
            throw std::runtime_error("Your string isn't one of valid suits!");
        }
        suit = newSuit;
        return *this;
    }

    Card &Card::setRank(int newRank){
        if (newRank < 2 || newRank > 14){
            throw std::runtime_error("Your number isn't a valid rank!");
        }
        rank = newRank;
        return *this;
    }

    // Форматирование
    string Card::format() const{
        string res = std::format("{}{}", myRankIcons[rank - 2], mySuitIcons[suit]);
        return res;
    }

    // Ввод/вывод состояния
    std::istream &operator>>(std::istream &istream, Card &card){
        istream >> card.rank >> card.suit;
        if (istream.good()){
            if (card.rank < 2 || card.rank > 14){
                throw std::runtime_error("Your number isn't a valid rank!");
            }
            if (std::find(mySuits.begin(), mySuits.end(), card.suit) == mySuits.end()){
                throw std::runtime_error("Your string isn't one of valid suits!");
            }
        }
        return istream;
    }

    std::ostream &operator<<(std::ostream &ostream, const Card &card){
        ostream << card.format() << ' ';
        return ostream;
    }

    bool is_less(const Card& card1, const Card& card2){
        return card1.getSuit() < card2.getSuit();
    }

    bool is_greater(const Card& card1, const Card& card2){
        return card1.getSuit() > card2.getSuit();
    }
}