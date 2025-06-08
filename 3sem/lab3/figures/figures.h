#ifndef FIGURES_H
#define FIGURES_H

#include <iostream>
#include <algorithm>
#include <format>
#include <vector>
#include "../vector/vector.h"
#include <memory>
#include <unordered_map>
#include "../game_components/square.h"
#include "../game_components/history.h"

namespace Figures {
    using std::string, MyVec::vector, std::shared_ptr, std::unordered_map, std::pair, std::min, std::max, std::format, std::find;
    using Components::numToColumn;

    static const vector<string> figureNames = {
            "pawn",
            "knight",
            "bishop",
            "rook",
            "queen",
            "king"
    };

    class Figure {
    protected:
        string color;
        string name;
        [[nodiscard]] bool onCheck_move_checkup(const vector<vector<shared_ptr<Figure>>> &gameField,
                                                const unordered_map<string, Square> &kingsPosition, Square s,
                                                Square d, const vector<Square> &checkInfo) const;
        [[nodiscard]] bool onAttackLine_move_checkup(const vector<vector<shared_ptr<Figure>>> &gameField,
                                                     const unordered_map<string, Square> &kingsPosition,
                                                     Square s, Square d) const;
        [[nodiscard]] bool whoseTurn_move_checkup(const string &whoseTurn, bool forKing) const;
    public:
        /// Конструктор по умолчанию
        Figure() = default;

        /// Конструктор фигуры по цвету и названию
        /// \param color Цвет фигуры
        /// \param name Название фигуры
        Figure(const string &color, const string &name);

        /// Получение значения цвета фигуры
        /// \return цвет фигуры
        [[nodiscard]] string getColor() const{ return color; };

        /// Получение значения названия фигуры
        /// \return название фигуры
        [[nodiscard]] string getName() const{ return name; };

        /// Проверка хода на корректность
        /// \param gameField Игровое поле (матрица указателей)
        /// \param startingSquare Клетка, на которой находится фигура
        /// \param destSquare Клетка, на которую хотят поместить фигуру
        /// \param kingsPosition Позиция королей
        /// \param checkInfo Вектор клеток, с которых есть атака на короля
        /// \param forKing true, если эта функция вызывана при расчете возможных ходов короля
        /// \param lastMove Последний совершенный ход
        /// \return true, если ход возможен; false в ином случае
        [[nodiscard]] virtual bool
        checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare, Square destSquare,
                  const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                  bool forKing, const string &lastMove) const = 0;

        /// Получение вектора всех возможных ходов фигуры с заданной позиции
        /// \param gameField Игровое поле (матрица указателей)
        /// \param startingSquare Клетка, на которой находится фигура
        /// \param kingsPosition Позиция королей
        /// \param checkInfo Вектор клеток, с которых есть атака на короля
        /// \param forKing true, если эта функция вызывана при расчете возможных ходов короля
        /// \param lastMove Последний совершенный ход
        /// \return Вектор всех возможных ходов фигуры с заданной позиции
        [[nodiscard]] virtual vector<Square>
        getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare,
                         const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                         bool forKing, const string &whoseTurn, const string &lastMove) const = 0;

        /// Деструктор по умолчанию
        ~Figure() = default;
    };

    class Pawn : public Figure {
    public:
        explicit Pawn(const string &color) : Figure(color, "pawn"){};
        [[nodiscard]] bool
        checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare, Square destSquare,
                  const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                  bool forKing, const string &lastMove) const override;
        [[nodiscard]] vector<Square>
        getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare,
                         const unordered_map<string, Square> &kingsPosition,
                         const vector<Square> &checkInfo,
                         bool forKing, const string &whoseTurn, const string &lastMove) const override;
    };

    class Knight : public Figure {
    public:
        explicit Knight(const string &color) : Figure(color, "knight"){};
        [[nodiscard]] bool
        checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare, Square destSquare,
                  const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                  bool forKing, const string &lastMove) const override;
        [[nodiscard]] vector<Square>
        getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare,
                         const unordered_map<string, Square> &kingsPosition,
                         const vector<Square> &checkInfo,
                         bool forKing, const string &whoseTurn, const string &lastMove) const override;
    };

    class Bishop : public Figure {
    public:
        explicit Bishop(const string &color) : Figure(color, "bishop"){};
        [[nodiscard]] bool
        checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare, Square destSquare,
                  const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                  bool forKing, const string &lastMove) const override;
        [[nodiscard]] vector<Square>
        getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare,
                         const unordered_map<string, Square> &kingsPosition,
                         const vector<Square> &checkInfo,
                         bool forKing, const string &whoseTurn, const string &lastMove) const override;
    };

    class Rook : public Figure {
    private:
        bool moved = false;
    public:
        explicit Rook(const string &color) : Figure(color, "rook"){};
        [[nodiscard]] bool
        checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare, Square destSquare,
                  const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                  bool forKing, const string &lastMove) const override;
        [[nodiscard]] vector<Square>
        getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare,
                         const unordered_map<string, Square> &kingsPosition,
                         const vector<Square> &checkInfo,
                         bool forKing, const string &whoseTurn, const string &lastMove) const override;
        [[nodiscard]] bool hasMoved() const{ return moved; };
        void setMovedTrue(){ moved = true; };
    };

    class Queen : public Figure {
    public:
        explicit Queen(const string &color) : Figure(color, "queen"){};
        [[nodiscard]] bool
        checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare, Square destSquare,
                  const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                  bool forKing, const string &lastMove) const override;
        [[nodiscard]] vector<Square>
        getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare,
                         const unordered_map<string, Square> &kingsPosition,
                         const vector<Square> &checkInfo,
                         bool forKing, const string &whoseTurn, const string &lastMove) const override;
    };

    class King : public Figure {
    private:
        bool moved = false;
    public:
        explicit King(const string &color) : Figure(color, "king"){};
        [[nodiscard]] bool
        checkMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare, Square destSquare,
                  const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                  bool forKing, const string &lastMove) const override;
        [[nodiscard]] bool
        squareUnderNoAttack(const vector<vector<shared_ptr<Figure>>> &gameField, Square destSquare,
                            const unordered_map<string, Square> &kingsPosition,
                            const vector<Square> &checkInfo, const string &lastMove) const;
        [[nodiscard]] bool
        castleCheckMove(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare, Square destSquare,
                        const unordered_map<string, Square> &kingsPosition, const vector<Square> &checkInfo,
                        const string &lastMove) const;
        [[nodiscard]] vector<Square>
        getPossibleMoves(const vector<vector<shared_ptr<Figure>>> &gameField, Square startingSquare,
                         const unordered_map<string, Square> &kingsPosition,
                         const vector<Square> &checkInfo,
                         bool forKing, const string &whoseTurn, const string &lastMove) const override;
        [[nodiscard]] bool hasMoved() const{ return moved; };
        void setMovedTrue(){ moved = true; };
    };

    /// Проверяет, есть ли дружественная фигура в указанной клетке
    /// \param gameField Игровое поле (матрица указателей)
    /// \param startingSquare Клетка, в которой находится фигура
    /// \param destSquare Клетка, в которой проверяется наличие дружественной фигуры
    /// \return true, если дружественная фигура есть в указанной клетке; false в ином случае
    bool hasFriendlyPiece(const vector<vector<shared_ptr<Figure>>> &gameField, const Square &startingSquare, const Square &destSquare);

    /// Проверяет, есть ли вражеская фигура в указанной клетке
    /// \param gameField Игровое поле (матрица указателей)
    /// \param startingSquare Клетка, в которой находится фигура
    /// \param destSquare Клетка, в которой проверяется наличие вражеской фигуры
    /// \return true, если вражеская фигура есть в указанной клетке; false в ином случае
    bool hasEnemyPiece(vector<vector<shared_ptr<Figure>>> gameField, const Square &startingSquare, const Square &destSquare);

    /// Получение ходов "по линии" для слона/ладьи/ферзя через ссылку на массив
    /// \param gameField Игровое поле (матрица указателей)
    /// \param direction Массив направлений (изменений line при изменении column)
    /// \param possibleMoves Вектор возможных ходов, в который записывается результат
    /// \param startingSquare Клетка, в которой находится фигура
    /// \param forKing true, если эта функция вызывана при расчете возможных ходов короля
    /// \param color Цвет фигуры
    void getLineMoves(const vector<vector<shared_ptr<Figure>>> &gameField, const int direction[2],
                      vector<Square> &possibleMoves, Square startingSquare, bool forKing, const string &color);
}

#endif