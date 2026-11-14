//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef CHESS_ENGINE_MOBILITY_H
#define CHESS_ENGINE_MOBILITY_H

#include <cstdint>
#include "chess/core/ChessBoard.h"


class Mobility {
private:
    // knight mobility difference, positive favours white, not used by the combined mobility at the moment
    static int evaluateKnightMobility(const ChessBoard &board);

    // bishop mobility difference, positive favours white, not used by the combined mobility at the moment
    static int evaluateBishopMobility(const ChessBoard &board);

    // rook mobility difference, positive favours white, not used by the combined mobility at the moment
    static int evaluateRookMobility(const ChessBoard &board);

    // queen mobility difference, positive favours white, not used by the combined mobility at the moment
    static int evaluateQueenMobility(const ChessBoard &board);

    // shared mobility counter for sliding pieces over the given directions
    static int calculateSlidingMobility(const ChessBoard &board, Piece piece, uint64_t friendlyOccupancy, const Direction directions[], int directionCount);

public:
    // total mobility difference for all piece types, positive favours white
    static int evaluateMobility(const ChessBoard &board);
};

#endif
