//
// Created by sahoo on 25-06-2026.
//

#ifndef CHESS_ENGINE_PIECEOWNERSHIP_H
#define CHESS_ENGINE_PIECEOWNERSHIP_H

#include "ChessBoard.h"

class PieceOwnership {
public:
    // returns true if the specified square is empty
    static bool isEmptySquare(ChessBoard &board, int square);

    // returns true if the specified square contains the current player's own piece
    static bool isOwnPiece(ChessBoard &board, int square);

    // returns true if the specified square contains an enemy piece
    static bool isEnemyPiece(ChessBoard &board, int square);
};


#endif //CHESS_ENGINE_PIECEOWNERSHIP_H
