#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <utility>

namespace Components {
    using std::string;

    class Player {
    private:
        string name;
        int elo;
    public:
        Player() = default;
        Player(string name, int elo): name{std::move(name)}, elo{elo}{};
        [[nodiscard]] string getName() const { return name; };
        [[nodiscard]] int getElo() const { return elo; };
        void changeEloByDelta(int delta){ elo += delta; };
    };
}

#endif