//
// Created by devrupamsahoo on 07-11-2026.
//

#include "RunManualTests.h"

#include "ManualTestUtils.h"
#include "TestMoveValidation.h"
#include "TestPawnMovement.h"
#include "TestPieceMovement.h"
#include "TestKingSafety.h"
#include "TestCastling.h"
#include "TestEnPassant.h"
#include "TestPawnPromotion.h"
#include "TestMoveHistory.h"
#include "TestFenAndMoveCounters.h"
#include "TestGameStatus.h"

// runs all manual tests, grouped by the chess rules and features they cover
void runManualTests() {
    using namespace ManualTests;

    // move validation
    testInvalidMoves();
    testCannotMoveOpponentPiece();

    // pawn movement
    testPawnForwardBlocked();
    testPawnCaptures();
    testPawnCannotMoveBackward();
    testPawnDoubleStepFromStartingPosition();
    testPawnCannotDoubleStepFromNonStartingPosition();
    testPawnDoubleStepBlockedAtIntermediateSquare();
    testPawnCannotCaptureStraightForward();
    testBlackPawnDoubleStepUpdatesEnPassantTarget();

    // piece movement
    testKnightJumpsOverPieces();
    testCannotCaptureFriendlyPiece();
    testBishopBlockedByFriendlyPiece();
    testRookBlockedByFriendlyPiece();
    testQueenDiagonalMovement();
    testRookCanCaptureEnemyPiece();
    testRookCannotJumpOverEnemyPiece();
    testBishopCanCaptureEnemyPiece();
    testQueenCannotJumpOverEnemyPiece();

    // king safety
    testKingCannotMoveIntoCheck();
    testCannotIgnoreCheck();
    testPinnedPieceCannotMove();
    testKingsCannotBecomeAdjacent();
    testKingCannotCaptureProtectedPiece();
    testBlockCheck();
    testDoubleCheckOnlyAllowsKingMove();
    testPinnedPieceCanMoveAlongPin();
    testKingCannotMoveMoreThanOneSquare();

    // castling
    testCastlingUndoRedo();
    testQueensideCastlingUndoRedo();
    testBlackKingsideCastling();
    testCastlingThroughAttackedSquare();
    testCastlingRightsAfterRookMoves();
    testCastlingWithoutRights();
    testCastlingPathBlocked();
    testCastlingThroughAttackedSquare1();
    testCastlingToAttackedSquare();
    testQueensideCastlingWithAttackedBSquare();
    testCastlingRightsAfterRookCapture();
    testKingMoveRemovesBothCastlingRights();
    testCastlingRightsNotRestoredAfterRookReturns();

    // en passant
    testEnPassantUndoRedo();
    testBlackEnPassantUndoRedo();
    testEnPassantExpiration();
    testEnPassantCannotExposeKing();
    testEnPassantTargetClearedAfterKnightMove();

    // pawn promotion
    testWhitePawnPromotion();
    testBlackPawnPromotion();
    testPawnPromotionByCapture();
    testPromotionUndoRedo();

    // move history
    testMultipleUndoRedo();
    testCaptureUndoRedo();

    // FEN and move counters
    testFenRoundTrip();
    testMoveCountersAfterQuietMoves();
    testPawnMoveResetsHalfmoveClock();
    testCaptureResetsHalfmoveClock();

    // game status
    testGameStatus();
    testThreefoldRepetition();
}