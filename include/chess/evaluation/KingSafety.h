//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef CHESS_ENGINE_KING_SAFETY_H
#define CHESS_ENGINE_KING_SAFETY_H

#include <cstdint>
#include "chess/core/ChessBoard.h"

class KingSafety {
private:
    // bonus for pawns directly in front of each king, positive favours white
    static int evaluatePawnShield(const ChessBoard &board);

    // penalty for open and semi open files next to each king, positive favours white
    static int evaluateKingOpenFiles(const ChessBoard &board);

    // penalty for enemy attacks on squares next to each king, not used by the main evaluation at the moment
    static int evaluateKingZonePressure(const ChessBoard &board);

    // turns pressure from unique enemy attackers into king danger, scaled by game phase, positive favours white
    static int evaluateKingAttackers(const ChessBoard &board, int gamePhase);

public:
    // combines pawn shield, open files and attacker pressure into one king safety score
    static int evaluateKingSafety(const ChessBoard &board, int gamePhase);
};

#endif
