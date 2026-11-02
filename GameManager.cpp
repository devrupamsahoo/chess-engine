//
// Created by sahoo on 23-06-2026.
//

#include "GameManager.h"
#include <iostream>
#include <cctype>
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
    std::string input;
    int startPos;
    int endPos;
    char promotedPiece = 'q'; // default to Queen if not specified

    // input loop for valid input format and valid piece selection
    while(true) {
        input = getInput("Enter move ('x' to reset): ");

        // resets the game
        if (input == "x") {
            board = ChessBoard(); // overwrites the current board with a fresh one
            system("cls");
            std::cout << "Game reset back to starting position!" << std::endl;
            board.printBoard();
            return false;         // returns false so the turn doesn't switch
        }

        // validates user input format
        if (!MoveValidator::isValidPosition(input)) {
            continue;
        }

        // extract the promotion character if provided
        if (input.length() == 5) {
            promotedPiece = input[4];
        }

        // use exactly 2 characters for coordinates
        startPos = Utils::coordinateToPosition(input.substr(0, 2));

        if (!board.isValidPiece(board.isWhiteTurn(), startPos)) {
            std::cout << "Invalid piece selected!" << std::endl;
            continue;
        }

        // use exactly 2 characters starting at index 2
        endPos = Utils::coordinateToPosition(input.substr(2, 2));

        // checks if move is legal
        if (!MoveValidator::isValidMove(board, startPos, endPos)) {
            continue;
        }

        break;
    }

    Piece pieceMoved = board.getPieceAt(startPos);

    // convert the promotion choice into the piece type for the moving side
    Piece promotionPiece = noPiece;

    // convert the promotion choice to the moving side's piece type for the move record
    if ((pieceMoved == whitePawn || pieceMoved == blackPawn) && PawnValidator::isPawnPromotion(endPos, board.isWhiteTurn())) {
        const bool white = pieceMoved == whitePawn;
        const char choice = static_cast<char>(std::tolower(static_cast<unsigned char>(promotedPiece)));

        switch (choice) {
            case 'q':
                promotionPiece = white ? whiteQueen : blackQueen;
                break;
            case 'r':
                promotionPiece = white ? whiteRook : blackRook;
                break;
            case 'b':
                promotionPiece = white ? whiteBishop : blackBishop;
                break;
            case 'n':
                promotionPiece = white ? whiteKnight : blackKnight;
                break;
        }
    }

    bool isCastling = (pieceMoved == whiteKing || pieceMoved == blackKing) &&
                      (std::abs((startPos & 7) - (endPos & 7)) == 2);

    // castling moves the king and rook together, so execute it separately from a normal move
    if (isCastling) {
        if (!board.executeCastle(startPos, endPos, board.isWhiteTurn())) {
            return false;
        }
    } else {
        // moves the piece
        board.makeMove(startPos, endPos, promotionPiece);
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
