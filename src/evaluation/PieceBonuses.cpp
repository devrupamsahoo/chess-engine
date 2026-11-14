//
// Created by devrupamsahoo on 10-11-2026.
//

#include "chess/evaluation/PieceBonuses.h"

#include "chess/evaluation/EvaluationConstants.h"

// bonus for the side that owns both bishops, positive favours white
int PieceBonuses::evaluateBishopPair(const ChessBoard& board) {
    // count bishops for each colour
    int whiteBishops = 0;
    int blackBishops = 0;

    for (int square = 0; square < squareCount; ++square) {
        Piece piece = board.getPieceAt(square);

        if (piece == whiteBishop) {
            whiteBishops++;
        }
        else if (piece == blackBishop) {
            blackBishops++;
        }
    }

    int score = 0;

    // bonus for the white pair, penalty for the black pair
    if (whiteBishops >= 2) {
        score += EvalConstants::BISHOP_PAIR_BONUS;
    }

    if (blackBishops >= 2) {
        score -= EvalConstants::BISHOP_PAIR_BONUS;
    }

    return score;
}

// bonus for a rook on an open or semi open file, caller flips the sign for black
int PieceBonuses::evaluateRookFile(const ChessBoard& board, int square, Piece rook) {
    int file = square % 8;

    // remember whether each side has a pawn on this file
    bool hasWhitePawn = false;
    bool hasBlackPawn = false;

    // Check all squares in this file.
    for (int rank = 0; rank < 8; ++rank) {
        int currentSquare = rank * 8 + file;

        Piece piece = board.getPieceAt(currentSquare);

        if (piece == whitePawn) {
            hasWhitePawn = true;
        }
        else if (piece == blackPawn) {
            hasBlackPawn = true;
        }
    }

    // open means no pawns at all, semi open means no own pawn but an enemy pawn
    int bonus = 0;

    // white rook checks for black pawns, black rook checks for white pawns
    if (rook == whiteRook) {
        if (!hasWhitePawn && !hasBlackPawn) {
            bonus = EvalConstants::ROOK_OPEN_FILE_BONUS;
        }
        else if (!hasWhitePawn && hasBlackPawn) {
            bonus = EvalConstants::ROOK_SEMI_OPEN_FILE_BONUS;
        }
    }
    else if (rook == blackRook) {
        if (!hasWhitePawn && !hasBlackPawn) {
            bonus = EvalConstants::ROOK_OPEN_FILE_BONUS;
        }
        else if (!hasBlackPawn && hasWhitePawn) {
            bonus = EvalConstants::ROOK_SEMI_OPEN_FILE_BONUS;
        }
    }

    return bonus;
}

// bonus for a rook sitting on the seventh rank relative to its colour
int PieceBonuses::evaluateRookSeventhRank(int square, Piece rook) {
    int rank = square / 8;

    // White rook on rank 7
    if (rook == whiteRook && rank == 6) {
        return EvalConstants::ROOK_SEVENTH_RANK_BONUS;
    }

    // Black rook on rank 2
    if (rook == blackRook && rank == 1) {
        return EvalConstants::ROOK_SEVENTH_RANK_BONUS;
    }

    return 0;
}
