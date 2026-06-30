//
// Created by sahoo on 28-06-2026.
//

#include "AttackValidator.h"
#include "Utils.h"

// precomputed attack masks indexed by source square
namespace AttackTables {
    uint64_t KnightAttacks[64] = {0};
    uint64_t KingAttacks[64] = {0};

    // pawnAttacks[0] for white pawns, pawnAttacks[1] for black pawns
    uint64_t PawnAttacks[2][64] = {0};
}

// initializes knight attack table computation
void AttackValidator::initKnightAttacks() {
    // step values for all eight possible knight moves
    int dr[8] = {2, 2, -2, -2, 1, 1, -1, -1};
    int df[8] = {1, -1, 1, -1, 2, -2, 2, -2};

    // generate attack mask for every board squares
    for (int square = 0; square < 64; square++) {
        // attack mask for the square
        uint64_t attacks = 0;

        int rank = square >> 3;
        int file = square & 7;

        // explores all 8 possible moves from a square
        for (int i = 0; i < 8; i++) {
            int newRank = rank + dr[i];
            int newFile = file + df[i];

            // ignores moves which leaves the board
            if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
                // computes & inserts the position into the attack mask
                int targetSquare = newRank * 8 + newFile;
                attacks |= (1ULL << targetSquare);
            }
        }

        // stores complete attack masks for the square
        AttackTables::KnightAttacks[square] = attacks;
    }
}

// initializes king attack table computation
void AttackValidator::initKingAttacks() {
    // step values for all eight possible king moves
    int dr[8] = {1, -1, -1, 1, 1, 0, -1, 0};
    int df[8] = {1, -1, 1, -1, 0, 1, 0, -1};

    // generates attack mask for every square in board
    for (int square = 0; square < 64; square++) {
        // attack mask for the square
        uint64_t attacks = 0;

        int rank = square >> 3;
        int file = square & 7;

        // explores all 8 possible moves from a square
        for (int i = 0; i < 8; i++) {
            int newRank = rank + dr[i];
            int newFile = file + df[i];

            // ignores moves which leaves the board
            if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {

                // computes & inserts the position into the attack mask
                int targetSquare = newRank * 8 + newFile;
                attacks |= (1ULL << targetSquare);
            }
        }

        // stores complete attack masks for the square
        AttackTables::KingAttacks[square] = attacks;
    }
}

// initializes pawn attack table computation
void AttackValidator::initPawnAttacks() {
    // generates attack mask for every square in board
    for (int square = 0; square < 64; square++) {

        // attack mask for white and black pawns for a square
        uint64_t whiteAttacks = 0;
        uint64_t blackAttacks = 0;

        int rank = square >> 3;
        int file = square & 7;

        // step values for left capture move
        int wRank1 = rank + 1;
        int wFile1 = file - 1;

        // step values for right capture move
        int wRank2 = rank + 1;
        int wFile2 = file + 1;

        // left diagonal capture
        if (wRank1 < 8 && wFile1 >= 0) {
            // computes & inserts the position into the attack mask
            int targetSquare = wRank1 * 8 + wFile1;
            whiteAttacks |= (1ULL << targetSquare);
        }

        // right diagonal capture
        if (wRank2 < 8 && wFile2 < 8) {
            int targetSquare = wRank2 * 8 + wFile2;
            whiteAttacks |= (1ULL << targetSquare);
        }

        // step values for left capture move
        int bRank1 = rank - 1;
        int bFile1 = file - 1;

        // step values for right capture move
        int bRank2 = rank - 1;
        int bFile2 = file + 1;

        // left diagonal capture
        if (bRank1 >= 0 && bFile1 >= 0) {
            int targetSquare = bRank1 * 8 + bFile1;
            blackAttacks |= (1ULL << targetSquare);
        }

        // right diagonal capture
        if (bRank2 >= 0 && bFile2 < 8) {
            int targetSquare = bRank2 * 8 + bFile2;
            blackAttacks |= (1ULL << targetSquare);
        }

        // store attack masks of pawns
        AttackTables::PawnAttacks[0][square] = whiteAttacks;
        AttackTables::PawnAttacks[1][square] = blackAttacks;
    }
}

// returns true if a square is being attacked by knights of given color
bool AttackValidator::isAttackedByKnight(ChessBoard &board, int square, bool attackerIsWhite) {
    // gets the bitboard containing all knights of the specified color
    uint64_t knights = attackerIsWhite ? board.getPieceBitboard(whiteKnight) : board.getPieceBitboard(blackKnight);

    // iterate through every knight on the board
    while (knights) {

        // extracts the least significant set bit, which represents one knight
        int fromSquare = __builtin_ctzll(knights);

        // removes the processed knight from the bitboard
        knights &= knights - 1;

        // lookup all squares this knight attacks
        uint64_t attackMask = AttackTables::KnightAttacks[fromSquare];

        // if the target square is in the attack mask, it is under attack
        if (attackMask & (1ULL << square)) {
            return true;
        }
    }

    return false;
}

// returns true if a square is being attacked by king of given color
bool AttackValidator::isAttackedByKing(ChessBoard &board, int square, bool attackerIsWhite) {
    // gets the bitboard containing all kings of the specified color
    uint64_t kings = attackerIsWhite ? board.getPieceBitboard(whiteKing) : board.getPieceBitboard(blackKing);

    // iterate through king on the board
    while (kings) {

        // extracts the least significant set bit, which represents one king
        int kingSquare = __builtin_ctzll(kings);

        // removes the processed king from the bitboard
        kings &= kings - 1;

        // gets the attack mask from the pre-computed lookup tables
        uint64_t attackMask = AttackTables::KingAttacks[kingSquare];

        // if the target square is in the attack mask, it is under attack
        if (attackMask & (1ULL << square)) {
            return true;
        }
    }

    return false;
}

// returns true if a square is being attacked by pawns of given color
bool AttackValidator::isAttackedByPawn(ChessBoard &board, int square, bool attackerIsWhite) {
    uint64_t pawns = attackerIsWhite ? board.getPieceBitboard(whitePawn) : board.getPieceBitboard(blackPawn);
    int color = attackerIsWhite ? 0 : 1;

    while (pawns) {

        int pawnSquare = __builtin_ctzll(pawns);
        pawns &= pawns - 1;

        uint64_t attackMask = AttackTables::PawnAttacks[color][pawnSquare];

        if (attackMask & (1ULL << square)) {
            return true;
        }
    }

    return false;
}

// returns true if a square is being attacked by rooks of given color
bool AttackValidator::isAttackedByRook(ChessBoard &board, int square, bool attackerIsWhite) {
    uint64_t occupancy = board.getAllOccupancy();

    // queen also behaves like rook
    uint64_t rooks = attackerIsWhite ? (board.getPieceBitboard(whiteRook) | board.getPieceBitboard(whiteQueen))
                            : (board.getPieceBitboard(blackRook) | board.getPieceBitboard(blackQueen));

    // loop through all rooks and queen
    while (rooks) {

        int rookSquare = __builtin_ctzll(rooks);
        rooks &= rooks - 1;

        // check four directions
        Direction dirs[4] = {up, down, left, right};

        // walks in four directions one by one
        for (int i = 0; i < 4; i++) {
            // gets the path in the direction from that square
            const std::vector<int> &ray = AttackTables::rayTable.getRay(rookSquare, dirs[i]);

            // walks in the direction square by square
            for (int sq : ray) {
                // the current square on the ray is exactly the target square
                if (sq == square) {
                    return true;
                }

                // stops if the path is blocked
                if (Utils::checkBit(occupancy, sq)) {
                    break;
                }
            }
        }
    }

    return false;
}

// returns true if a square is being attacked by bishops of given color
bool AttackValidator::isAttackedByBishop(ChessBoard &board, int square, bool attackerIsWhite) {
    uint64_t occupancy = board.getAllOccupancy();

    // queen also behaves like bishop
    uint64_t bishops = attackerIsWhite ? (board.getPieceBitboard(whiteBishop) | board.getPieceBitboard(whiteQueen))
                                : (board.getPieceBitboard(blackBishop) | board.getPieceBitboard(blackQueen));

    Direction dirs[4] = {upLeft, upRight, downLeft, downRight};

    while (bishops) {

        int bishopSquare = __builtin_ctzll(bishops);
        bishops &= bishops - 1;

        for (int i = 0; i < 4; i++) {

            const std::vector<int> &ray = AttackTables::rayTable.getRay(bishopSquare, dirs[i]);

            for (int sq : ray) {

                if (sq == square) {
                    return true;
                }

                if (Utils::checkBit(occupancy, sq)) {
                    break;
                }
            }
        }
    }

    return false;
}

// returns true if a square is being attacked by queens of given color
bool AttackValidator::isAttackedByQueen(ChessBoard &board, int square, bool attackerIsWhite) {
    // queen is the combination of rook and bishop

    bool rookAttack = isAttackedByRook(board, square, attackerIsWhite);

    if (rookAttack) {
        return true;
    }

    bool bishopAttack = isAttackedByBishop(board, square, attackerIsWhite);

    if (bishopAttack) {
        return true;
    }

    return false;
}

// returns true if a square is being attacked by pieces of given color
bool AttackValidator::isSquareUnderAttack(ChessBoard &board, int square, bool attackerIsWhite) {

    if (isAttackedByPawn(board, square, attackerIsWhite)) {
        return true;
    }

    if (isAttackedByKnight(board, square, attackerIsWhite)) {
        return true;
    }

    if (isAttackedByKing(board, square, attackerIsWhite)) {
        return true;
    }

    if (isAttackedByBishop(board, square, attackerIsWhite)) {
        return true;
    }

    if (isAttackedByRook(board, square, attackerIsWhite)) {
        return true;
    }

    if (isAttackedByQueen(board, square, attackerIsWhite)) {
        return true;
    }

    return false;
}
