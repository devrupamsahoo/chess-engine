//
// Created by devrupamsahoo on 10-11-2026.
//

#include "chess/evaluation/PawnStructure.h"
#include <initializer_list>

#include "chess/evaluation/EvaluationConstants.h"

// penalty for stacked pawns on one file, positive favours white
int PawnStructure::evaluateDoubledPawns(const ChessBoard& board) {
    int score = 0;

    for (int file = 0; file < 8; ++file) {
        // count pawns of each colour on the current file
        int whitePawnCount = 0;
        int blackPawnCount = 0;

        // Count pawns on this file.
        for (int rank = 0; rank < 8; ++rank) {
            int square = rank * 8 + file;

            Piece piece = board.getPieceAt(square);

            if (piece == whitePawn) {
                whitePawnCount++;
            }
            else if (piece == blackPawn) {
                blackPawnCount++;
            }
        }

        // Penalize White for doubled pawns.
        if (whitePawnCount > 1) {
            score -= (whitePawnCount - 1) * EvalConstants::DOUBLED_PAWN_PENALTY;
        }

        // Penalize Black for doubled pawns.
        if (blackPawnCount > 1) {
            score += (blackPawnCount - 1) * EvalConstants::DOUBLED_PAWN_PENALTY;
        }
    }

    return score;
}

// penalty for pawns with no friendly pawn on either neighbouring file, positive favours white
int PawnStructure::evaluateIsolatedPawns(const ChessBoard& board) {
    int score = 0;

    for (int square = 0; square < squareCount; ++square) {
        Piece piece = board.getPieceAt(square);

        // skip squares that hold no pawn, both conditions must be true to skip
        if (piece != whitePawn && piece != blackPawn) {
            continue;
        }

        int file = square % 8;

        // assume isolated until a friendly pawn is found next door
        bool hasFriendlyPawn = false;

        // Check the adjacent files.
        for (int adjacentFile : {file - 1, file + 1}) {
            if (adjacentFile < 0 || adjacentFile >= 8) {
                continue;
            }

            for (int rank = 0; rank < 8; ++rank) {
                int adjacentSquare = rank * 8 + adjacentFile;

                Piece adjacentPiece = board.getPieceAt(adjacentSquare);

                if (piece == whitePawn && adjacentPiece == whitePawn) {
                    hasFriendlyPawn = true;
                }

                if (piece == blackPawn && adjacentPiece == blackPawn) {
                    hasFriendlyPawn = true;
                }
            }
        }

        // isolated white pawn lowers the score, isolated black pawn raises it
        if (!hasFriendlyPawn) {
            score += (piece == whitePawn)
                ? -EvalConstants::ISOLATED_PAWN_PENALTY
                : EvalConstants::ISOLATED_PAWN_PENALTY;
        }
    }

    return score;
}

// bonus for pawns with no enemy pawn ahead on their own or neighbouring files, grows with advancement
int PawnStructure::evaluatePassedPawns(const ChessBoard& board) {
    int score = 0;

    for (int square = 0; square < squareCount; ++square) {
        Piece piece = board.getPieceAt(square);

        // only pawns can be passed pawns
        if (piece != whitePawn && piece != blackPawn) {
            continue;
        }

        int rank = square / 8;
        int file = square % 8;

        // assume passed until an enemy pawn blocks the path
        bool isPassed = true;

        // Determine the direction in which to search.
        int startRank;
        int endRank;
        int step;
        Piece enemyPawn;

        if (piece == whitePawn) {
            startRank = rank + 1;
            endRank = 8;
            step = 1;
            enemyPawn = blackPawn;
        } else {
            startRank = rank - 1;
            endRank = -1;
            step = -1;
            enemyPawn = whitePawn;
        }

        // Check the same and adjacent files ahead.
        for (int checkFile = file - 1;
             checkFile <= file + 1;
             ++checkFile) {

            if (checkFile < 0 || checkFile >= 8) {
                continue;
            }

            for (int checkRank = startRank;
                 checkRank != endRank;
                 checkRank += step) {

                int checkSquare = checkRank * 8 + checkFile;

                if (board.getPieceAt(checkSquare) == enemyPawn) {
                    isPassed = false;
                    break;
                }
                 }

            if (!isPassed) {
                break;
            }
             }

        // black rank is mirrored so the same bonus table serves both colours
        if (isPassed) {
            if (piece == whitePawn) {
                score += EvalConstants::PASSED_PAWN_BONUS[rank];
            } else {
                score -= EvalConstants::PASSED_PAWN_BONUS[7 - rank];
            }
        }
    }

    return score;
}
