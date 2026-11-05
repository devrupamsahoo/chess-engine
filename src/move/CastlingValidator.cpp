//
// Created by sahoo on 25-06-2026.
//

#include "chess/move/CastlingValidator.h"
#include "chess/move/AttackValidator.h"
#include "chess/move/KingValidator.h"
#include "chess/move/PathValidator.h"

// returns true if kingside castling rights are still available
bool CastlingValidator::hasKingsideRights(ChessBoard &board, bool isWhitePlayer) {
    const CastlingRights &r = board.getCastlingRights();

    return isWhitePlayer ? r.wk : r.bk;
}

// returns true if queenside castling rights are still available
bool CastlingValidator::hasQueensideRights(ChessBoard &board, bool isWhitePlayer) {
    const CastlingRights &r = board.getCastlingRights();

    return  isWhitePlayer ? r.wq : r.bq;
}

// returns true if all specified squares are safe from enemy attacks
bool CastlingValidator::areSquaresSafeFromAttack(ChessBoard &board, std::vector<int> &squares, bool isWhitePlayer) {
    // check each square against attacks from the opposing side
    for (int sq : squares) {
        if (AttackValidator::isSquareUnderAttack(board, sq, !isWhitePlayer)) {
            return false;
        }
    }

    return true;
}

// every square between the king and rook must be empty
bool CastlingValidator::isPathClear(ChessBoard &board, int startPos, int endPos) {
    if (PathValidator::isPathBlocked(board, startPos, endPos)) {
        return false;
    }

    return true;
}

// returns true if castling is possible on the king side
bool CastlingValidator::canCastleKingside(ChessBoard &board, bool isWhitePlayer) {
    if (KingValidator::isKingInCheck(board, isWhitePlayer)) {
        return false;
    }

    if (!hasKingsideRights(board, isWhitePlayer)) {
        return false;
    }

    // castling rights record move history, so verify the king and rook are still on their starting squares
    int kingSquare = isWhitePlayer ? e1 : e8;
    int rookSquare = isWhitePlayer ? h1 : h8;

    Piece king = isWhitePlayer ? whiteKing : blackKing;
    Piece rook = isWhitePlayer ? whiteRook : blackRook;

    if (board.getPieceAt(kingSquare) != king || board.getPieceAt(rookSquare) != rook) {
        return false;
    }

    int pathStart = isWhitePlayer ? e1 : e8;
    int pathEnd   = isWhitePlayer ? h1 : h8;

    if (!isPathClear(board, pathStart, pathEnd)) {
        return false;
    }

    // the king cannot start on, cross, or finish on a square attacked by the opponent
    std::vector<int> squares = isWhitePlayer ? std::vector<int>{e1, f1, g1} : std::vector<int>{e8, f8, g8};

    if (!areSquaresSafeFromAttack(board, squares, isWhitePlayer)) {
        return false;
    }

    return true;
}

// returns true if castling is possible on the queen side
bool CastlingValidator::canCastleQueenside(ChessBoard &board, bool isWhitePlayer) {
    if (KingValidator::isKingInCheck(board, isWhitePlayer)) {
        return false;
    }

    if (!hasQueensideRights(board, isWhitePlayer)) {
        return false;
    }

    // castling rights record move history, so verify the king and rook are still on their starting squares
    int kingSquare = isWhitePlayer ? e1 : e8;
    int rookSquare = isWhitePlayer ? a1 : a8;

    Piece king = isWhitePlayer ? whiteKing : blackKing;
    Piece rook = isWhitePlayer ? whiteRook : blackRook;

    if (board.getPieceAt(kingSquare) != king || board.getPieceAt(rookSquare) != rook) {
        return false;
    }

    int pathStart = isWhitePlayer ? e1 : e8;
    int pathEnd   = isWhitePlayer ? a1 : a8;

    if (!isPathClear(board, pathStart, pathEnd)) {
        return false;
    }

    // squares should not be attacked by any enemy pieces
    std::vector<int> squares = isWhitePlayer ? std::vector<int>{e1, d1, c1} : std::vector<int>{e8, d8, c8};

    if (!areSquaresSafeFromAttack(board, squares, isWhitePlayer)) {
        return false;
    }

    return true;
}
