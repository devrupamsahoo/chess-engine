//
// Created by devrupamsahoo on 10-11-2026.
//

#include "chess/evaluation/KingSafety.h"
#include <cstdlib>

#include "chess/evaluation/EvaluationConstants.h"
#include "chess/move/AttackValidator.h"
#include "chess/evaluation/KingAttackers.h"

// bonus for pawns directly in front of each king, positive favours white
int KingSafety::evaluatePawnShield(const ChessBoard &board) {
    int score = 0;

    // negative one means the king was not found
    int whiteKingSquare = -1;
    int blackKingSquare = -1;

    // Find both kings
    for (int square = 0; square < squareCount; ++square) {
        Piece piece = board.getPieceAt(square);

        if (piece == whiteKing) {
            whiteKingSquare = square;
        }
        else if (piece == blackKing) {
            blackKingSquare = square;
        }
    }

    // Evaluate White's pawn shield
    if (whiteKingSquare != -1) {
        int kingFile = whiteKingSquare % 8;
        int kingRank = whiteKingSquare / 8;

        // white shield pawns stand one rank above the king
        int shieldRank = kingRank + 1;

        if (shieldRank < 8) {
            for (int file = kingFile - 1; file <= kingFile + 1; ++file) {
                if (file < 0 || file >= 8) {
                    continue;
                }

                int square = shieldRank * 8 + file;

                if (board.getPieceAt(square) == whitePawn) {
                    // each shield pawn adds a fixed bonus
                    score += 10;
                }
            }
        }
    }

    // Evaluate Black's pawn shield
    if (blackKingSquare != -1) {
        int kingFile = blackKingSquare % 8;
        int kingRank = blackKingSquare / 8;

        // black shield pawns stand one rank below the king
        int shieldRank = kingRank - 1;

        if (shieldRank >= 0) {
            for (int file = kingFile - 1; file <= kingFile + 1; ++file) {
                if (file < 0 || file >= 8) {
                    continue;
                }

                int square = shieldRank * 8 + file;

                if (board.getPieceAt(square) == blackPawn) {
                    // black shield pawn counts against the white perspective
                    score -= 10;
                }
            }
        }
    }

    return score;
}

// penalty for open and semi open files next to each king, positive favours white
int KingSafety::evaluateKingOpenFiles(const ChessBoard &board) {
    int score = 0;

    int whiteKingSquare = -1;
    int blackKingSquare = -1;

    // Find both kings
    for (int square = 0; square < squareCount; ++square) {
        Piece piece = board.getPieceAt(square);

        if (piece == whiteKing) {
            whiteKingSquare = square;
        }
        else if (piece == blackKing) {
            blackKingSquare = square;
        }
    }

    // Evaluate files around White's king
    if (whiteKingSquare != -1) {
        int kingFile = whiteKingSquare % 8;

        for (int file = kingFile - 1; file <= kingFile + 1; ++file) {
            if (file < 0 || file >= 8) {
                continue;
            }

            // scan the file for pawns of each colour
            bool hasWhitePawn = false;
            bool hasBlackPawn = false;

            for (int rank = 0; rank < 8; ++rank) {
                int square = rank * 8 + file;

                Piece piece = board.getPieceAt(square);

                if (piece == whitePawn) {
                    hasWhitePawn = true;
                }
                else if (piece == blackPawn) {
                    hasBlackPawn = true;
                }
            }

            // fully open file costs more than a semi open one
            if (!hasWhitePawn && !hasBlackPawn) {
                score -= 20;
            }
            else if (!hasWhitePawn && hasBlackPawn) {
                score -= 10;
            }
        }
    }

    // Evaluate files around Black's king
    if (blackKingSquare != -1) {
        int kingFile = blackKingSquare % 8;

        for (int file = kingFile - 1; file <= kingFile + 1; ++file) {
            if (file < 0 || file >= 8) {
                continue;
            }

            // scan the file for pawns of each colour
            bool hasWhitePawn = false;
            bool hasBlackPawn = false;

            for (int rank = 0; rank < 8; ++rank) {
                int square = rank * 8 + file;

                Piece piece = board.getPieceAt(square);

                if (piece == whitePawn) {
                    hasWhitePawn = true;
                }
                else if (piece == blackPawn) {
                    hasBlackPawn = true;
                }
            }

            // same logic mirrored for the black king
            if (!hasWhitePawn && !hasBlackPawn) {
                score += 20;
            }
            else if (!hasBlackPawn && hasWhitePawn) {
                score += 10;
            }
        }
    }

    return score;
}

// penalty for enemy attacks on squares next to each king, not used by the main evaluation at the moment
int KingSafety::evaluateKingZonePressure(const ChessBoard &board) {
    int score = 0;

    int whiteKingSquare = -1;
    int blackKingSquare = -1;

    // Find both kings
    for (int square = 0; square < squareCount; ++square) {
        Piece piece = board.getPieceAt(square);

        if (piece == whiteKing) {
            whiteKingSquare = square;
        }
        else if (piece == blackKing) {
            blackKingSquare = square;
        }
    }

    // Eight possible neighboring directions
    const int directions[8] = {
        -9, -8, -7,
        -1,       1,
         7,  8,  9
    };

    // Evaluate pressure around White's king
    if (whiteKingSquare != -1) {
        int kingFile = whiteKingSquare % 8;
        int kingRank = whiteKingSquare / 8;

        for (int direction : directions) {
            int targetSquare = whiteKingSquare + direction;

            if (targetSquare < 0 || targetSquare >= squareCount) {
                continue;
            }

            int targetFile = targetSquare % 8;
            int targetRank = targetSquare / 8;

            // Prevent wrapping around board edges
            if (std::abs(targetFile - kingFile) > 1 ||
                std::abs(targetRank - kingRank) > 1) {
                continue;
            }

            // penalise when the neighbour square is attacked by the enemy
            if (AttackValidator::isSquareUnderAttack(
                    board, targetSquare, false)) {
                score -= 10;
            }
        }
    }

    // Evaluate pressure around Black's king
    if (blackKingSquare != -1) {
        int kingFile = blackKingSquare % 8;
        int kingRank = blackKingSquare / 8;

        for (int direction : directions) {
            int targetSquare = blackKingSquare + direction;

            if (targetSquare < 0 || targetSquare >= squareCount) {
                continue;
            }

            int targetFile = targetSquare % 8;
            int targetRank = targetSquare / 8;

            // Prevent wrapping around board edges
            if (std::abs(targetFile - kingFile) > 1 ||
                std::abs(targetRank - kingRank) > 1) {
                continue;
            }

            // reward when the neighbour square is attacked by the enemy
            if (AttackValidator::isSquareUnderAttack(
                    board, targetSquare, true)) {
                score += 10;
            }
        }
    }

    return score;
}

// turns pressure from unique enemy attackers into king danger, scaled by game phase, positive favours white
int KingSafety::evaluateKingAttackers(const ChessBoard &board, int gamePhase) {

    const int directions[8] = {
        -9, -8, -7,
        -1,       1,
         7,  8,  9
    };

    // sums weighted attacker counts around the king, helper not called at the moment
    auto calculatePressure = [&](int kingSquare, bool attackerIsWhite) {

        if (kingSquare < 0 || kingSquare >= squareCount) {
            return 0;
        }

        int pressure = 0;

        int kingFile = kingSquare % 8;
        int kingRank = kingSquare / 8;

        for (int direction : directions) {

            int targetSquare = kingSquare + direction;

            if (targetSquare < 0 || targetSquare >= squareCount) {
                continue;
            }

            int targetFile = targetSquare % 8;
            int targetRank = targetSquare / 8;

            // skip squares that wrapped around the board edge, file or rank jump above one
            if (std::abs(targetFile - kingFile) > 1 ||
                std::abs(targetRank - kingRank) > 1) {
                continue;
            }

            // weights are pawn five, knight ten, bishop ten, rook fifteen, queen twenty
            if (attackerIsWhite) {
                pressure += KingAttackers::countAttackers(board, targetSquare, true, whitePawn) * 5;
                pressure += KingAttackers::countAttackers(board, targetSquare, true, whiteKnight) * 10;
                pressure += KingAttackers::countAttackers(board, targetSquare, true, whiteBishop) * 10;
                pressure += KingAttackers::countAttackers(board, targetSquare, true, whiteRook) * 15;
                pressure += KingAttackers::countAttackers(board, targetSquare, true, whiteQueen) * 20;
            }
            else {
                pressure += KingAttackers::countAttackers(board, targetSquare, false, blackPawn) * 5;
                pressure += KingAttackers::countAttackers(board, targetSquare, false, blackKnight) * 10;
                pressure += KingAttackers::countAttackers(board, targetSquare, false, blackBishop) * 10;
                pressure += KingAttackers::countAttackers(board, targetSquare, false, blackRook) * 15;
                pressure += KingAttackers::countAttackers(board, targetSquare, false, blackQueen) * 20;
            }
        }

        return pressure;
    };

    // converts pressure into a danger value using the lookup table
    auto calculateKingDanger = [](int pressure) {

        // table step is five pressure points, clamped to the last entry
        int index = pressure / 5;

        if (index > 20) {
            index = 20;
        }

        return EvalConstants::KING_DANGER_TABLE[index];
    };

    int whiteKingSquare = -1;
    int blackKingSquare = -1;

    // Find both kings
    for (int square = 0; square < squareCount; ++square) {

        Piece piece = board.getPieceAt(square);

        if (piece == whiteKing) {
            whiteKingSquare = square;
        }
        else if (piece == blackKing) {
            blackKingSquare = square;
        }
    }

    // Calculate enemy pressure around each king
    int whiteKingPressure =
    KingAttackers::evaluateUniqueKingAttackers(board, whiteKingSquare, false);

    int blackKingPressure =
        KingAttackers::evaluateUniqueKingAttackers(board, blackKingSquare, true);

    // Return White-perspective score
    int whiteKingDanger = calculateKingDanger(whiteKingPressure);
    int blackKingDanger = calculateKingDanger(blackKingPressure);

    // more danger for black is good for white
    int score = blackKingDanger - whiteKingDanger;

    // fade out as pieces leave the board
    return (score * gamePhase) / 24;
}

// combines pawn shield, open files and attacker pressure into one king safety score
int KingSafety::evaluateKingSafety(const ChessBoard &board, int gamePhase) {
    // Calculate individual king-safety scores
    int pawnShieldScore = evaluatePawnShield(board);

    int openFilesScore = evaluateKingOpenFiles(board);

    int attackerPressureScore = evaluateKingAttackers(board, gamePhase);

    // Scale the features that aren't already phase-aware
    pawnShieldScore = (pawnShieldScore * gamePhase) / 24;

    openFilesScore = (openFilesScore * gamePhase) / 24;

    // Combine all three scores
    return pawnShieldScore
         + openFilesScore
         + attackerPressureScore;
}
