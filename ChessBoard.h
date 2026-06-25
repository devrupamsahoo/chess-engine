//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_CHESSBOARD_H
#define CHESS_ENGINE_CHESSBOARD_H

#include <cstdint>
#include <array>
#include "Enums.h"

class ChessBoard {
private:
    // array of 12 bitboards
    std::array<uint64_t, pieceCount> board{};

    bool whiteTurn = true;
public:
    // ctor
    ChessBoard();

    // initialize board
    void initBoard();

    // prints entire board
    void printBoard();

    // prints one bitboard
    void printBitboard(Piece piece);

    // returns true if white's turn
    bool isWhiteTurn();

    // switches player's turn
    void switchTurn();

    // returns piece type exists on a square
    Piece getPieceAt(int square);

    // returns true if the selected coordinate contains a piece belonging to the player whose turn it is
    bool isValidPiece(bool whitesTurn, int square);

    // dtor
    ~ChessBoard() = default;
};


#endif //CHESS_ENGINE_CHESSBOARD_H
