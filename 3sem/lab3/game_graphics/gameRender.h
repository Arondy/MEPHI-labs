#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include "../vector/vector.h"
#include <vector>
#include <unordered_map>
#include <mutex>
#include "../game_state/GameState.h"

# define cast static_cast

const int figureNum = 12;
const int windowXStartingSize = 1408;   // Оптимально - должно быть подобрано, чтобы Y было кратно 8
const float xAspectRatio = 16.0;
const float yAspectRatio = 9.0;
const float aspectRatio = xAspectRatio / yAspectRatio;
const float offsetRatio = (1 - yAspectRatio / xAspectRatio) / 2 ;
const int textureSize = 256;  // Обязательно w = h
static float squareSize;

using namespace sf;
using namespace std::chrono;
using tgui::TextArea, tgui::Gui;
using std::string, std::pair, std::unordered_map, std::shared_ptr, std::thread, std::mutex,
std::lock_guard, std::make_shared, std::format, std::find, MyVec::vector;
using Components::Player, State::GameState, Figures::Figure;

class GameRender {
private:
    unordered_map<string, vector<Texture>> textures;
    unordered_map<string, vector<Sprite>> sprites;
    unordered_map<string, Font> fonts;
    unordered_map<string, Text> texts;
    unordered_map<string, shared_ptr<thread>> threads;
    bool clockFlag = true;
    mutex resultMutex;
    shared_ptr<GameState> gameState;
    RenderWindow window;
    Gui gui;
    void onResize_scale(const Vector2u &newWinSize, const Vector2u &prevWinSize);
    void onResize_change_position(const Vector2u &newWinSize, const Vector2u &prevWinSize);
    void resize_window(Vector2u &prevWinSize);
    void load_textures();
    void load_font();
    void set_position_from_square(Sprite &sprite, const Square &square);
    void create_round_rect();
    void create_result_string(const string &gameResult, bool turn);
    void start_clock_thread();
    void place_figures_at_start();
    void place_and_scale_board_at_start();
    void scale_figures_at_start();
    void create_sprite_out_of_figure(const shared_ptr<Figure> &figure);
    void place_sprite(Sprite &sprite, int i, int j);
    void create_hints(const Sprite &figure);
    void place_hints(const Vector2u &currWinSize, const vector<Square> &moves, const Square &frFigure);
    void create_check_hint(const Square &king);
    void create_and_place_clock_text();
    void update_clocks_text();
    void create_and_place_players_names();
    void create_and_place_history();
    void update_history_text(string text);
    vector<Square> select_friendly_figure(Vector2u &currWinSize, Square &frFigure, int &frIndex, bool &isMakingMove);
    void enPassant_check(const Vector2u &currWinSize, const Square &frFigure, int &frIndex, int fX, int fY);
    void onPromote_texture_change(const Square &frFigure, const int &frIndex, int fY);
    void castle_check(const Vector2u &currWinSize, const Square &frFigure, int fX, int mY);
    void make_move(Vector2u &currWinSize, Square &frFigure, int frIndex, vector<Square> &possibleMoves);
    void on_game_start();
    void draw();
public:
    GameRender();
    [[nodiscard]] shared_ptr<GameState> getGameState() const { return gameState; };
    void render();
    ~GameRender();
};

#endif