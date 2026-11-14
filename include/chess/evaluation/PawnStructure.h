//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef CHESS_ENGINE_PAWN_STRUCTURE_H
#define CHESS_ENGINE_PAWN_STRUCTURE_H

#include <cstdint>
#include "chess/core/ChessBoard.h"

class PawnStructure {
public:
    // penalty for stacked pawns on one file, positive favours white
    static int evaluateDoubledPawns(const ChessBoard& board);

    // penalty for pawns with no friendly pawn on either neighbouring file, positive favours white
    static int evaluateIsolatedPawns(const ChessBoard& board);

    // bonus for pawns with no enemy pawn ahead on their own or neighbouring files, grows with advancement
    static int evaluatePassedPawns(const ChessBoard& board);
};

#endif
