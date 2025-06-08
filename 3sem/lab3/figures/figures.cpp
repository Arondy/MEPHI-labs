#include "figures.h"

namespace Figures {
    bool hasFriendlyPiece(const vector<vector<shared_ptr<Figure>>> &gameField, const Square &s, const Square &d){
        return (gameField[d.line][d.column] && gameField[s.line][s.column] &&
                gameField[d.line][d.column]->getColor() == gameField[s.line][s.column]->getColor());
    }

    bool hasEnemyPiece(vector<vector<shared_ptr<Figure>>> gameField, const Square &s, const Square &d){
        return (gameField[d.line][d.column] && gameField[s.line][s.column] &&
                gameField[d.line][d.column]->getColor() != gameField[s.line][s.column]->getColor());
    }

    void getLineMoves(const vector<vector<shared_ptr<Figure>>> &gameField, const int direction[2],
                      vector<Square> &possibleMoves, Square s, bool forKing, const string &color){
        int line, column;
        int dL = direction[0];
        int dC = direction[1];
        for (int i = 1; i < 8; i++){
            line = s.line + i * dL;
            column = s.column + i * dC;

            if (line >= 0 && line < 8 && column >= 0 && column < 8){
                Square move{line, column};

                if (gameField[line][column]){
                    if (!forKing && hasFriendlyPiece(gameField, s, move)){
                        break;
                    } else {
                        possibleMoves.emplace_back(line, column);
                        if (forKing && gameField[line][column]->getName() == "king"
                            && gameField[line][column]->getColor() != color){
                            if (line + dL >= 0 && line + dL < 8 && column + dC >= 0 && column + dC < 8){
                                possibleMoves.emplace_back(line + dL, column + dC);
                            }
                        }
                        break;
                    }
                }
                possibleMoves.push_back(move);
            } else {
                break;
            }
        }
    }

    Figure::Figure(const string &color, const string &name){
        if (color != "white" && color != "black"){
            throw std::runtime_error("Color is wrong");
        }
        if (std::find(figureNames.begin(), figureNames.end(), name) == figureNames.end()){
            throw std::runtime_error("Name is wrong");
        }
        this->color = color;
        this->name = name;
    }

    bool Figure::whoseTurn_move_checkup(const string &whoseTurn, bool forKing) const{
        if (color == whoseTurn || forKing){
            return true;
        }
        return false;
    }

    bool Figure::onCheck_move_checkup(const vector<vector<shared_ptr<Figure>>> &gameField,
                                      const unordered_map<string, Square> &kingsPosition, Square s, Square d,
                                      const vector<Square> &checkInfo) const{
        if (d.line < 0 || d.line >= 8 || d.column < 0 || d.column >= 8){
            return false;
        }

        if (checkInfo.empty()){
            return true;
        }

        string sColor = gameField[s.line][s.column]->getColor();
        string tColor = gameField[checkInfo[0].line][checkInfo[0].column]->getColor();

        if (sColor == tColor){
            return true;
        }

        if (checkInfo.size() == 1){
            int lineDiff, columnDiff, dL, dC;
            Square threat = checkInfo[0];
            Square kingPos = kingsPosition.at(color);

            lineDiff = abs(threat.line - kingPos.line);
            columnDiff = abs(threat.column - kingPos.column);

            if (lineDiff == 0 || columnDiff == 0 || lineDiff == columnDiff){
                dL = (lineDiff == 0) ? 0 : (threat.line < kingPos.line) ? 1 : -1;
                dC = (columnDiff == 0) ? 0 : (threat.column < kingPos.column) ? 1 : -1;

                for (int i = threat.line, j = threat.column;
						i != kingPos.line || j != kingPos.column; i += dL, j += dC){
                    if (d == Square{i, j}){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool Figure::onAttackLine_move_checkup(const vector<vector<shared_ptr<Figure>>> &gameField,
                                           const unordered_map<string, Square> &kingsPosition, Square s,
                                           Square d) const{
        Square king = kingsPosition.at(color);
        shared_ptr<Figure> figure;
        int lineDiff, columnDiff, dL, dC;
        string attFigName;

        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                figure = gameField[i][j];

                if (!figure || figure->getColor() == color){
                    continue;
                }

                attFigName = figure->getName();
                lineDiff = king.line - i;
                columnDiff = king.column - j;

                bool isDiagonal = (abs(lineDiff) == abs(columnDiff));
                bool isOrthogonal = (lineDiff == 0 && columnDiff != 0) || (lineDiff != 0 && columnDiff == 0);

                if ((attFigName == "bishop" && isDiagonal) ||
                    (attFigName == "rook" && isOrthogonal) ||
                    (attFigName == "queen" && (isDiagonal || isOrthogonal))){
                    if (d == Square{i, j}){
                        return true;
                    }

                    dL = (lineDiff > 0) ? 1 : (lineDiff < 0) ? -1 : 0;
                    dC = (columnDiff > 0) ? 1 : (columnDiff < 0) ? -1 : 0;
                    bool onLine = false;
                    int figuresOnLine = 0;

                    for (int l = i + dL, c = j + dC; l != king.line || c != king.column; l += dL, c += dC){
                        if (d == Square{l, c}){
                            return true;
                        }
                        if (s == Square{l, c}){
                            onLine = true;
                        }
                        if (gameField[l][c]){
                            if (gameField[l][c]->getColor() == color){
                                figuresOnLine++;
                            } else {
                                onLine = false;
                                break;
                            }
                        }
                    }
                    if (onLine && figuresOnLine == 1){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool Pawn::checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square s, Square d,
                         const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                         bool forKing, const string &lastMove) const{
        if (s.line >= 8 || s.column >= 8){
            return false;
        }
        if (s.line < 0 || s.column < 0){
            return false;
        }

        // Первый ход (на две клетки вперёд)
        if ((s.line == 1 && color == "black")){
            if (s.line - d.line == -2 && s.column - d.column == 0 && !gameField[d.line][d.column]
                && !gameField[d.line - 1][d.column] && !forKing){
                return true;
            }
        } else if (s.line == 6 && color == "white"){
            if (s.line - d.line == 2 && s.column - d.column == 0 && !gameField[d.line][d.column]
                && !gameField[d.line + 1][d.column] && !forKing){
                return true;
            }
        }

        // Обычный ход вперёд
        if (color == "black"){
            if (s.line - d.line == -1 && s.column - d.column == 0 && !gameField[d.line][d.column] && !forKing){
                return true;
            }
        } else {
            if (s.line - d.line == 1 && s.column - d.column == 0 && !gameField[d.line][d.column] && !forKing){
                return true;
            }
        }

        // Съедение
        if (color == "black"){
            if (s.line - d.line == -1 && abs(s.column - d.column) == 1
                && (gameField[d.line][d.column] && gameField[d.line][d.column]->getColor() ==
                                                   "white" || forKing)){
                return true;
            }
        } else {
            if (s.line - d.line == 1 && abs(s.column - d.column) == 1 && (gameField[d.line][d.column] &&
                                                                          gameField[d.line][d.column]->getColor() ==
                                                                          "black" || forKing)){
                return true;
            }
        }

        // En passant
        if (abs(s.column - d.column) == 1 && !forKing){
            if ((s.line == 4 && color == "black")){
                if (s.line - d.line == -1 && lastMove == format("{}{}", numToColumn.at(d.column), 4)){
                    return true;
                }
            } else if (s.line == 3 && color == "white"){
                if (s.line - d.line == 1 && lastMove == format("{}{}", numToColumn.at(d.column), 5)){
                    return true;
                }
            }
        }

        return false;
    }

    vector<Square> Pawn::getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square s,
                                          const unordered_map<string, Square> &kingsPosition,
                                          const vector<Square> &checkInfo, bool forKing, const string &whoseTurn,
                                          const string &lastMove) const{
        vector<Square> possibleMoves;
        vector<Square> realMoves;

        if (gameField[s.line][s.column] && gameField[s.line][s.column]->getColor() == "white"){
            possibleMoves = {
                    Square(s.line - 1, s.column),
                    Square(s.line - 2, s.column),
                    Square(s.line - 1, s.column + 1),
                    Square(s.line - 1, s.column - 1)
            };
        } else {
            possibleMoves = {
                    Square(s.line + 1, s.column),
                    Square(s.line + 2, s.column),
                    Square(s.line + 1, s.column + 1),
                    Square(s.line + 1, s.column - 1)
            };
        }
        for (Square possibleMove: possibleMoves){
            if (whoseTurn_move_checkup(whoseTurn, forKing) &&
                onCheck_move_checkup(gameField, kingsPosition, s, possibleMove, checkInfo) &&
                onAttackLine_move_checkup(gameField, kingsPosition, s, possibleMove) &&
                checkMove(gameField, s, possibleMove, kingsPosition, checkInfo, forKing, lastMove)){
                realMoves.push_back(possibleMove);
            }

        }
        return realMoves;
    }

    bool Knight::checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square s, Square d,
                           const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                           bool forKing, const string &lastMove) const{
        if (s.line >= 8 || s.column >= 8){
            return false;
        }
        if (s.line < 0 || s.column < 0){
            return false;
        }

        if (abs(s.line - d.line) == 2 && abs(s.column - d.column) == 1){
            if (!hasFriendlyPiece(gameField, s, d) || forKing){
                return true;
            }
        }
        if (abs(s.line - d.line) == 1 && abs(s.column - d.column) == 2){
            if (!hasFriendlyPiece(gameField, s, d) || forKing){
                return true;
            }
        }
        return false;
    }

    vector<Square> Knight::getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square s,
                                            const unordered_map<string, Square> &kingsPosition,
                                            const vector<Square> &checkInfo, bool forKing, const string &whoseTurn,
                                            const string &lastMove) const{
        vector<Square> realMoves;
        vector<Square> possibleMoves = {
                Square(s.line + 1, s.column + 2),
                Square(s.line + 1, s.column - 2),
                Square(s.line - 1, s.column + 2),
                Square(s.line - 1, s.column - 2),
                Square(s.line + 2, s.column + 1),
                Square(s.line + 2, s.column - 1),
                Square(s.line - 2, s.column + 1),
                Square(s.line - 2, s.column - 1)
        };

        for (Square possibleMove: possibleMoves){
            if (whoseTurn_move_checkup(whoseTurn, forKing) &&
                onCheck_move_checkup(gameField, kingsPosition, s, possibleMove, checkInfo) &&
                onAttackLine_move_checkup(gameField, kingsPosition, s, possibleMove) &&
                checkMove(gameField, s, possibleMove, kingsPosition, checkInfo, forKing, lastMove)){
                realMoves.push_back(possibleMove);
            }
        }
        return realMoves;
    }

    bool Bishop::checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square s, Square d,
                           const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                           bool forKing, const string &lastMove) const{
        if (s.line >= 8 || s.column >= 8){
            return false;
        }
        if (s.line < 0 || s.column < 0){
            return false;
        }

        if (abs(s.line - d.line) == abs(s.column - d.column)){
            int deltaL = (d.line > s.line) ? 1 : -1;
            int deltaC = (d.column > s.column) ? 1 : -1;
            for (int i = s.line + deltaL, j = s.column + deltaC; i != d.line; i += deltaL, j += deltaC){
                if (gameField[i][j]){
                    return false;
                }
            }
            if (!hasFriendlyPiece(gameField, s, d)){
                return true;
            }
        }
        return false;
    }

    vector<Square> Bishop::getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square s,
                                            const unordered_map<string, Square> &kingsPosition,
                                            const vector<Square> &checkInfo, bool forKing, const string &whoseTurn,
                                            const string &lastMove) const{
        vector<Square> possibleMoves;
        vector<Square> realMoves;
        int line, column;

        int directions[4][2] = {{-1, -1},
                                {-1, 1},
                                {1,  -1},
                                {1,  1}};

        for (auto &direction: directions){
            getLineMoves(gameField, direction, possibleMoves, s, forKing, color);
        }

        for (Square possibleMove: possibleMoves){
            if (whoseTurn_move_checkup(whoseTurn, forKing) &&
                onCheck_move_checkup(gameField, kingsPosition, s, possibleMove, checkInfo) &&
                onAttackLine_move_checkup(gameField, kingsPosition, s, possibleMove)){
                realMoves.push_back(possibleMove);
            }
        }
        return realMoves;
    }

    bool Rook::checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square s, Square d,
                         const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                         bool forKing, const string &lastMove) const{
        if (s.line >= 8 || s.column >= 8){
            return false;
        }
        if (s.line < 0 || s.column < 0){
            return false;
        }

        if (s.line - d.line != 0 && s.column - d.column != 0){
            return false;
        }
        int deltaL;
        int deltaC;

        if (d.line == s.line){
            deltaL = 0;
            deltaC = (d.column > s.column) ? 1 : -1;
        } else {
            deltaL = (d.line > s.line) ? 1 : -1;
            deltaC = 0;
        }
        for (int i = s.line + deltaL, j = s.column + deltaC; i != d.line || j != d.column; i += deltaL, j += deltaC){
            if (gameField[i][j]){
                return false;
            }
        }
        if (!hasFriendlyPiece(gameField, s, d)){
            return true;
        }
        return false;
    }

    vector<Square> Rook::getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square s,
                                          const unordered_map<string, Square> &kingsPosition,
                                          const vector<Square> &checkInfo, bool forKing, const string &whoseTurn,
                                          const string &lastMove) const{
        vector<Square> possibleMoves;
        vector<Square> realMoves;
        int line, column;

        int directions[4][2] = {{-1, 0},
                                {1,  0},
                                {0,  -1},
                                {0,  1}};

        for (auto &direction: directions){
            getLineMoves(gameField, direction, possibleMoves, s, forKing, color);
        }

        for (Square possibleMove: possibleMoves){
            if (whoseTurn_move_checkup(whoseTurn, forKing) &&
                onCheck_move_checkup(gameField, kingsPosition, s, possibleMove, checkInfo) &&
                onAttackLine_move_checkup(gameField, kingsPosition, s, possibleMove)){
                realMoves.push_back(possibleMove);
            }
        }
        return realMoves;
    }

    bool Queen::checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square s, Square d,
                          const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                          bool forKing, const string &lastMove) const{
        if (s.line >= 8 || s.column >= 8){
            return false;
        }
        if (s.line < 0 || s.column < 0){
            return false;
        }

        if (abs(s.line - d.line) != abs(s.column - d.column) || (s.line - d.line != 0 && s.column - d.column != 0)){
            return false;
        }

        int deltaL;
        int deltaC;

        if (d.line == s.line){
            deltaL = 0;
            deltaC = (d.column > s.column) ? 1 : -1;
        } else if (d.column == s.column){
            deltaL = (d.line > s.line) ? 1 : -1;
            deltaC = 0;
        } else {
            deltaL = (d.line > s.line) ? 1 : -1;
            deltaC = (d.column > s.column) ? 1 : -1;
        }

        for (int i = s.line + deltaL, j = s.column + deltaC; i != d.line || j != d.column; i += deltaL, j += deltaC){
            if (gameField[i][j]){
                return false;
            }
        }
        if (!hasFriendlyPiece(gameField, s, d)){
            return true;
        }
        return false;
    }

    vector<Square> Queen::getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square s,
                                           const unordered_map<string, Square> &kingsPosition,
                                           const vector<Square> &checkInfo, bool forKing, const string &whoseTurn,
                                           const string &lastMove) const{
        vector<Square> possibleMoves;
        vector<Square> realMoves;
        int line, column;

        int bishopDirections[4][2] = {{-1, -1},
                                      {-1, 1},
                                      {1,  -1},
                                      {1,  1}};

        int rookDirections[4][2] = {{-1, 0},
                                    {1,  0},
                                    {0,  -1},
                                    {0,  1}};

        for (auto &direction: bishopDirections){
            getLineMoves(gameField, direction, possibleMoves, s, forKing, color);
        }

        for (auto &direction: rookDirections){
            getLineMoves(gameField, direction, possibleMoves, s, forKing, color);
        }

        for (Square possibleMove: possibleMoves){
            if (whoseTurn_move_checkup(whoseTurn, forKing) &&
                onCheck_move_checkup(gameField, kingsPosition, s, possibleMove, checkInfo) &&
                onAttackLine_move_checkup(gameField, kingsPosition, s, possibleMove)){
                realMoves.push_back(possibleMove);
            }
        }
        return realMoves;
    }

    bool King::squareUnderNoAttack(const vector<vector<shared_ptr<Figure>>> &gameField, Square d,
                                   const unordered_map<string, Square> &kingsPosition,
                                   const vector<Square> &checkInfo, const string &lastMove) const{
        vector<Square> beatenSquares{};

        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (gameField[i][j] && gameField[i][j]->getColor() != color){
                    if (gameField[i][j]->getName() == "king"){
                        for (int x = -1; x <= 1; x++){
                            for (int y = -1; y <= 1; y++){
                                Square k{i + x, j + y};
                                beatenSquares.push_back(k);
                            }
                        }
                    } else {
                        beatenSquares = gameField[i][j]->getPossibleMoves(gameField, Square{i, j}, kingsPosition,
                                                                          checkInfo, true, "", lastMove);
                    }
                    if (find(beatenSquares.begin(), beatenSquares.end(), d) != beatenSquares.end()){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool King::checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square s, Square d,
                         const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                         bool forKing, const string &lastMove) const{
        if (s.line >= 8 || s.column >= 8 || d.line >= 8 || d.column >= 8){
            return false;
        }
        if (s.line < 0 || s.column < 0 || d.line < 0 || d.column < 0){
            return false;
        }
        if (s == d){
            return false;
        }
        if (abs(s.line - d.line) > 1 || abs(s.column - d.column) > 1){
            return false;
        }
        if (hasFriendlyPiece(gameField, s, d)){
            return false;
        }

        return squareUnderNoAttack(gameField, d, kingsPosition, checkInfo, lastMove);
    }

    bool King::castleCheckMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square s, Square d,
                               const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                               const string &lastMove) const{
        if (s.line >= 8 || s.column != 4 || d.line >= 8 || d.column >= 8){
            return false;
        }
        if (s.line < 0 || d.line < 0 || d.column < 0){
            return false;
        }
        if (s.line - d.line != 0 || abs(s.column - d.column) != 2){
            return false;
        }
        if (gameField[d.line][d.column]){
            return false;
        }
        if (!checkInfo.empty()){
            return false;
        }

        if (!hasMoved() && squareUnderNoAttack(gameField, d, kingsPosition, checkInfo, lastMove)){
            int dC;
            Square onCastlePath;

            if (d.column - s.column == 2){
                dC = 1;
                onCastlePath = Square{s.line, s.column + 1};
            } else if (d.column - s.column == -2){
                dC = -2;
                onCastlePath = Square{s.line, s.column - 1};
                if (gameField[s.line][s.column - 1]){
                    return false;
                }
            }

            if (gameField[onCastlePath.line][onCastlePath.column] ||
                !squareUnderNoAttack(gameField, onCastlePath, kingsPosition, checkInfo, lastMove)){
                return false;
            }

            shared_ptr<Figure> figure = gameField[s.line][d.column + dC];
            if (figure && figure->getName() == "rook"){
                shared_ptr<Rook> rook = std::dynamic_pointer_cast<Rook>(figure);
                if (!rook->hasMoved()){
                    return true;
                }
            }
        }
        return false;
    }

    vector<Square> King::getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square s,
                                          const unordered_map<string, Square> &kingsPosition,
                                          const vector<Square> &checkInfo, bool forKing, const string &whoseTurn,
                                          const string &lastMove) const{
        vector<Square> realMoves;

        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                Square d{s.line + i, s.column + j};
                if (whoseTurn_move_checkup(whoseTurn, forKing) &&
                    checkMove(gameField, s, d, kingsPosition, checkInfo, forKing, lastMove)){
                    realMoves.push_back(d);
                }
            }
        }
        Square oo = Square{s.line, s.column + 2};
        Square ooo = Square{s.line, s.column - 2};
        if (whoseTurn_move_checkup(whoseTurn, forKing)){
            if (castleCheckMove(gameField, s, oo, kingsPosition, checkInfo, lastMove)){
                realMoves.push_back(oo);
            }
            if (castleCheckMove(gameField, s, ooo, kingsPosition, checkInfo, lastMove)){
                realMoves.push_back(ooo);
            }
        }
        return realMoves;
    }
}