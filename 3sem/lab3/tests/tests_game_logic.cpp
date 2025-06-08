#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../game_state/GameState.h"

using namespace Figures;
using namespace State;
using std::cin, std::cout;

TEST_CASE("States"){
    SECTION("Check"){
        GameState state{};
        state.loadGame("../../game_state/saves/check.json");
        REQUIRE(state.checkGameState() == "check");
    }SECTION("Mate"){
        GameState state{};
        state.loadGame("../../game_state/saves/mate.json");
        REQUIRE(state.checkGameState() == "mate");
    }SECTION("Stalemate"){
        GameState state{};
        state.loadGame("../../game_state/saves/stalemate.json");
        REQUIRE(state.checkGameState() == "stalemate");
    }SECTION("Nothing state"){
        GameState state{};
        state.loadGame("../../game_state/saves/basic_game_start.json");
        REQUIRE(state.checkGameState() == "nothing");
        unordered_map<string, unordered_map<string, int>> map = state.getEloChange();
        REQUIRE(map.size() == 2);
        Square s1{6, 0}, s2{4, 0};
        string temp;
        state.gameMove(s1, s2, temp);
        state.saveGame("test_save.json");
    }
}

TEST_CASE("Game moves"){
    string temp;
    SECTION("Basic move"){
        GameState state{};
        state.loadGame("../../game_state/saves/basic_game_start.json");
        REQUIRE(state.getField()[6][1]->getName() == "pawn");
        state.gameMove(Square{6, 1}, Square{4, 1}, temp);
        REQUIRE(state.getField()[6][1] == nullptr);
        REQUIRE(state.getField()[4][1]->getName() == "pawn");
    }SECTION("Eat figure"){
        GameState state{};
        state.loadGame("../../game_state/saves/check.json");
        state.gameMove(Square{4, 4}, Square{4, 5}, temp);
        shared_ptr<Figure> rook = state.getField()[5][4];
        shared_ptr<Figure> pawn = state.getField()[1][4];
        REQUIRE(rook->getName() == "rook");
        REQUIRE(pawn->getName() == "pawn");
        REQUIRE(rook->getColor() != pawn->getColor());
        state.gameMove(Square{5, 4}, Square{1, 4}, temp);
        REQUIRE(state.getField()[5][4] == nullptr);
        REQUIRE(state.getField()[1][4]->getName() == "rook");
    }SECTION("Two moves in a row"){
        GameState state{};
        state.loadGame("../../game_state/saves/basic_game_start.json");
        state.gameMove(Square{6, 1}, Square{4, 1}, temp);
        REQUIRE(state.getField()[4][1]->getPossibleMoves(state.getField(), Square{4, 1}, state.getKingsPosition(),
                                                         state.getCheckInfo(), false,
                                                         state.getWhoseTurn(), state.getHistory().getLastMove()).empty());
        REQUIRE(state.getField()[7][1]->getPossibleMoves(state.getField(), Square{4, 1}, state.getKingsPosition(),
                                                         state.getCheckInfo(), false,
                                                         state.getWhoseTurn(), state.getHistory().getLastMove()).empty());
    }SECTION("Open king to the line attack"){
        GameState state{};
        state.loadGame("../../game_state/saves/attack_on_line.json");
        shared_ptr<Figure> king = state.getField()[2][4];
        shared_ptr<Figure> knight = state.getField()[3][4];
        shared_ptr<Figure> rook = state.getField()[5][4];
        REQUIRE(king->getName() == "king");
        REQUIRE(rook->getName() == "rook");
        REQUIRE(knight->getName() == "knight");
        REQUIRE((king->getColor() == knight->getColor() && knight->getColor() != rook->getColor()));
        REQUIRE(knight->getPossibleMoves(state.getField(), Square{3, 4}, state.getKingsPosition(),
                                         state.getCheckInfo(), false,
                                         state.getWhoseTurn(), state.getHistory().getLastMove()).empty());
    }SECTION("Move after mate"){
        GameState state{};
        state.loadGame("../../game_state/saves/mate.json");
        REQUIRE(state.checkGameState() == "mate");
        vector<vector<shared_ptr<Figure>>> gameField = state.getField();
        bool check = true;
        vector<Square> moves;
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                if (gameField[i][j] && gameField[i][j]->getColor() == state.getWhoseTurn()){
                    moves = gameField[i][j]->getPossibleMoves(gameField, Square{i, j},
                                                              state.getKingsPosition(),
                                                              state.getCheckInfo(), false,
                                                              state.getWhoseTurn(), state.getHistory().getLastMove());
                    if (!moves.empty()){
                        check = false;
                    }
                }
            }
        }
        REQUIRE(check);
    }
}