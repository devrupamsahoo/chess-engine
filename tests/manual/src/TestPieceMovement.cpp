//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/TestPieceMovement.h"
#include "../include/ManualTestUtils.h"

#include "chess/core/GameSession.h"

#include <string>

namespace ManualTests {
    // tests that knights can jump over other pieces
    void testKnightJumpsOverPieces() {
        GameSession session;

        const std::string position =
                "k7/8/8/8/8/8/1PP5/1N2K3 w - - 0 1";

        expect(session.loadFen(position), "load knight jumping position");

        expect(session.makeMove(b1, c3),
               "knight jumps over friendly pawns");
    }

    // tests that a piece cannot capture another friendly piece
    void testCannotCaptureFriendlyPiece() {
        GameSession session;

        const std::string position =
                "k7/8/8/8/8/8/3P4/1N2K3 w - - 0 1";

        expect(session.loadFen(position), "load friendly piece position");

        expect(!session.makeMove(b1, d2),
               "knight cannot capture its own pawn");

        expect(session.currentFen() == position,
               "friendly piece capture leaves the position unchanged");
    }

    // tests that bishops cannot move through occupied squares
    void testBishopBlockedByFriendlyPiece() {
        GameSession session;

        const std::string position =
                "k7/8/8/8/8/8/3P4/2B1K3 w - - 0 1";

        expect(session.loadFen(position), "load blocked bishop position");

        expect(!session.makeMove(c1, e3),
               "bishop cannot jump over a friendly pawn");

        expect(session.currentFen() == position,
               "blocked bishop move leaves the position unchanged");
    }

    // tests that rooks cannot move through occupied squares
    void testRookBlockedByFriendlyPiece() {
        GameSession session;

        const std::string position =
                "k7/8/8/8/8/8/P7/R3K3 w - - 0 1";

        expect(session.loadFen(position), "load blocked rook position");

        expect(!session.makeMove(a1, a8),
               "rook cannot jump over a friendly pawn");

        expect(session.currentFen() == position,
               "blocked rook move leaves the position unchanged");
    }

    // tests diagonal queen movement
    void testQueenDiagonalMovement() {
        GameSession session;

        const std::string position =
                "k7/8/8/8/8/8/8/3QK3 w - - 0 1";

        expect(session.loadFen(position), "load queen movement position");

        expect(session.makeMove(d1, h5),
               "queen moves along an unobstructed diagonal");
    }

    // tests whether rooks can capture enemy pieces
    void testRookCanCaptureEnemyPiece() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/n7/8/8/R6K w - - 0 1"),
               "load rook capture position");

        expect(session.makeMove(a1, a4),
               "rook captures enemy knight");

        expect(session.capturedPieces().size() == 1,
               "rook capture records captured piece");
    }

    // tests that rooks cannot jump over enemy pieces
    void testRookCannotJumpOverEnemyPiece() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/8/p7/8/R6K w - - 0 1"),
               "load blocked rook position");

        expect(!session.makeMove(a1, a4),
               "rook cannot jump over enemy piece");
    }

    // tests whether bishops can capture enemy pieces
    void testBishopCanCaptureEnemyPiece() {
        GameSession session;

        expect(session.loadFen("k7/8/8/8/3p4/2B5/8/7K w - - 0 1"),
               "load bishop capture position");

        expect(session.makeMove(c3, d4),
               "bishop captures enemy pawn");

        expect(session.capturedPieces().size() == 1,
               "bishop capture records captured piece");
    }

    // tests that queens cannot jump over enemy pieces
    void testQueenCannotJumpOverEnemyPiece() {
        GameSession session;

        expect(session.loadFen("7k/8/8/8/8/3p4/8/3Q3K w - - 0 1"),
               "load blocked queen position");

        expect(!session.makeMove(d1, d4),
               "queen cannot jump over enemy piece");
    }
}
