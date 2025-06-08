#include <iostream>
#include "game_graphics/gameRender.h"

using std::cin, std::cout, std::cerr, std::endl;

int main(){
    try {
        GameRender gr{};
        gr.getGameState()->loadGame("saves/basic_game_start.json");
        gr.render();
    }
    catch (const std::bad_alloc &ba){
        cerr << "Not enough memory" << endl;
        return 1;
    }
    catch (const std::exception &e){
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}