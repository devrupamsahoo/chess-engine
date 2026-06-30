//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_GAMEMANAGER_H
#define CHESS_ENGINE_GAMEMANAGER_H

#include "ChessBoard.h"
#include <string>

class GameManager {
private:
    // creates chessboard object
    ChessBoard board;

    // master flag for game loop
    bool gameIsRunning;

    // displays the current player's turn
    void displayTurn();

    // takes input from player
    static std::string getInput(const std::string &prompt);

    // processes input validation, move validation, moving pieces
    // returns true if the current player's move is successfully completed
    bool processTurn();

public:
    // ctor
    GameManager();

    // runs the game
    void run();

    // dtor
    ~GameManager() = default;
};


#endif //CHESS_ENGINE_GAMEMANAGER_H
