//
// Created by sahoo on 23-06-2026.
//

#include "GameManager.h"
#include <iostream>
#include <cctype>
#include "chess/move/MoveValidator.h"
#include "chess/core/ChessBoard.h"
#include "chess/move/KingValidator.h"
#include "chess/move/PawnValidator.h"
#include "chess/core/Utils.h"
#include "chess/notation/FEN.h"
#include <fstream>
#include "DisplayBoard.h"
#include <algorithm>
#include <filesystem>

GameManager::GameManager() {
    gameIsRunning = true;

    // save the starting position as the first match snapshot
    fenHistory.push_back(FEN::serialize(board));
}

// runs the game
void GameManager::run() {
    printControls();

    DisplayBoard::printBoard(board);

    std::cout << fenHistory[currentFenIndex] << std::endl;

    // game loop
    while (gameIsRunning) {
        displayTurn();

        bool moveCompleted = processTurn();

        if (moveCompleted && gameIsRunning) {
            // clears terminal
            system("cls");

            printControls();

            // next player (opponent) after the move
            bool isNextPlayerWhite = !board.isWhiteTurn();

            // notify if the opponent's king is in check
            if (KingValidator::isKingInCheck(board, isNextPlayerWhite)) {
                std::cout << std::endl << (isNextPlayerWhite ? "White" : "Black") << " king is in check!" << std::endl;
            }

            DisplayBoard::printBoard(board);

            board.switchTurn();

            // discard snapshots from a redo branch if a new move was made after undo
            fenHistory.resize(currentFenIndex + 1);

            // record the position after the move and turn switch
            fenHistory.push_back(FEN::serialize(board));

            currentFenIndex = fenHistory.size() - 1;

            std::cout << fenHistory[currentFenIndex] << std::endl;
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
        if (!getInput("Enter your move: ", input)) {
            gameIsRunning = false;
            return false;
        }

        // normalize commands, coordinates, and promotion choices to lowercase
        std::transform(input.begin(), input.end(), input.begin(),[](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });

        // reset the default promotion choice for each newly entered move
        promotedPiece = 'q';

        // undo move
        if (input == "u") {
            if (!board.canUndo()) {
                std::cout << "No moves to undo!" << std::endl;
                continue;
            }

            board.undoMove();
            board.switchTurn();

            // move the displayed snapshot back to the position before the undone move
            --currentFenIndex;

            system("cls");
            printControls();
            DisplayBoard::printBoard(board);
            std::cout << fenHistory[currentFenIndex] << std::endl;
            return false;
        }

        // redo move
        if (input == "r") {
            if (!board.canRedo()) {
                std::cout << "No moves to redo!" << std::endl;
                continue;
            }

            board.redoMove();
            board.switchTurn();

            // move the displayed snapshot forward to the redone position
            ++currentFenIndex;

            system("cls");
            printControls();
            DisplayBoard::printBoard(board);
            std::cout << fenHistory[currentFenIndex] << std::endl;
            return false;
        }

        // quit the game
        if (input == "q") {
            gameIsRunning = false;
            return false;
        }

        // resets the game
        if (input == "x") {
            board = ChessBoard(); // overwrites the current board with a fresh one

            // discard the old match snapshots and start history from the initial position
            fenHistory.clear();

            fenHistory.push_back(FEN::serialize(board));
            currentFenIndex = 0;

            system("cls");
            std::cout << "Game reset back to starting position!" << std::endl;
            printControls();
            DisplayBoard::printBoard(board);
            std::cout << fenHistory[currentFenIndex] << std::endl;
            return false;         // returns false so the turn doesn't switch
        }

        // load a position from FEN and start a new match history from it
        if (input == "f") {
            std::string fen;
            std::cout << "Enter FEN: ";

            if (!std::getline(std::cin >> std::ws, fen)) {
                gameIsRunning = false;
                return false;
            }

            if (!FEN::load(board, fen)) {
                std::cout << "Invalid FEN." << std::endl;
                continue;
            }

            fenHistory.clear();
            fenHistory.push_back(FEN::serialize(board));
            currentFenIndex = 0;

            system("cls");
            printControls();
            DisplayBoard::printBoard(board);
            std::cout << fenHistory[currentFenIndex] << std::endl;
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
            for (const std::string& fen : fenHistory) {
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

        Piece selectedPiece = board.getPieceAt(startPos);

        bool isPromotionMove = (selectedPiece == whitePawn || selectedPiece == blackPawn)
            && PawnValidator::isPawnPromotion(endPos, board.isWhiteTurn());

        // reject promotion choices unless a pawn is reaching its last rank
        if (input.length() == 5 && !isPromotionMove) {
            std::cout << "Promotion choice is only valid when a pawn reaches the last rank." << std::endl;
            continue;
        }

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

    // recognize castling only when the king moves between its exact home and castling squares
    bool isCastling = (pieceMoved == whiteKing && startPos == e1 && (endPos == g1 || endPos == c1)) ||
                      (pieceMoved == blackKing && startPos == e8 && (endPos == g8 || endPos == c8));

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
bool GameManager::getInput(const std::string &prompt, std::string &input) {
    std::cout << prompt;

    // report stream failure so the caller can exit when input ends
    return static_cast<bool>(std::cin >> input);
}

// displays the current player's turn
void GameManager::displayTurn() {

    if (board.isWhiteTurn()) {
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
    std::cout << "Enter 'x' to restart game"<< std::endl;
    std::cout << "Enter 'q' to quit game" << std::endl;
    std::cout << "Enter 'e' export FEN from current match" << std::endl;
    std::cout << "Enter 'f' to load match from FEN" << std::endl;
    std::cout << std::endl;
}
