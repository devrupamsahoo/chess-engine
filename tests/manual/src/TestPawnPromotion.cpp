//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/TestPawnPromotion.h"
#include "../include/ManualTestUtils.h"

#include "chess/core/GameSession.h"

#include <string>

namespace ManualTests {
       // tests pawn promotion for both colors, capture promotion, and undo/redo behavior

       // tests white pawn promotion on the final rank
    void testWhitePawnPromotion() {
        GameSession session;

        expect(session.loadFen("7k/P7/8/8/8/8/8/7K w - - 0 1"),
               "load white promotion position");

        expect(session.makeMove(a7, a8),
               "white pawn promotes on final rank");

        expect(session.currentFen() ==
               "Q6k/8/8/8/8/8/8/7K b - - 0 1",
               "white pawn promotes to queen");
    }

       // tests black pawn promotion on the final rank
    void testBlackPawnPromotion() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/8/8/p7/7K b - - 0 1"),
               "load black promotion position");

        expect(session.makeMove(a2, a1),
               "black pawn promotes on final rank");

        expect(session.currentFen() ==
               "7k/8/8/8/8/8/8/q6K w - - 0 2",
               "black pawn promotes to queen");
    }

       // tests promotion when a pawn captures an enemy piece on the final rank
    void testPawnPromotionByCapture() {
        GameSession session;

        expect(session.loadFen("r6k/1P6/8/8/8/8/8/7K w - - 0 1"),
               "load promotion capture position");

        expect(session.makeMove(b7, a8),
               "pawn captures and promotes");

        expect(session.currentFen() ==
               "Q6k/8/8/8/8/8/8/7K b - - 0 1",
               "promotion capture produces promoted piece");

        expect(session.capturedPieces().size() == 1,
               "promotion capture records captured rook");
    }

       // tests whether undo and redo correctly restore the pawn and promoted piece
    void testPromotionUndoRedo() {
        GameSession session;

        expect(session.loadFen("7k/P7/8/8/8/8/8/7K w - - 0 1"),
               "load promotion undo redo position");

        expect(session.makeMove(a7, a8),
               "make promotion move");

        std::string promotedFen = session.currentFen();

        expect(session.undo(), "undo promotion");

        expect(session.currentFen() ==
               "7k/P7/8/8/8/8/8/7K w - - 0 1",
               "undo restores original pawn");

        expect(session.redo(), "redo promotion");

        expect(session.currentFen() == promotedFen,
               "redo restores promoted piece");
    }

}