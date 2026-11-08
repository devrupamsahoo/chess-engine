//
// Created by devrupamsahoo on 08-11-2026.
//

#include "../include/TestCastlingMoveGeneration.h"

#include "chess/core/ChessBoard.h"
#include "chess/movegen/MoveGeneration.h"
#include "chess/notation/FEN.h"

#include <algorithm>

namespace MoveGenerationTests {

    bool testCastlingThroughAttackIsFiltered() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "k4r2/8/8/8/8/8/8/4K2R w K - 0 1")) {
            return false;
        }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        const auto isKingsideCastle = [](const PseudoLegalMove &move) {
            return move.castling == whiteKingside;
        };

        const bool existsInPseudoMoves =
            std::any_of(
                pseudoMoves.begin(),
                pseudoMoves.end(),
                isKingsideCastle);

        const bool existsInLegalMoves =
            std::any_of(
                legalMoves.begin(),
                legalMoves.end(),
                isKingsideCastle);

        return existsInPseudoMoves && !existsInLegalMoves;
    }

    bool testValidCastlingIsKept() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "k7/8/8/8/8/8/8/4K2R w K - 0 1")) {
            return false;
        }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        return std::any_of(
            legalMoves.begin(),
            legalMoves.end(),
            [](const PseudoLegalMove &move) {
                return move.castling == whiteKingside;
            });
    }

    bool testWhiteQueensideCastlingIsKept() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "7k/8/8/8/8/8/8/R3K3 w Q - 0 1")) {
            return false;
        }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        return std::any_of(
            legalMoves.begin(),
            legalMoves.end(),
            [](const PseudoLegalMove &move) {
                return move.castling == whiteQueenside;
            });
    }

    bool testBlackKingsideCastlingIsKept() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "4k2r/8/8/8/8/8/8/K7 b k - 0 1")) {
            return false;
        }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        return std::any_of(
            legalMoves.begin(),
            legalMoves.end(),
            [](const PseudoLegalMove &move) {
                return move.castling == blackKingside;
            });
    }

    bool testBlackCastlingThroughAttackIsFiltered() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "4k2r/8/8/8/8/8/8/K4R2 b k - 0 1")) {
            return false;
        }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        const auto isBlackKingsideCastle = [](const PseudoLegalMove &move) {
            return move.castling == blackKingside;
        };

        const bool existsInPseudoMoves =
            std::any_of(
                pseudoMoves.begin(),
                pseudoMoves.end(),
                isBlackKingsideCastle);

        const bool existsInLegalMoves =
            std::any_of(
                legalMoves.begin(),
                legalMoves.end(),
                isBlackKingsideCastle);

        return existsInPseudoMoves && !existsInLegalMoves;
    }

}
