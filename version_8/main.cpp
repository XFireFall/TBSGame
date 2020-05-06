#include "Lib/frontend/game.h"
#include <iostream>

int main() {
    try {
        Game game{};
        game.run();
    } catch (std::exception &e) {
        std::cerr << "Error : " << e.what() << std::endl;
    }
}
