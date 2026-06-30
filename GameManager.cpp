//
// Created by sahoo on 23-06-2026.
//

#include "GameManager.h"
#include <iostream>
#include "MoveValidator.h"
#include "ChessBoard.h"
#include "KingValidator.h"
#include "PawnValidator.h"
#include "Utils.h"

GameManager::GameManager() {
    gameIsRunning = true;
}

// runs the game
void GameManager::run() {
    board.printBoard();

    // game loop
    while (gameIsRunning) {
        displayTurn();

        bool moveCompleted = processTurn();

        if (moveCompleted && gameIsRunning) {
            // clears terminal
            system("cls");

            // next player (opponent) after the move
            bool isNextPlayerWhite = !board.isWhiteTurn();

            // notify if the opponent's king is in check
            if (KingValidator::isKingInCheck(board, isNextPlayerWhite)) {
                std::cout << std::endl << (isNextPlayerWhite ? "White" : "Black") << " king is in check!" << std::endl;
            }

            board.printBoard();

            board.switchTurn();
        }
    }

    std::cout << "Game exited!!" << std::endl;
}

// processes input validation, move validation, moving pieces
// returns true if the current player's move is successfully completed
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
    std::cout << "Piece selected: " << pieceToCharMap[selectedPiece] << std::endl;


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

        // checks if move is legal
        if (!MoveValidator::isValidMove(board, startPos, endPos)) {
            continue;
        }

        break;
    }

    std::cout << "Moving piece from "<< startPos << " to " << endPos << std::endl;

    Piece pieceMoved = board.getPieceAt(startPos);

    bool isCastling = (pieceMoved == whiteKing || pieceMoved == blackKing) &&
                      (std::abs((startPos & 7) - (endPos & 7)) == 2);

    if (isCastling) {
        board.executeCastle(startPos, endPos, board.isWhiteTurn());
    } else {
        // moves the piece
        board.makeMove(startPos, endPos);
    }

    // handles pawn promotion after move
    if (pieceMoved == whitePawn || pieceMoved == blackPawn) {

        int rank = endPos >> 3;

        // white pawn reaches 8th rank or black pawn reaches 1st rank
        if (rank == 7 || rank == 0) {
            PawnValidator::promotePawn(board, endPos, (pieceMoved == whitePawn));
        }
    }

    return true;
}

// takes input from player
std::string GameManager::getInput(const std::string &prompt) {
    std::string input;
    std::cout << prompt;
    std::cin >> input;

    return input;
}

// displays the current player's turn
void GameManager::displayTurn() {

    if (board.isWhiteTurn()) {
        std::cout << "White's Turn" << std::endl;
    } else {
        std::cout << "Black's Turn" << std::endl;
    }
}
