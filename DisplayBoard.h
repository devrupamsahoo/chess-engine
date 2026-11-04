//
// Created by devrupamsahoo on 03-11-2026.
//

#ifndef CHESS_ENGINE_DISPLAYBOARD_H
#define CHESS_ENGINE_DISPLAYBOARD_H

#include "Enums.h"

class ChessBoard;

class DisplayBoard {
public:
    // print the position from black's side, with rank 8 at the top and file a at the left
    static void printBoard(const ChessBoard& board);

    // prints single bitboard
    static void printBitboard(const ChessBoard& board, Piece pieceType);
};

#endif // CHESS_ENGINE_DISPLAYBOARD_H