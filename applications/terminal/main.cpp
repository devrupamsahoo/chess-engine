#include <iostream>
#include <ostream>

#include "include/GameManager.h"

// starts the terminal chess application
int main() {
    std::cout << "WELCOME..." << std::endl;
    std::cout << std::endl;

    // creates the game manager and starts the main game loop
    GameManager game;
    game.run();


    return 0;
}
