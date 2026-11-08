//
// Created by sahoo on 23-06-2026.
//

#include "../include/GameManager.h"
#include <iostream>
#include <cctype>
#include "chess/core/ChessBoard.h"
#include "chess/move/KingValidator.h"
#include "chess/move/PawnValidator.h"
#include "chess/core/Utils.h"
#include "chess/notation/FEN.h"
#include <fstream>
#include "../include/DisplayBoard.h"
#include <algorithm>
#include <filesystem>

namespace {
    // validates terminal move input such as e2e4 or e7e8q
    bool isValidMoveInput(const std::string &input) {
        if (input.length() != 4 && input.length() != 5) {
            return false;
        }

        if (input[0] < 'a' || input[0] > 'h' ||
            input[1] < '1' || input[1] > '8' ||
            input[2] < 'a' || input[2] > 'h' ||
            input[3] < '1' || input[3] > '8') {
            return false;
        }

        if (input.length() == 4) {
            return true;
        }

        const char promotion = static_cast<char>(
            std::tolower(static_cast<unsigned char>(input[4])));

        return promotion == 'q' || promotion == 'r' ||
               promotion == 'b' || promotion == 'n';
    }
}

GameManager::GameManager() {
    gameIsRunning = true;
}

// runs the game
void GameManager::run() {
    printControls();

    DisplayBoard::printBoard(session.board());

    std::cout << session.currentFen() << std::endl;

    // game loop
    while (gameIsRunning) {
        displayTurn();

        bool moveCompleted = processTurn();

        // commands that modify the position without completing a normal turn return false here
        if (moveCompleted && gameIsRunning) {
            // clears terminal
            system("cls");

            printControls();

            // next player (opponent) after the move
            bool isNextPlayerWhite = session.board().isWhiteTurn();

            // the winner is the opponent of the side to move
            const std::string winner = isNextPlayerWhite ? "Black" : "White";

            // reads the current game status after the move
            const GameStatus status =
                    session.gameStatus();

            // reports check, checkmate, stalemate, and draw states
            switch (status) {
                case GameStatus::check:
                    std::cout << (isNextPlayerWhite ? "White" : "Black") << " king is in check!" << std::endl;
                    break;

                case GameStatus::checkmate:
                    std::cout << "Checkmate! " << winner << " wins!" << std::endl;

                    // stops the game loop after checkmate
                    gameIsRunning = false;
                    break;

                case GameStatus::stalemate:
                    std::cout << "Stalemate! Draw." << std::endl;

                    // stops the game loop after stalemate
                    gameIsRunning = false;
                    break;

                case GameStatus::draw:
                    std::cout << "Draw!" << std::endl;

                    // stops the game loop after a draw
                    gameIsRunning = false;
                    break;

                case GameStatus::ongoing:
                    break;
            }

            DisplayBoard::printBoard(session.board());

            std::cout << session.currentFen() << std::endl;
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
    while (true) {
        if (!getInput("Enter your move: ", input)) {
            gameIsRunning = false;
            return false;
        }

        // normalize commands, coordinates, and promotion choices to lowercase
        std::transform(input.begin(), input.end(), input.begin(), [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });

        // reset the default promotion choice for each newly entered move
        promotedPiece = 'q';

        // undo move
        if (input == "u") {
            if (!session.undo()) {
                std::cout << "No moves to undo!" << std::endl;
                continue;
            }

            system("cls");
            printControls();
            DisplayBoard::printBoard(session.board());
            std::cout << session.currentFen() << std::endl;
            return false;
        }

        // redo move
        if (input == "r") {
            if (!session.redo()) {
                std::cout << "No moves to redo!" << std::endl;
                continue;
            }

            system("cls");
            printControls();
            DisplayBoard::printBoard(session.board());
            std::cout << session.currentFen() << std::endl;
            return false;
        }

        // quit the game
        if (input == "q") {
            gameIsRunning = false;
            return false;
        }

        // resets the game
        if (input == "x") {
            session.reset();

            system("cls");
            std::cout << "Game reset back to starting position!" << std::endl;
            printControls();
            DisplayBoard::printBoard(session.board());
            std::cout << session.currentFen() << std::endl;
            return false; // returns false so the turn doesn't switch
        }

        // load a position from FEN and start a new match history from it
        if (input == "f") {
            std::string fen;
            std::cout << "Enter FEN: ";

            if (!std::getline(std::cin >> std::ws, fen)) {
                gameIsRunning = false;
                return false;
            }

            if (!session.loadFen(fen)) {
                std::cout << "Invalid FEN." << std::endl;
                continue;
            }


            system("cls");
            printControls();
            DisplayBoard::printBoard(session.board());
            std::cout << session.currentFen() << std::endl;
            return false;
        }

        // write each recorded position in the current match history to a file
        if (input == "e") {
            const std::filesystem::path fenPath = std::filesystem::path(CHESS_ENGINE_PROJECT_DIR) / "match_fens.txt";
            std::ofstream fenFile(fenPath);

            if (!fenFile) {
                std::cout << "Could not open match_fens.txt for writing." << std::endl;
                continue;
            }

            // export snapshots in move order, including the current position
            for (const std::string &fen: session.fenHistory()) {
                fenFile << fen << '\n';
            }

            fenFile.close();

            if (!fenFile) {
                std::cout << "Could not write the complete FEN history." << std::endl;
                continue;
            }

            std::cout << "FEN history exported to match_fens.txt." << std::endl;
            continue;
        }

        // validates user input format
        if (!isValidMoveInput(input)) {
            continue;
        }

        // extract the promotion character if provided
        if (input.length() == 5) {
            promotedPiece = input[4];
        }

        // use exactly 2 characters for coordinates
        startPos = Utils::coordinateToPosition(input.substr(0, 2));
        endPos = Utils::coordinateToPosition(input.substr(2, 2));

        Piece selectedPiece = session.board().getPieceAt(startPos);

        // promotion requires a pawn and a destination on its final rank
        bool isPromotionMove =
                (selectedPiece == whitePawn || selectedPiece == blackPawn) &&
                PawnValidator::isPawnPromotion(
                    endPos, session.board().isWhiteTurn());

        // reject promotion choices unless a pawn is reaching its last rank
        if (input.length() == 5 && !isPromotionMove) {
            std::cout << "Promotion choice is only valid when a pawn reaches the last rank." << std::endl;
            continue;
        }

        Piece promotionPiece = noPiece;

        // convert the promotion choice into the piece type for the moving side
        if (isPromotionMove) {
            const bool white = selectedPiece == whitePawn;

            switch (promotedPiece) {
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
                default: ;
            }
        }

        // let the session validate and execute the move
        if (!session.makeMove(startPos, endPos, promotionPiece)) {
            std::cout << "Invalid move!" << std::endl;
            continue;
        }

        return true;
    }
}


// takes input from player
bool GameManager::getInput(const std::string &prompt, std::string &input) {
    std::cout << prompt;

    // report stream failure so the caller can exit when input ends
    return static_cast<bool>(std::cin >> input);
}

// displays the current player's turn
void GameManager::displayTurn() {
    if (session.board().isWhiteTurn()) {
        std::cout << "White's Turn" << std::endl;
    } else {
        std::cout << "Black's Turn" << std::endl;
    }
}

// display the commands available during the game
void GameManager::printControls() {
    std::cout << std::endl;
    std::cout << "Enter 'u' to undo move" << std::endl;
    std::cout << "Enter 'r' to redo move" << std::endl;
    std::cout << "Enter 'x' to restart game" << std::endl;
    std::cout << "Enter 'q' to quit game" << std::endl;
    std::cout << "Enter 'e' export FEN from current match" << std::endl;
    std::cout << "Enter 'f' to load match from FEN" << std::endl;
    std::cout << std::endl;
}
