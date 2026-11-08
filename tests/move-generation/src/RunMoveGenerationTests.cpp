//
// Created by devrupamsahoo on 08-11-2026.
//

#include "../include/RunMoveGenerationTests.h"

#include "../include/TestCastlingMoveGeneration.h"
#include "../include/TestEnPassantMoveGeneration.h"
#include "../include/TestLegalMoveFiltering.h"
#include "../include/TestPerft.h"
#include "../include/TestPromotionMoveGeneration.h"
#include "../include/TestPseudoLegalMoveGeneration.h"

#include "ManualTestUtils.h"

namespace MoveGenerationTests {
    void runMoveGenerationTests() {
        ManualTests::expect(
            testStartingPositionMoveCount(),
            "starting position generates 20 pseudo-legal moves");

        ManualTests::expect(
            testRookStopsAtFriendlyPiece(),
            "rook stops before a friendly piece");

        ManualTests::expect(
            testMoveThatExposesKingIsFiltered(),
            "moves exposing the king are filtered");

        ManualTests::expect(
            testKingInCheckOnlyAllowsSafeMoves(),
            "checked king receives only safe legal moves");

        ManualTests::expect(
            testEnPassantExposingKingIsFiltered(),
            "en passant exposing the king is filtered");

        ManualTests::expect(
            testCastlingThroughAttackIsFiltered(),
            "castling through an attacked square is filtered");

        ManualTests::expect(
            testValidCastlingIsKept(),
            "valid castling is kept");

        ManualTests::expect(
            testAllPromotionChoicesAreLegal(),
            "all promotion choices are legal when the king remains safe");

        ManualTests::expect(
            testPromotionExposingKingIsFiltered(),
            "promotion exposing the king is filtered");

        ManualTests::expect(
            testWhiteQueensideCastlingIsKept(),
            "white queenside castling is kept");

        ManualTests::expect(
            testBlackKingsideCastlingIsKept(),
            "black kingside castling is kept");

        ManualTests::expect(
            testBlackCastlingThroughAttackIsFiltered(),
            "black castling through an attacked square is filtered");

        runPerftTests();

        // Enable this only when intentionally running the slower depth-four tests.
        runExtendedPerftTests();
    }
}
