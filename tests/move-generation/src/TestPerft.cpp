//
// Created by devrupamsahoo on 05-11-2026.
//

#include "chess/core/ChessBoard.h"
#include "chess/movegen/MoveGeneration.h"
#include "chess/notation/FEN.h"
#include "ManualTestUtils.h"


// tests the expected number of pseudo-legal moves in the standard starting position
bool testStartingPositionMoveCount() {
    const ChessBoard board;
    const auto moves = MoveGeneration::generatePseudoLegalMoves(board);

    return moves.size() == 20;
}

// tests that a rook can move along an open file but cannot pass through a friendly piece
bool testRookStopsAtFriendlyPiece() {
    ChessBoard board;
    if (!FEN::load(board, "7k/8/8/8/8/8/8/R3K3 w - - 0 1")) {
        return false;
    }

    bool rookCanReachA8 = false;

    for (const auto &move: MoveGeneration::generatePseudoLegalMoves(board)) {
        if (move.startSquare == a1 && move.endSquare == a8) {
            rookCanReachA8 = true;
        }

        if (move.startSquare == a1 && move.endSquare == e1) {
            return false; // e1 has White's king
        }
    }

    return rookCanReachA8;
}

// runs all manual move-generation tests and records their results
void runMoveGenerationTests() {
    ManualTests::expect(
        testStartingPositionMoveCount(),
        "starting position generates 20 pseudo-legal moves");

    ManualTests::expect(
        testRookStopsAtFriendlyPiece(),
        "rook stops before a friendly piece");
}
