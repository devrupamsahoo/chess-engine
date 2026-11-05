//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_GAMEMANAGER_H
#define CHESS_ENGINE_GAMEMANAGER_H

#include "chess/core/GameSession.h"
#include <string>

class GameManager {
private:
    // manages the current game state and move history
    GameSession session;

    // master flag for game loop
    bool gameIsRunning;

    // displays the current player's turn
    void displayTurn();

    // takes input from player
    static bool getInput(const std::string &prompt, std::string &input);

    // processes input validation, move validation, moving pieces
    // returns true if the current player's move is successfully completed
    bool processTurn();

    // display the commands available during the game
    static void printControls();

public:
    // ctor
    GameManager();

    // runs the game
    void run();

    // dtor
    ~GameManager() = default;
};


#endif //CHESS_ENGINE_GAMEMANAGER_H
