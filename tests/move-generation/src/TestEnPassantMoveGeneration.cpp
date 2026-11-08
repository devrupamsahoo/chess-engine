//
// Created by devrupamsahoo on 08-11-2026.
//

#include "../include/TestEnPassantMoveGeneration.h"

#include "chess/core/ChessBoard.h"
#include "chess/movegen/MoveGeneration.h"
#include "chess/notation/FEN.h"

#include <algorithm>

namespace MoveGenerationTests {

    bool testEnPassantExposingKingIsFiltered() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "4r2k/8/8/3pP3/8/8/8/4K3 w - d6 0 1")) {
            return false;
            }

        const auto pseudoMoves =
            MoveGeneration::generatePseudoLegalMoves(board);

        MoveGeneration generator;

        const auto legalMoves =
            generator.filterLegalMoves(board, pseudoMoves);

        const auto isEnPassantMove = [](const PseudoLegalMove &move) {
            return move.startSquare == e5 &&
                   move.endSquare == d6 &&
                   move.isEnPassant;
        };

        const bool existsInPseudoMoves =
            std::any_of(
                pseudoMoves.begin(),
                pseudoMoves.end(),
                isEnPassantMove);

        const bool existsInLegalMoves =
            std::any_of(
                legalMoves.begin(),
                legalMoves.end(),
                isEnPassantMove);

        return existsInPseudoMoves && !existsInLegalMoves;
    }

}
