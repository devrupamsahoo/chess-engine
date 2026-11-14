//
// Created by devrupamsahoo on 10-11-2026.
//

#include "chess/evaluation/EvaluationConstants.h"

namespace EvalConstants {
    // knight piece square table, stored rank eight first
    const int KNIGHT_PST[64] = {
        // rank 8
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -30, 0, 10, 15, 15, 10, 0, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 0, 15, 20, 20, 15, 0, -30,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -40, -20, 0, 5, 5, 0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
        // rank 1
    };

    // pawn piece square table, stored rank eight first
    const int PAWN_PST[64] = {
        // rank 8
        0, 0, 0, 0, 0, 0, 0, 0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5, 5, 10, 25, 25, 10, 5, 5,
        0, 0, 0, 20, 20, 0, 0, 0,
        5, -5, -10, 0, 0, -10, -5, 5,
        5, 10, 10, -20, -20, 10, 10, 5,
        0, 0, 0, 0, 0, 0, 0, 0
        // rank 1
    };

    // bishop piece square table, stored rank eight first
    const int BISHOP_PST[64] = {
        // rank 8
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, 5, 0, 0, 0, 0, 5, -10,
        -10, 10, 10, 10, 10, 10, 10, -10,
        -10, 0, 10, 10, 10, 10, 0, -10,
        -10, 5, 5, 10, 10, 5, 5, -10,
        -10, 0, 5, 10, 10, 5, 0, -10,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
        // rank 1
    };

    // rook piece square table, stored rank eight first
    const int ROOK_PST[64] = {
        // rank 8
        0, 0, 0, 5, 5, 0, 0, 0,
        5, 10, 10, 10, 10, 10, 10, 5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        5, 10, 10, 10, 10, 10, 10, 5,
        0, 0, 0, 5, 5, 0, 0, 0
        // rank 1
    };

    // queen piece square table, stored rank eight first
    const int QUEEN_PST[64] = {
        // rank 8
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10, 0, 5, 0, 0, 0, 0, -10,
        -10, 5, 5, 5, 5, 5, 0, -10,
        -5, 0, 5, 5, 5, 5, 0, -5,
        0, 0, 5, 5, 5, 5, 0, -5,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20
        // rank 1
    };

    // king middlegame piece square table, stored rank eight first
    const int KING_MIDDLEGAME_PST[64] = {
        // rank 8
        20, 30, 10, 0, 0, 10, 30, 20,
        20, 20, 0, 0, 0, 0, 20, 20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30
        // rank 1
    };

    // king endgame piece square table, stored rank eight first
    const int KING_ENDGAME_PST[64] = {
        -50, -30, -30, -30, -30, -30, -30, -50,
        -30, -10, -10, -10, -10, -10, -10, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -20, -10, 0, 0, -10, -20, -30,
        -50, -40, -30, -20, -20, -30, -40, -50
    };

    // passed pawn bonus indexed by rank, advanced pawns are worth more
    const int PASSED_PAWN_BONUS[8] = {
    0, 10, 20, 30, 50, 75, 120, 0
    };

    // extra king danger by number of distinct attackers, capped at five
    const int KING_ATTACKER_COUNT_BONUS[6] = {
     0, 5, 12, 22, 35, 50
    };

    // king danger lookup, indexed by attack pressure divided by five
    const int KING_DANGER_TABLE[21] = {
    0,   5,  10,  15,  20,
       26,  33,  41,  50,  60,
       71,  83,  96, 110, 125,
      141, 158, 176, 195, 215,
      236
    };
}
