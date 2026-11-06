//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/TestCastling.h"
#include "../include/ManualTestUtils.h"

#include "chess/core/GameSession.h"

#include <string>

namespace ManualTests {
    // tests kingside castling and its undo and redo operations
    void testCastlingUndoRedo() {
        GameSession session;

        const std::string startingPosition =
                "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";

        expect(session.loadFen(startingPosition), "load the castling test position");

        expect(session.makeMove(e1, g1), "white castles kingside");

        const std::string castledPosition =
                "r3k2r/8/8/8/8/8/8/R4RK1 b kq - 1 1";

        expect(session.currentFen() == castledPosition,
               "castling moves the king and rook and clears white castling rights");

        expect(session.undo(), "undo kingside castling");
        expect(session.currentFen() == startingPosition,
               "undo restores the king, rook, and castling rights");

        expect(session.redo(), "redo kingside castling");
        expect(session.currentFen() == castledPosition,
               "redo restores the castled position");
    }

    // tests white queenside castling and its undo and redo operations
    void testQueensideCastlingUndoRedo() {
        GameSession session;

        const std::string startingPosition =
                "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";

        expect(session.loadFen(startingPosition), "load the queenside castling position");

        expect(session.makeMove(e1, c1), "white castles queenside");

        const std::string castledPosition =
                "r3k2r/8/8/8/8/8/8/2KR3R b kq - 1 1";

        expect(session.currentFen() == castledPosition,
               "queenside castling moves the king and rook and clears white rights");

        expect(session.undo(), "undo queenside castling");
        expect(session.currentFen() == startingPosition,
               "undo restores the queenside castling position");

        expect(session.redo(), "redo queenside castling");
        expect(session.currentFen() == castledPosition,
               "redo restores the queenside castling result");
    }

    // tests black kingside castling
    void testBlackKingsideCastling() {
        GameSession session;

        const std::string startingPosition =
                "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1";

        expect(session.loadFen(startingPosition), "load the black castling position");

        expect(session.makeMove(e8, g8), "black castles kingside");

        const std::string castledPosition =
                "r4rk1/8/8/8/8/8/8/R3K2R w KQ - 1 2";

        expect(session.currentFen() == castledPosition,
               "black kingside castling moves the king and rook and clears black rights");
    }

    // tests that castling is rejected when a transit square is attacked
    void testCastlingThroughAttackedSquare() {
        GameSession session;

        const std::string startingPosition =
                "4kr2/8/8/8/8/8/8/R3K2R w KQ - 0 1";

        expect(session.loadFen(startingPosition), "load the attacked transit square position");

        expect(!session.makeMove(e1, g1),
               "kingside castling is rejected when f1 is attacked");

        expect(session.currentFen() == startingPosition,
               "rejected castling leaves the board unchanged");
    }

    // tests that moving a rook permanently removes its castling rights
    void testCastlingRightsAfterRookMoves() {
        GameSession session;

        const std::string startingPosition =
                "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";

        expect(session.loadFen(startingPosition),
               "load the castling rights position");

        expect(session.makeMove(a1, a2),
               "white moves the queenside rook");

        expect(session.makeMove(e8, e7),
               "black moves the king");

        expect(session.makeMove(a2, a1),
               "white returns the queenside rook");

        expect(session.makeMove(e7, e8),
               "black returns the king");

        const std::string positionBeforeCastling =
                session.currentFen();

        expect(!session.makeMove(e1, c1),
               "white cannot castle queenside after moving the rook");

        expect(session.currentFen() == positionBeforeCastling,
               "rejected castling leaves the position unchanged");
    }

    // tests that castling is rejected when castling rights are unavailable
    void testCastlingWithoutRights() {
        GameSession session;

        const std::string position =
                "4k3/8/8/8/8/8/8/R3K2R w - - 0 1";

        expect(session.loadFen(position), "load position without castling rights");

        expect(!session.makeMove(e1, g1),
               "kingside castling is rejected without castling rights");

        expect(!session.makeMove(e1, c1),
               "queenside castling is rejected without castling rights");
    }

    // tests that castling is rejected when a piece blocks the king's path
    void testCastlingPathBlocked() {
        GameSession session;

        const std::string position =
                "4k3/8/8/8/8/8/8/4KB1R w K - 0 1";

        expect(session.loadFen(position), "load blocked castling position");

        expect(!session.makeMove(e1, g1),
               "castling is rejected when f1 is occupied");
    }

    // tests that castling is rejected when the transit square is attacked
    void testCastlingThroughAttackedSquare1() {
        GameSession session;

        const std::string position =
                "k4r2/8/8/8/8/8/8/4K2R w K - 0 1";

        expect(session.loadFen(position), "load attacked transit square position");

        expect(!session.makeMove(e1, g1),
               "castling is rejected when f1 is attacked");
    }

    // tests that castling is rejected when the destination square is attacked
    void testCastlingToAttackedSquare() {
        GameSession session;

        const std::string position =
                "k5r1/8/8/8/8/8/8/4K2R w K - 0 1";

        expect(session.loadFen(position), "load attacked destination position");

        expect(!session.makeMove(e1, g1),
               "castling is rejected when g1 is attacked");
    }

    // tests that queenside castling does not require the b-file square to be safe
    void testQueensideCastlingWithAttackedBSquare() {
        GameSession session;

        const std::string position =
                "1r5k/8/8/8/8/8/8/R3K3 w Q - 0 1";

        expect(session.loadFen(position), "load queenside b-square position");

        expect(session.makeMove(e1, c1),
               "queenside castling is allowed when only b1 is attacked");
    }

    // tests that capturing a rook removes its castling rights
    void testCastlingRightsAfterRookCapture() {
        GameSession session;

        const std::string position =
                "r3k3/8/8/8/8/8/8/R3K3 b Qq - 0 1";

        expect(session.loadFen(position), "load rook capture position");

        expect(session.makeMove(a8, a1),
               "black rook captures the white queenside rook");

        const std::string expectedPosition =
                "4k3/8/8/8/8/8/8/r3K3 w - - 0 2";

        expect(session.currentFen() == expectedPosition,
               "capturing and moving the rooks clears both queenside rights");
    }

    // tests that moving the king removes both castling rights
    void testKingMoveRemovesBothCastlingRights() {
        GameSession session;

        expect(session.loadFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"),
               "load king castling rights position");

        expect(session.makeMove(e1, d1),
               "white king moves");

        expect(session.currentFen() ==
               "r3k2r/8/8/8/8/8/8/R2K3R b kq - 1 1",
               "king move removes both white castling rights");
    }

    // tests that returning a rook to its original square does not restore castling rights
    void testCastlingRightsNotRestoredAfterRookReturns() {
        GameSession session;

        expect(session.loadFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"),
               "load rook return position");

        expect(session.makeMove(a1, a2),
               "white rook moves away");

        expect(session.makeMove(e8, e7),
               "black king moves");

        expect(session.makeMove(a2, a1),
               "white rook returns");

        expect(session.currentFen() ==
               "r6r/4k3/8/8/8/8/8/R3K2R b K - 3 2",
               "white queenside castling right remains removed");
    }
}
