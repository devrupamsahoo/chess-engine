//
// Created by devrupamsahoo on 08-11-2026.
//

#include "chess/evaluation/Evaluation.h"
#include "chess/evaluation/EvaluationConstants.h"
#include "chess/evaluation/GamePhase.h"
#include "chess/evaluation/PieceBonuses.h"
#include "chess/evaluation/PawnStructure.h"
#include "chess/evaluation/KingSafety.h"
#include "chess/evaluation/Mobility.h"
#include <algorithm>


// calculates material balance from White's perspective, excluding all positional terms and piece-square tables.
int Evaluation::evaluateMaterial(const ChessBoard &board) {
    int materialScore = 0;

    for (int square = 0; square < squareCount; ++square) {
        const Piece piece = board.getPieceAt(square);

        switch (piece) {
            case whitePawn:
                materialScore += EvalConstants::PAWN_VALUE;
                break;

            case blackPawn:
                materialScore -= EvalConstants::PAWN_VALUE;
                break;

            case whiteKnight:
                materialScore += EvalConstants::KNIGHT_VALUE;
                break;

            case blackKnight:
                materialScore -= EvalConstants::KNIGHT_VALUE;
                break;

            case whiteBishop:
                materialScore += EvalConstants::BISHOP_VALUE;
                break;

            case blackBishop:
                materialScore -= EvalConstants::BISHOP_VALUE;
                break;

            case whiteRook:
                materialScore += EvalConstants::ROOK_VALUE;
                break;

            case blackRook:
                materialScore -= EvalConstants::ROOK_VALUE;
                break;

            case whiteQueen:
                materialScore += EvalConstants::QUEEN_VALUE;
                break;

            case blackQueen:
                materialScore -= EvalConstants::QUEEN_VALUE;
                break;

            case whiteKing:
                materialScore += EvalConstants::KING_VALUE;
                break;

            case blackKing:
                materialScore -= EvalConstants::KING_VALUE;
                break;

            case noPiece:
                break;
        }
    }

    return materialScore;
}


// main entry point, sums material, piece square tables and every positional term, returns score for the side to move
int Evaluation::evaluate(const ChessBoard &board) {
    // running score, positive favours white until the final flip
    int whitePerspectiveScore = 0;

    // game phase from zero endgame to twenty four opening, blends the king tables
    int phase = std::clamp(GamePhase::calculateGamePhase(board), 0, 24);

    // walk every square, add material and table value per piece
    for (int square = 0; square < squareCount; ++square) {
        const Piece piece = board.getPieceAt(square);

        switch (piece) {
            // white pieces add score, tables are stored from rank eight so white squares are mirrored
            case whitePawn:
                whitePerspectiveScore += EvalConstants::PAWN_VALUE;
                whitePerspectiveScore += EvalConstants::PAWN_PST[square ^ 56];
                break;

            // black pieces subtract score and read the tables directly
            case blackPawn:
                whitePerspectiveScore -= EvalConstants::PAWN_VALUE;
                whitePerspectiveScore -= EvalConstants::PAWN_PST[square];
                break;

            case whiteKnight:
                whitePerspectiveScore += EvalConstants::KNIGHT_VALUE;
                whitePerspectiveScore += EvalConstants::KNIGHT_PST[square ^ 56];
                break;

            case blackKnight:
                whitePerspectiveScore -= EvalConstants::KNIGHT_VALUE;
                whitePerspectiveScore -= EvalConstants::KNIGHT_PST[square];
                break;

            case whiteBishop:
                whitePerspectiveScore += EvalConstants::BISHOP_VALUE;
                whitePerspectiveScore += EvalConstants::BISHOP_PST[square ^ 56];
                break;

            case blackBishop:
                whitePerspectiveScore -= EvalConstants::BISHOP_VALUE;
                whitePerspectiveScore -= EvalConstants::BISHOP_PST[square];
                break;

            // rooks also get file and seventh rank bonuses
            case whiteRook:
                whitePerspectiveScore += EvalConstants::ROOK_VALUE;
                whitePerspectiveScore += EvalConstants::ROOK_PST[square ^ 56];
                whitePerspectiveScore += PieceBonuses::evaluateRookFile(board, square, whiteRook);
                whitePerspectiveScore += PieceBonuses::evaluateRookSeventhRank(square, whiteRook);
                break;

            case blackRook:
                whitePerspectiveScore -= EvalConstants::ROOK_VALUE;
                whitePerspectiveScore -= EvalConstants::ROOK_PST[square];
                whitePerspectiveScore -= PieceBonuses::evaluateRookFile(board, square, blackRook);
                whitePerspectiveScore -= PieceBonuses::evaluateRookSeventhRank(square, blackRook);
                break;

            case whiteQueen:
                whitePerspectiveScore += EvalConstants::QUEEN_VALUE;
                whitePerspectiveScore += EvalConstants::QUEEN_PST[square ^ 56];
                break;

            case blackQueen:
                whitePerspectiveScore -= EvalConstants::QUEEN_VALUE;
                whitePerspectiveScore -= EvalConstants::QUEEN_PST[square];
                break;

            // kings blend middlegame and endgame tables by game phase
            case whiteKing: {
                int index = square ^ 56;

                int mgScore = EvalConstants::KING_MIDDLEGAME_PST[index];
                int egScore = EvalConstants::KING_ENDGAME_PST[index];

                int kingScore = (mgScore * phase + egScore * (24 - phase)) / 24;

                whitePerspectiveScore += kingScore;
                break;
            }

            case blackKing: {
                int mgScore = EvalConstants::KING_MIDDLEGAME_PST[square];
                int egScore = EvalConstants::KING_ENDGAME_PST[square];

                int kingScore = (mgScore * phase + egScore * (24 - phase)) / 24;

                whitePerspectiveScore -= kingScore;
                break;
            }

            case noPiece:
                break;
        }
    }

    // add positional terms, each already returns a white perspective score
    whitePerspectiveScore += PieceBonuses::evaluateBishopPair(board);

    whitePerspectiveScore += PawnStructure::evaluateDoubledPawns(board);

    whitePerspectiveScore += PawnStructure::evaluateIsolatedPawns(board);

    whitePerspectiveScore += PawnStructure::evaluatePassedPawns(board);

    whitePerspectiveScore += KingSafety::evaluateKingSafety(board, phase);

    whitePerspectiveScore += Mobility::evaluateMobility(board);

    // positive means side to move is better
    return board.isWhiteTurn() ? whitePerspectiveScore : -whitePerspectiveScore;
}

int Evaluation::evaluatePieceSquareTables(const ChessBoard &board) {
    int whitePerspectiveScore = 0;

    const int phase = std::clamp(
        GamePhase::calculateGamePhase(board),
        0,
        24
    );

    for (int square = 0; square < squareCount; ++square) {
        const Piece piece = board.getPieceAt(square);

        switch (piece) {
            case whitePawn:
                whitePerspectiveScore +=
                    EvalConstants::PAWN_PST[square ^ 56];
                break;

            case blackPawn:
                whitePerspectiveScore -=
                    EvalConstants::PAWN_PST[square];
                break;

            case whiteKnight:
                whitePerspectiveScore +=
                    EvalConstants::KNIGHT_PST[square ^ 56];
                break;

            case blackKnight:
                whitePerspectiveScore -=
                    EvalConstants::KNIGHT_PST[square];
                break;

            case whiteBishop:
                whitePerspectiveScore +=
                    EvalConstants::BISHOP_PST[square ^ 56];
                break;

            case blackBishop:
                whitePerspectiveScore -=
                    EvalConstants::BISHOP_PST[square];
                break;

            case whiteRook:
                whitePerspectiveScore +=
                    EvalConstants::ROOK_PST[square ^ 56];
                break;

            case blackRook:
                whitePerspectiveScore -=
                    EvalConstants::ROOK_PST[square];
                break;

            case whiteQueen:
                whitePerspectiveScore +=
                    EvalConstants::QUEEN_PST[square ^ 56];
                break;

            case blackQueen:
                whitePerspectiveScore -=
                    EvalConstants::QUEEN_PST[square];
                break;

            case whiteKing: {
                const int index = square ^ 56;

                const int mgScore =
                    EvalConstants::KING_MIDDLEGAME_PST[index];

                const int egScore =
                    EvalConstants::KING_ENDGAME_PST[index];

                whitePerspectiveScore +=
                    (mgScore * phase + egScore * (24 - phase)) / 24;

                break;
            }

            case blackKing: {
                const int mgScore =
                    EvalConstants::KING_MIDDLEGAME_PST[square];

                const int egScore =
                    EvalConstants::KING_ENDGAME_PST[square];

                whitePerspectiveScore -=
                    (mgScore * phase + egScore * (24 - phase)) / 24;

                break;
            }

            case noPiece:
                break;
        }
    }

    return whitePerspectiveScore;
}