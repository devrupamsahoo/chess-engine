//
// Created by devrupamsahoo on 08-11-2026.
//

#include "../include/TestPseudoLegalMoveGeneration.h"

#include "chess/core/ChessBoard.h"
#include "chess/movegen/MoveGeneration.h"
#include "chess/notation/FEN.h"

namespace MoveGenerationTests {

    bool testStartingPositionMoveCount() {
        const ChessBoard board;

        const auto moves =
            MoveGeneration::generatePseudoLegalMoves(board);

        return moves.size() == 20;
    }

    bool testRookStopsAtFriendlyPiece() {
        ChessBoard board;

        if (!FEN::load(
            board,
            "7k/8/8/8/8/8/8/R3K3 w - - 0 1")) {
            return false;
            }

        bool rookCanReachA8 = false;

        for (const auto &move :
             MoveGeneration::generatePseudoLegalMoves(board)) {
            if (move.startSquare == a1 &&
                move.endSquare == a8) {
                rookCanReachA8 = true;
                }

            if (move.startSquare == a1 &&
                move.endSquare == e1) {
                return false;
                }
             }

        return rookCanReachA8;
    }

}
