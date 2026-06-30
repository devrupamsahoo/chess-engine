//
// Created by sahoo on 28-06-2026.
//

#ifndef CHESS_ENGINE_ATTACKVALIDATOR_H
#define CHESS_ENGINE_ATTACKVALIDATOR_H


#include "ChessBoard.h"
#include "RayTable.h"

// precomputed attack masks for all pieces
namespace AttackTables {
    // pre-computed attack table for knights
    extern uint64_t KnightAttacks[64];

    // pre-computed attack table for kings
    extern uint64_t KingAttacks[64];

    // pre-computed attack table for pawns
    // pawnAttacks[0] for white pawns
    // pawnAttacks[1] for black pawns
    extern uint64_t PawnAttacks[2][64];
}

class AttackValidator {
private:
    // initializes knight attack table computation
    static void initKnightAttacks();

    // initializes king attack table computation
    static void initKingAttacks();

    // initializes pawn attack table computation
    static void initPawnAttacks();

    // returns true if a square is being attacked by knights of given color
    static bool isAttackedByKnight(ChessBoard &board, int square, bool attackerIsWhite);

    // returns true if a square is being attacked by king of given color
    static bool isAttackedByKing(ChessBoard &board, int square, bool attackerIsWhite);

    // returns true if a square is being attacked by pawns of given color
    static bool isAttackedByPawn(ChessBoard &board, int square, bool attackerIsWhite);

    // returns true if a square is being attacked by rooks of given color
    static bool isAttackedByRook(ChessBoard &board, int square, bool attackerIsWhite);

    // returns true if a square is being attacked by bishops of given color
    static bool isAttackedByBishop(ChessBoard &board, int square, bool attackerIsWhite);

    // returns true if a square is being attacked by queens of given color
    static bool isAttackedByQueen(ChessBoard &board, int square, bool attackerIsWhite);

public:
    // returns true if a square is being attacked by pieces of given color
    static bool isSquareUnderAttack(ChessBoard &board, int square, bool attackerIsWhite);

};


#endif //CHESS_ENGINE_ATTACKVALIDATOR_H
