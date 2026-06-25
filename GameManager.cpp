//
// Created by sahoo on 23-06-2026.
//

#include "GameManager.h"
#include <iostream>
#include "MoveValidator.h"
#include "ChessBoard.h"
#include "Utils.h"

GameManager::GameManager() {
    gameIsRunning = true;
}

// runs the game
void GameManager::run() {
    board.printBoard();

    // game loop
    while (gameIsRunning) {
        printTurn();

        bool moveCompleted = processTurn();

        if (moveCompleted && gameIsRunning) {
            // clears terminal
            system("cls");

            board.printBoard();

            board.switchTurn();
        }
    }

    std::cout << "Game exited!!" << std::endl;
}

// processes input validation, move validation, moving pieces
bool GameManager::processTurn() {
    std::string startPosInput;
    int startPos;

    // input loop for valid input format and valid piece selection
    while (true) {
        startPosInput = getInput("Enter piece position ('x' to exit): ");

        // exits the game
        if (startPosInput == "x") {
            gameIsRunning = false;
            return false;
        }

        // validates user input format
        if (!MoveValidator::isValidPosition(startPosInput)) {
            continue;
        }

        startPos = Utils::coordinateToPosition(startPosInput);

        if (!board.isValidPiece(board.isWhiteTurn(), startPos)) {
            std::cout << "Invalid piece selected!" << std::endl;
            continue;
        }

        break;
    }

    Piece selectedPiece = board.getPieceAt(startPos);
    std::cout << "Piece selected: " << pieceMap[selectedPiece] << std::endl;


    std::string endPosInput;
    int endPos;

    // second input loop for validating input format and legal move
    while (true) {
        endPosInput = getInput("Enter piece position ('x' to exit or 'b' to unselect): ");

        // exits the game
        if (endPosInput == "x") {
            gameIsRunning = false;
            return false;
        }

        // lets player select another piece
        if (endPosInput == "b") {
            std::cout << "Piece unselected!" << std::endl;
            return false;
        }

        // validates user input format
        if (!MoveValidator::isValidPosition(endPosInput)) {
            continue;
        }

        endPos = Utils::coordinateToPosition(endPosInput);

        // TODO: check if move is legal

        break;
    }

    std::cout << "Moving piece from "<< startPos << " to " << endPos << std::endl;

    // TODO: move the piece

    return true;
}

// takes input from player
std::string GameManager::getInput(const std::string &prompt) {
    std::string input;
    std::cout << prompt;
    std::cin >> input;

    return input;
}

// prints player's turn
void GameManager::printTurn() {
    if (board.isWhiteTurn()) {
        std::cout << "White's Turn" << std::endl;
    } else {
        std::cout << "Black's Turn" << std::endl;
    }
}