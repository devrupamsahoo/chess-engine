//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef CHESS_ENGINE_PIECE_BONUSES_H
#define CHESS_ENGINE_PIECE_BONUSES_H

#include <cstdint>
#include "chess/core/ChessBoard.h"

class PieceBonuses {
public:
    // bonus for the side that owns both bishops, positive favours white
    static int evaluateBishopPair(const ChessBoard& board);

    // bonus for a rook on an open or semi open file, caller flips the sign for black
    static int evaluateRookFile(const ChessBoard& board, int square, Piece rook);

    // bonus for a rook sitting on the seventh rank relative to its colour
    static int evaluateRookSeventhRank(int square, Piece rook);
};

#endif
