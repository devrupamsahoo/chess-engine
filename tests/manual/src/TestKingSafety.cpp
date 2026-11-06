//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/TestKingSafety.h"
#include "../include/ManualTestUtils.h"

#include "chess/core/GameSession.h"

#include <string>

namespace ManualTests {

    // tests that the king cannot move onto an attacked square
    void testKingCannotMoveIntoCheck() {
        GameSession session;

        const std::string startingPosition =
                "k3r3/8/8/8/8/8/8/3K4 w - - 0 1";

        expect(session.loadFen(startingPosition),
               "load the king safety position");

        expect(!session.makeMove(d1, e1),
               "king cannot move onto a square attacked by a rook");

        expect(session.currentFen() == startingPosition,
               "rejected king move leaves the position unchanged");
    }

    // tests that a player cannot make an unrelated move while in check
    void testCannotIgnoreCheck() {
        GameSession session;

        const std::string startingPosition =
                "k3r3/8/8/8/8/8/8/4K2R w - - 0 1";

        expect(session.loadFen(startingPosition),
               "load the position where white is in check");

        expect(!session.makeMove(h1, h2),
               "player cannot ignore check by moving another piece");

        expect(session.currentFen() == startingPosition,
               "rejected move leaves the checked position unchanged");
    }

    // tests that a pinned piece cannot expose its king to check
    void testPinnedPieceCannotMove() {
        GameSession session;

        const std::string position =
                "k3r3/8/8/8/8/8/4R3/4K3 w - - 0 1";

        expect(session.loadFen(position), "load pinned piece position");

        expect(!session.makeMove(e2, f2),
               "pinned rook cannot move away from the king's file");

        expect(session.currentFen() == position,
               "illegal pinned piece move leaves the position unchanged");
    }

    // tests that a king cannot move next to the enemy king
    void testKingsCannotBecomeAdjacent() {
        GameSession session;

        const std::string position =
                "8/8/8/8/8/4k3/8/4K3 w - - 0 1";

        expect(session.loadFen(position), "load adjacent king position");

        expect(!session.makeMove(e1, e2),
               "king cannot move next to the enemy king");

        expect(session.currentFen() == position,
               "illegal king move leaves the position unchanged");
    }

    // tests that a king cannot capture a piece protected by another enemy piece
    void testKingCannotCaptureProtectedPiece() {
        GameSession session;

        const std::string position =
                "k3r3/8/8/8/8/8/4r3/4K3 w - - 0 1";

        expect(session.loadFen(position), "load protected piece position");

        expect(!session.makeMove(e1, e2),
               "king cannot capture a rook protected by another rook");

        expect(session.currentFen() == position,
               "illegal king capture leaves the position unchanged");
    }

    // tests that a player can block a sliding check
    void testBlockCheck() {
        GameSession session;

        const std::string position =
                "k3r3/8/8/8/2B5/8/8/4K3 w - - 0 1";

        expect(session.loadFen(position), "load block check position");

        expect(session.makeMove(c4, e2),
               "bishop blocks the rook check");
    }

    // tests that a player cannot resolve double check by moving another piece
    void testDoubleCheckOnlyAllowsKingMove() {
        GameSession session;

        const std::string position =
                "k3r3/8/8/8/1b6/8/8/4K2R w - - 0 1";

        expect(session.loadFen(position), "load double check position");

        expect(!session.makeMove(h1, h2),
               "rook cannot move while the king is under double check");

        expect(session.currentFen() == position,
               "illegal double check response leaves the position unchanged");
    }

    // tests that a pinned piece can move along the pin
    void testPinnedPieceCanMoveAlongPin() {
        GameSession session;

        expect(session.loadFen("k3r3/8/8/8/8/8/4R3/4K3 w - - 0 1"),
               "load pinned piece position");

        expect(session.makeMove(e2, e3),
               "pinned rook can move along pin");
    }

    // tests that the king cannot move more than one square
    void testKingCannotMoveMoreThanOneSquare() {
        GameSession session;

        expect(session.loadFen("k7/8/8/8/8/8/8/4K3 w - - 0 1"),
               "load king movement position");

        expect(!session.makeMove(e1, e3),
               "king cannot move more than one square");
    }

}