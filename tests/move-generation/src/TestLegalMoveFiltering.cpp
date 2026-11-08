//
// Created by devrupamsahoo on 08-11-2026.
//

#include "../include/TestLegalMoveFiltering.h"

#include "chess/core/ChessBoard.h"
#include "chess/movegen/MoveGeneration.h"
#include "chess/notation/FEN.h"
#include "chess/move/KingValidator.h"

#include <algorithm>

namespace MoveGenerationTests {

    bool testMoveThatExposesKingIsFiltered() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "4r1k1/8/8/8/8/8/4R3/4K3 w - - 0 1")) {
            return false;
        }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        const auto isRookMove = [](const PseudoLegalMove &move) {
            return move.startSquare == e2 &&
                   move.endSquare == a2;
        };

        const bool existsInPseudoMoves =
            std::any_of(
                pseudoMoves.begin(),
                pseudoMoves.end(),
                isRookMove);

        const bool existsInLegalMoves =
            std::any_of(
                legalMoves.begin(),
                legalMoves.end(),
                isRookMove);

        return existsInPseudoMoves && !existsInLegalMoves;
    }

    bool testKingInCheckOnlyAllowsSafeMoves() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "4r1k1/8/8/8/8/8/8/R3K3 w - - 0 1")) {
            return false;
        }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        for (const PseudoLegalMove &move : legalMoves) {
            ChessBoard copyBoard(board);

            copyBoard.makeMove(
                move.startSquare,
                move.endSquare,
                move.promotion,
                true);

            if (KingValidator::isKingInCheck(copyBoard, true)) {
                return false;
            }
        }

        return !legalMoves.empty();
    }

}
