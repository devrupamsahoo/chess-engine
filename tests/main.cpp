#include "manual/include/ManualTestUtils.h"
#include "manual/include/TestMoveValidation.h"
#include "manual/include/TestPawnMovement.h"
#include "manual/include/TestPieceMovement.h"
#include "manual/include/TestKingSafety.h"
#include "manual/include/TestCastling.h"
#include "manual/include/TestEnPassant.h"
#include "manual/include/TestPawnPromotion.h"
#include "manual/include/TestMoveHistory.h"
#include "manual/include/TestFenAndMoveCounters.h"
#include "TestPerft.h"

#include "chess/core/GameSession.h"

#include <iostream>

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
}

// runs the manual and move-generation test suites, then reports the overall result
int main() {
    runManualTests();
    runMoveGenerationTests();

    using namespace ManualTests;

    if (failures == 0) {
        std::cout << "All tests passed!\n";
    } else {
        std::cout << failures << " test(s) failed.\n";
    }

    return failures == 0 ? 0 : 1;
}
