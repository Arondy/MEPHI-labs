#include <iostream>
#include <algorithm>
#include "menu.h"

using namespace Menu;

int main(){
    try {
        CardsPile pile{0};
        menu(pile);
    }
    catch (const std::bad_alloc &ba){
        std::cerr << "Not enough memory" << endl;
        return 1;
    }
    catch (const std::exception &e){
        std::cerr << e.what() << endl;
        return 1;
    }
    return 0;
}