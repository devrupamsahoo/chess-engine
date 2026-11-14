//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef CHESS_ENGINE_EVALUATION_CONSTANTS_H
#define CHESS_ENGINE_EVALUATION_CONSTANTS_H

namespace EvalConstants {
    // material values in centipawns, king is zero since it is never captured
    constexpr int PAWN_VALUE = 100;
    constexpr int KNIGHT_VALUE = 320;
    constexpr int BISHOP_VALUE = 330;
    constexpr int ROOK_VALUE = 500;
    constexpr int QUEEN_VALUE = 900;
    constexpr int KING_VALUE = 0;

    // bonus for owning both bishops
    constexpr int BISHOP_PAIR_BONUS = 30;

    // rook file bonuses, open means no pawns, semi open means only enemy pawns
    constexpr int ROOK_OPEN_FILE_BONUS = 20;
    constexpr int ROOK_SEMI_OPEN_FILE_BONUS = 10;

    // bonus for a rook on the seventh rank
    constexpr int ROOK_SEVENTH_RANK_BONUS = 20;

    // penalty for each extra pawn stacked on a file
    constexpr int DOUBLED_PAWN_PENALTY = 15;

    // penalty for a pawn with no friendly pawn on neighbouring files
    constexpr int ISOLATED_PAWN_PENALTY = 15;

    // mobility weight per available square for each piece type
    constexpr int KNIGHT_MOBILITY_WEIGHT = 4;
    constexpr int BISHOP_MOBILITY_WEIGHT = 3;
    constexpr int ROOK_MOBILITY_WEIGHT = 2;
    constexpr int QUEEN_MOBILITY_WEIGHT = 1;

    // knight piece square table, stored rank eight first
    extern const int KNIGHT_PST[64];

    // pawn piece square table, stored rank eight first
    extern const int PAWN_PST[64];

    // bishop piece square table, stored rank eight first
    extern const int BISHOP_PST[64];

    // rook piece square table, stored rank eight first
    extern const int ROOK_PST[64];

    // queen piece square table, stored rank eight first
    extern const int QUEEN_PST[64];

    // king middlegame piece square table, stored rank eight first
    extern const int KING_MIDDLEGAME_PST[64];

    // king endgame piece square table, stored rank eight first
    extern const int KING_ENDGAME_PST[64];

    // passed pawn bonus indexed by rank, advanced pawns are worth more
    extern const int PASSED_PAWN_BONUS[8];

    // extra king danger by number of distinct attackers, capped at five
    extern const int KING_ATTACKER_COUNT_BONUS[6];

    // king danger lookup, indexed by attack pressure divided by five
    extern const int KING_DANGER_TABLE[21];
}

#endif
