//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/TestMoveHistory.h"
#include "../include/ManualTestUtils.h"

#include "chess/core/GameSession.h"

#include <string>

namespace ManualTests {
    // tests move history restoration across undo and redo operations

    // tests that multiple undo and redo operations restore the correct positions
    void testMultipleUndoRedo() {
        GameSession session;

        expect(session.makeMove(e2, e4),
               "white plays e2-e4");

        expect(session.makeMove(e7, e5),
               "black plays e7-e5");

        const std::string positionAfterTwoMoves =
                session.currentFen();

        expect(session.makeMove(g1, f3),
               "white plays g1-f3");

        expect(session.makeMove(b8, c6),
               "black plays b8-c6");

        const std::string finalPosition =
                session.currentFen();

        expect(session.undo(), "undo black's second move");
        expect(session.undo(), "undo white's second move");

        expect(session.currentFen() == positionAfterTwoMoves,
               "multiple undos restore the earlier position");

        expect(session.redo(), "redo white's second move");
        expect(session.redo(), "redo black's second move");

        expect(session.currentFen() == finalPosition,
               "multiple redos restore the final position");
    }

    // tests whether undo and redo correctly restore captured pieces
    void testCaptureUndoRedo() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/8/8/p7/R6K w - - 0 1"),
               "load capture undo redo position");

        expect(session.makeMove(a1, a2),
               "rook captures pawn");

        expect(session.capturedPieces().size() == 1,
               "captured piece recorded");

        expect(session.undo(),
               "undo capture");

        expect(session.currentFen() ==
               "7k/8/8/8/8/8/p7/R6K w - - 0 1",
               "undo restores captured pawn");

        expect(session.capturedPieces().empty(),
               "undo removes captured piece from captured list");

        expect(session.redo(),
               "redo capture");

        expect(session.capturedPieces().size() == 1,
               "redo restores captured piece record");
    }
}
