#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "square.h"


namespace Components {
    using std::vector, std::string, std::unordered_map, std::pair;

    static const unordered_map<string, string> figureToLetter = {
            {"pawn", ""},
            {"knight", "N"},
            {"bishop", "B"},
            {"rook", "R"},
            {"queen", "Q"},
            {"king", "K"}
    };
    static const unordered_map<char, int> columnToNum = {
            {'a', 0},
            {'b', 1},
            {'c', 2},
            {'d', 3},
            {'e', 4},
            {'f', 5},
            {'g', 6},
            {'h', 7}
    };
    static const unordered_map<int, char> numToColumn = {
            {0, 'a'},
            {1, 'b'},
            {2, 'c'},
            {3, 'd'},
            {4, 'e'},
            {5, 'f'},
            {6, 'g'},
            {7, 'h'}
    };

    class GameHistory {
    private:
        vector<string> history;
    public:
        GameHistory() = default;
        explicit GameHistory(vector<string> history): history{std::move(history)}{};
        [[nodiscard]] vector<string> getHistory() const { return history; };
        [[nodiscard]] string getLastMove() const ;
        void addMove(const Square &currS, const string &currName, const Square &newS, bool takes, const string &state, bool promote);
    };
}

#endif