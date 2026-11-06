//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/TestFenAndMoveCounters.h"
#include "../include/ManualTestUtils.h"

#include "chess/core/GameSession.h"

#include <string>

namespace ManualTests {
    // tests FEN round trips and halfmove and fullmove counter updates

    // verifies that loading and serializing a FEN preserves the position exactly
    void testFenRoundTrip() {
        GameSession session;

        std::string fen =
                "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 12 25";

        expect(session.loadFen(fen), "load FEN for round trip");

        expect(session.currentFen() == fen,
               "FEN serialization preserves loaded position");
    }

    // tests counter updates after moves that do not involve pawns or captures
    void testMoveCountersAfterQuietMoves() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/8/8/8/1N5K w - - 7 12"),
               "load move counter position");

        expect(session.makeMove(b1, c3),
               "white makes quiet move");

        expect(session.makeMove(h8, g8),
               "black makes quiet move");

        expect(session.currentFen() ==
               "6k1/8/8/8/8/2N5/8/7K w - - 9 13",
               "halfmove and fullmove counters update correctly");
    }

    // tests that a pawn move resets the halfmove clock
    void testPawnMoveResetsHalfmoveClock() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/8/8/4P3/7K w - - 17 9"),
               "load pawn halfmove position");

        expect(session.makeMove(e2, e3),
               "pawn makes quiet move");

        expect(session.currentFen() ==
               "7k/8/8/8/8/4P3/8/7K b - - 0 9",
               "pawn move resets halfmove clock");
    }

    // tests that a capture resets the halfmove clock
    void testCaptureResetsHalfmoveClock() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/8/8/p7/R6K w - - 17 9"),
               "load capture halfmove position");

        expect(session.makeMove(a1, a2),
               "rook captures pawn");

        expect(session.currentFen() ==
               "7k/8/8/8/8/8/R7/7K b - - 0 9",
               "capture resets halfmove clock");
    }
}
