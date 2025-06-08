#include "history.h"

namespace Components {
    string GameHistory::getLastMove() const {
        if (!history.empty()){
            return history.back();
        } else {
            return "";
        }
    }

    void GameHistory::addMove(const Square &currS, const string &currName, const Square &newS, bool takes, const string &state, bool promote){
        string move;

        if (currName == "king"){
            if (newS.column - currS.column == 2){
                move = "O-O";
            } else if (newS.column - currS.column == -2){
                move = "O-O-O";
            }
        }
        if (move.empty()){
            move += figureToLetter.at(currName);
            if (takes){
                if (move.empty()){
                    move += numToColumn.at(currS.column);
                }
                move += 'x';
            }
            move += numToColumn.at(newS.column);
            move += std::to_string(8 - newS.line);
            if (promote){
                move += "=Q";
            }
            if (state == "check"){
                move += '+';
            } else if (state == "mate"){
                move += '#';
            }
        }
        history.push_back(move);
    }
}