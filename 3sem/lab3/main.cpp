#include <iostream>
#include <memory>
#include <vector>
#include "vector/vector.h"
#include "game_graphics/gameRender.h"

using std::cin, std::cout, std::cerr, std::endl, std::shared_ptr, MyVec::vector;

int main(){
    try {
        GameRender gr{};
        gr.getGameState()->loadGame("../game_state/saves/test.json");
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