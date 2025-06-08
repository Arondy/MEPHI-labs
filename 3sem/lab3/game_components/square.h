#ifndef SQUARE_H
#define SQUARE_H

struct Square {
    int line;
    int column;
    Square() = default;
    Square(int line, int column) : line{line}, column{column}{};
    bool operator==(const Square &second) const {
        return line == second.line && column == second.column;
    }
    ~Square() = default;
};

namespace std {
    template <>
    struct hash<Square> {
        std::size_t operator()(const Square& square) const {
            return std::hash<int>()(square.line) ^ std::hash<int>()(square.column);
        }
    };
}

#endif