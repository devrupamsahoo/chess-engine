
//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/TestMoveValidation.h"
#include "../include/ManualTestUtils.h"

#include "chess/core/GameSession.h"

#include <string>

namespace ManualTests {

    // tests that invalid moves are rejected without changing the position
    void testInvalidMoves() {
        GameSession session;

        const std::string startingPosition = session.currentFen();

        expect(!session.makeMove(e7, e5),
               "white cannot move a black pawn");

        expect(session.currentFen() == startingPosition,
               "invalid move leaves the board unchanged");

        expect(!session.makeMove(e2, e2),
               "piece cannot move to its current square");

        expect(session.currentFen() == startingPosition,
               "same-square move leaves the board unchanged");
    }

    // tests that a player cannot move the opponent's piece
    void testCannotMoveOpponentPiece() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/8/8/4P3/7K b - - 0 1"),
               "load wrong turn position");

        expect(!session.makeMove(e2, e4),
               "black cannot move white pawn");
    }

}
