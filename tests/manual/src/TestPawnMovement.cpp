//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/TestPawnMovement.h"
#include "../include/ManualTestUtils.h"

#include "chess/core/GameSession.h"

#include <string>

namespace ManualTests {
    // tests that a pawn cannot move forward when its path is blocked
    void testPawnForwardBlocked() {
        GameSession session;

        const std::string position =
                "k7/8/8/8/4p3/4P3/8/7K w - - 0 1";

        expect(session.loadFen(position), "load blocked pawn position");

        expect(!session.makeMove(e3, e4),
               "pawn cannot move forward into an occupied square");

        expect(!session.makeMove(e3, e5),
               "pawn cannot jump over an occupied square");

        expect(session.currentFen() == position,
               "blocked pawn moves leave the position unchanged");
    }

    // tests that a pawn can capture an opponent's piece diagonally
    void testPawnCaptures() {
        GameSession session;

        expect(session.loadFen("7k/8/8/3p4/4P3/8/8/7K w - - 0 1"),
               "load pawn capture position");

        expect(session.makeMove(e4, d5),
               "white pawn captures black pawn");
    }

    // tests that a pawn cannot move backward
    void testPawnCannotMoveBackward() {
        GameSession session;

        expect(!session.makeMove(e2, e1),
               "pawn cannot move backward");
    }

    // tests that a pawn can move two squares from its starting position
    void testPawnDoubleStepFromStartingPosition() {
        GameSession session;

        expect(session.makeMove(e2, e4),
               "pawn can double step from starting position");
    }

    // tests that a pawn cannot move two squares after leaving its starting position
    void testPawnCannotDoubleStepFromNonStartingPosition() {
        GameSession session;

        expect(session.makeMove(e2, e3),
               "pawn moves one square forward");

        expect(!session.makeMove(e3, e5),
               "pawn cannot double step from non-starting position");
    }

    // tests that a pawn cannot double step when the intermediate square is blocked
    void testPawnDoubleStepBlockedAtIntermediateSquare() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/4N3/8/4P3/7K w - - 0 1"),
               "load blocked pawn double step position");

        expect(!session.makeMove(e2, e4),
               "pawn cannot double step when intermediate square is blocked");
    }

    // tests that a pawn cannot capture an opponent's piece directly forward
    void testPawnCannotCaptureStraightForward() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/4r3/8/4P3/7K w - - 0 1"),
               "load pawn forward capture position");

        expect(!session.makeMove(e2, e4),
               "pawn cannot capture straight forward");
    }

    // tests that a black pawn double step sets the correct en passant target
    void testBlackPawnDoubleStepUpdatesEnPassantTarget() {
        GameSession session;

        expect(session.loadFen("7k/4p3/8/8/8/8/8/7K b - - 0 1"),
               "load black pawn double step position");

        expect(session.makeMove(e7, e5),
               "black pawn double step");

        expect(session.currentFen() ==
               "7k/8/8/4p3/8/8/8/7K w - e6 0 2",
               "black double step sets en passant target");
    }
}
