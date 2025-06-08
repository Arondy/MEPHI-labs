#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <fstream>
#include <unordered_map>
#include "../json_lib/json.hpp"
#include "../figures/figures.h"
#include "../game_components/clock.h"
#include "../game_components/history.h"
#include "../game_components/player.h"


namespace State {
    using namespace std::chrono;
    using std::string, std::pair, std::unordered_map, std::shared_ptr, std::thread, std::mutex, std::make_shared, std::lock_guard;
    using Figures::Figure, Components::Player, Components::Clock, Components::GameHistory, MyVec::vector;
    using json = nlohmann::json;

    const string basic_game_file = "basic_game_start.json";
    static std::mutex exCheckMutex;
    static std::mutex clockFlagMutex;
    class GameState {
    private:
        vector<vector<shared_ptr<Figure>>> gameField{8, vector<shared_ptr<Figure>>{8}};
        pair<Player, Player> players;
        Clock clock;
        GameHistory history;
        string whoseTurn;
        unordered_map<string, Square> kingsPosition;
        vector<Square> checkInfo;
        unordered_map<string, unordered_map<string, int>> eloChange;
        int clockRes = 0;
        bool clockFlag = true;
        unordered_map<string, shared_ptr<thread>> threads;
        void fillEloChange();
        void examineCheckThread(vector<Square> &threats, int start, int end);
        void examineCheck();
        void specialMoves(const Square &currS, const Square &newS, const string &name, const shared_ptr<Figure> &currFigure,
                     bool &promote);
        void basicMove(const Square &currS, const Square &newS);
        void updateClock();
        void updateElo(const string &state);
        [[nodiscard]] bool isCheck() const;
        [[nodiscard]] bool noMoveAvailable() const;
        void startClockThread();
    public:
        GameState() = default;
        [[nodiscard]] const vector<vector<shared_ptr<Figure>>> &getField() const{ return gameField; };
        [[nodiscard]] const pair<Player, Player> &getPlayers() const{ return players; };
        [[nodiscard]] const Clock &getClock() const{ return clock; };
        [[nodiscard]] const GameHistory &getHistory() const{ return history; };
        [[nodiscard]] const string &getWhoseTurn() const{ return whoseTurn; };
        [[nodiscard]] const unordered_map<string, Square> &getKingsPosition() const{ return kingsPosition; };
        [[nodiscard]] const vector<Square> &getCheckInfo() const{ return checkInfo; };
        [[nodiscard]] const int &getClockRes() const;
        [[nodiscard]] unordered_map<string, unordered_map<string, int>> getEloChange() const{ return eloChange; };
        [[nodiscard]] string checkGameState() const;
        void stopClock();
        void flipTurn();
        bool gameMove(const Square &currS, const Square &newS, string &state);
        void saveGame(const string &filename) const;
        void loadGame(const string &filename);
        ~GameState();
    };

    shared_ptr<Figure> createFigureByName(const string &name, const string &color);
}

#endif