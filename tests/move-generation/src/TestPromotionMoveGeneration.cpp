//
// Created by devrupamsahoo on 08-11-2026.
//

#include "../include/TestPromotionMoveGeneration.h"

#include "chess/core/ChessBoard.h"
#include "chess/movegen/MoveGeneration.h"
#include "chess/notation/FEN.h"

namespace MoveGenerationTests {

    bool testAllPromotionChoicesAreLegal() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "7k/P7/8/8/8/8/8/4K3 w - - 0 1")) {
            return false;
        }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        int promotionCount = 0;

        for (const PseudoLegalMove &move : legalMoves) {
            if (move.startSquare == a7 &&
                move.endSquare == a8 &&
                move.promotion != noPiece) {
                ++promotionCount;
            }
        }

        return promotionCount == 4;
    }

    bool testPromotionExposingKingIsFiltered() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "k2nr3/4P3/8/8/8/8/8/4K3 w - - 0 1")) {
            return false;
        }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        int pseudoPromotionCount = 0;
        int legalPromotionCount = 0;

        for (const PseudoLegalMove &move : pseudoMoves) {
            if (move.startSquare == e7 &&
                move.endSquare == d8 &&
                move.promotion != noPiece) {
                ++pseudoPromotionCount;
            }
        }

        for (const PseudoLegalMove &move : legalMoves) {
            if (move.startSquare == e7 &&
                move.endSquare == d8 &&
                move.promotion != noPiece) {
                ++legalPromotionCount;
            }
        }

        return pseudoPromotionCount == 4 &&
               legalPromotionCount == 0;
    }

}
