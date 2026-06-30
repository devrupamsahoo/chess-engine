//
// Created by sahoo on 25-06-2026.
//

#include "CastlingValidator.h"

#include <iostream>

#include "AttackValidator.h"
#include "KingValidator.h"
#include "PathValidator.h"

// returns true if kingside castling rights are still available
bool CastlingValidator::hasKingsideRights(ChessBoard &board, bool isWhitePlayer) {
    const CastlingRights &r = board.getCastlingRights();

    return isWhitePlayer ? r.wk : r.bk;
}

// returns true if queenside castling rights are still available
bool CastlingValidator::hasQueensideRights(ChessBoard &board, bool isWhitePlayer) {
    const CastlingRights &r = board.getCastlingRights();

    return isWhitePlayer ? r.wq : r.bq;
}

// returns true if all specified squares are safe from enemy attacks
bool CastlingValidator::areSquaresSafeFromAttack(ChessBoard &board, std::vector<int> &squares, bool isWhitePlayer) {
    for (int sq : squares) {
        if (AttackValidator::isSquareUnderAttack(board, sq, !isWhitePlayer)) {
            return false;
        }
    }

    return true;
}

// returns true if all squares between the king and rook are unoccupied
bool CastlingValidator::isPathClear(ChessBoard &board, int startPos, int endPos) {
    if (PathValidator::isPathBlocked(board, startPos, endPos)) {
        return false;
    }

    return true;
}

// returns true if castling is possible on the king side
bool CastlingValidator::canCastleKingside(ChessBoard &board, bool isWhitePlayer) {
    if (KingValidator::isKingInCheck(board, isWhitePlayer)) {
        std::cout << "King is in check\n";
        return false;
    }

    if (!hasKingsideRights(board, isWhitePlayer)) {
        std::cout << "No castling rights\n";
        return false;
    }

    int pathStart = isWhitePlayer ? e1 : e8;
    int pathEnd   = isWhitePlayer ? h1 : h8;

    if (!isPathClear(board, pathStart, pathEnd)) {
        std::cout << "Path not clear\n";
        return false;
    }

    // squares should not be attacked by any enemy pieces
    std::vector<int> squares = isWhitePlayer ? std::vector<int>{e1, f1, g1} : std::vector<int>{e8, f8, g8};

    if (!areSquaresSafeFromAttack(board, squares, isWhitePlayer)) {
        std::cout << "Squares unsafe\n";
        return false;
    }

    return true;
}

// returns true if castling is possible on the queen side
bool CastlingValidator::canCastleQueenside(ChessBoard &board, bool isWhitePlayer) {
    if (KingValidator::isKingInCheck(board, isWhitePlayer)) {
        std::cout << "King is in check\n";
        return false;
    }

    if (!hasQueensideRights(board, isWhitePlayer)) {
        std::cout << "No castling rights\n";
        return false;
    }

    int pathStart = isWhitePlayer ? e1 : e8;
    int pathEnd   = isWhitePlayer ? a1 : a8;

    if (!isPathClear(board, pathStart, pathEnd)) {
        std::cout << "Path not clear\n";
        return false;
    }

    // squares should not be attacked by any enemy pieces
    std::vector<int> squares = isWhitePlayer ? std::vector<int>{e1, d1, c1} : std::vector<int>{e8, d8, c8};

    if (!areSquaresSafeFromAttack(board, squares, isWhitePlayer)) {
        std::cout << "Squares are unsafe\n";
        return false;
    }

    return true;
}
