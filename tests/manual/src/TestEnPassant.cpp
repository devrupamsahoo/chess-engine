//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/TestEnPassant.h"
#include "../include/ManualTestUtils.h"

#include "chess/core/GameSession.h"

#include <string>

namespace ManualTests {
    // tests en passant captures, expiration, king safety, and undo/redo behavior


    // tests white en passant capture and its undo and redo operations
    void testEnPassantUndoRedo() {
        GameSession session;

        expect(session.makeMove(e2, e4), "white opens with e2-e4");
        expect(session.makeMove(a7, a6), "black plays a7-a6");
        expect(session.makeMove(e4, e5), "white advances e4-e5");
        expect(session.makeMove(d7, d5), "black plays d7-d5");

        const std::string positionBeforeCapture =
                "rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3";

        expect(session.currentFen() == positionBeforeCapture,
               "double pawn move records the en passant target");

        expect(session.makeMove(e5, d6), "white captures en passant");

        const std::string positionAfterCapture =
                "rnbqkbnr/1pp1pppp/p2P4/8/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 3";

        expect(session.currentFen() == positionAfterCapture,
               "en passant removes the captured pawn and expires the target");
        expect(session.capturedPieces().size() == 1
               && session.capturedPieces()[0] == blackPawn,
               "en passant records the captured black pawn");

        expect(session.undo(), "undo en passant");
        expect(session.currentFen() == positionBeforeCapture,
               "undo restores the en passant position");
        expect(session.capturedPieces().empty(),
               "undo removes the capture from the captured-piece list");

        expect(session.redo(), "redo en passant");
        expect(session.currentFen() == positionAfterCapture,
               "redo restores the en passant result");
        expect(session.capturedPieces().size() == 1
               && session.capturedPieces()[0] == blackPawn,
               "redo restores the captured-piece record");
    }

    // tests black en passant capture and its undo and redo operations
    void testBlackEnPassantUndoRedo() {
        GameSession session;

        const std::string startingPosition =
                "rnbqkbnr/pppp1ppp/8/8/4p3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2";

        expect(session.loadFen(startingPosition), "load the black en passant position");

        const std::string positionBeforeCapture = session.currentFen();

        expect(session.makeMove(d2, d4), "white advances d2-d4");

        const std::string positionAfterDoubleMove = session.currentFen();

        expect(session.makeMove(e4, d3), "black captures en passant");

        const std::string positionAfterCapture = session.currentFen();

        expect(session.capturedPieces().size() == 1
               && session.capturedPieces()[0] == whitePawn,
               "en passant records the captured white pawn");

        expect(session.undo(), "undo black en passant");
        expect(session.currentFen() == positionAfterDoubleMove,
               "undo restores the position before black en passant");
        expect(session.capturedPieces().empty(),
               "undo removes the captured white pawn");

        expect(session.redo(), "redo black en passant");
        expect(session.currentFen() == positionAfterCapture,
               "redo restores the black en passant result");
        expect(session.capturedPieces().size() == 1
               && session.capturedPieces()[0] == whitePawn,
               "redo restores the captured white pawn record");
    }

    // tests that the en passant opportunity expires after one move
    void testEnPassantExpiration() {
        GameSession session;

        expect(session.makeMove(e2, e4), "white opens with e2-e4");
        expect(session.makeMove(a7, a6), "black plays a7-a6");
        expect(session.makeMove(e4, e5), "white advances e4-e5");
        expect(session.makeMove(d7, d5), "black plays d7-d5");

        expect(session.makeMove(g1, f3), "white plays g1-f3");

        expect(session.makeMove(a6, a5), "black plays a6-a5");

        const std::string positionBeforeAttempt = session.currentFen();

        expect(!session.makeMove(e5, d6),
               "en passant is rejected after the opportunity expires");

        expect(session.currentFen() == positionBeforeAttempt,
               "rejected en passant leaves the board unchanged");

        expect(session.capturedPieces().empty(),
               "expired en passant does not record a capture");
    }

    // tests that en passant is rejected when it exposes the king to a rook attack
    void testEnPassantCannotExposeKing() {
        GameSession session;

        const std::string position =
                "4r1k1/8/8/3pP3/8/8/8/4K3 w - d6 0 1";

        expect(session.loadFen(position), "load en passant discovered check position");

        expect(!session.makeMove(e5, d6),
               "en passant is rejected when it exposes the king to check");

        expect(session.currentFen() == position,
               "illegal en passant leaves the position unchanged");

        expect(session.capturedPieces().empty(),
               "illegal en passant does not record a capture");
    }

    // tests that en passant target is cleared when the opponent makes a different move
    void testEnPassantTargetClearedAfterKnightMove() {
        GameSession session;

        expect(session.makeMove(e2, e4),
               "white pawn double step");

        expect(session.makeMove(b8, c6),
               "black knight moves");

        expect(session.currentFen() ==
               "r1bqkbnr/pppppppp/2n5/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 1 2",
               "en passant target cleared after knight move");
    }
}
