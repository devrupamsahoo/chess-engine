//
// Created by sahoo on 25-06-2026.
//

#include "KingValidator.h"

#include "AttackValidator.h"
#include "Utils.h"

// returns true if a king move is valid
bool KingValidator::isKingMove(int startPos, int endPos) {
    int startRank = startPos >> 3;
    int startFile = startPos & 7;

    int endRank = endPos >> 3;
    int endFile = endPos & 7;

    int rankDiff = abs(startRank - endRank);
    int fileDiff = abs(startFile - endFile);

    return (rankDiff <= 1) && (fileDiff <= 1);
}

// returns true if the specified player's king is currently in check
bool KingValidator::isKingInCheck(ChessBoard &board, bool isWhitePlayer) {

    int kingSquare = board.getKingPos(isWhitePlayer);

    bool attackerIsWhite = !isWhitePlayer;

    return AttackValidator::isSquareUnderAttack(
        board,
        kingSquare,
        attackerIsWhite
    );
}

// returns true if the move leaves the current player's king in check
bool KingValidator::leavesKingInCheck(ChessBoard &board, int startPos, int endPos) {

    bool isWhitePlayer = board.isWhiteTurn();

    board.makeMove(startPos, endPos);

    bool kingInCheck = isKingInCheck(board, isWhitePlayer);

    board.undoMove();

    return kingInCheck;
}

