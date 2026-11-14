//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef TEST_PAWN_STRUCTURE_H
#define TEST_PAWN_STRUCTURE_H

namespace EvaluationTests {
    // runs all pawn structure evaluation tests for doubled, isolated, and passed pawns
    void testDoubledWhitePawns();

    void testDoubledBlackPawns();

    void testTriplePawnsReceiveTwoPenalties();

    void testIsolatedWhitePawn();

    void testIsolatedBlackPawn();

    void testPawnSupportedByFriendlyPawnOnLeftAdjacentFile();

    void testPawnSupportedByFriendlyPawnOnRightAdjacentFile();

    void testPassedWhitePawn();

    void testPassedBlackPawn();

    void testBlockedWhitePawnIsNotPassed();

    void testBlockedBlackPawnIsNotPassed();

    void testAdvancedPassedPawnReceivesLargerBonus();

    void testEdgeFilePassedPawn();

    void testEnemyPawnOnAdjacentFilePreventsPassedPawnStatus();

    void testEnemyPawnBehindPawnDoesNotPreventPassedPawnStatus();

    void runPawnStructureTests();
} // namespace EvaluationTests

#endif // TEST_PAWN_STRUCTURE_H
