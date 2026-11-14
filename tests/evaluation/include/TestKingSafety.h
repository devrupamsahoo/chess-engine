//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef TEST_KING_SAFETY_H
#define TEST_KING_SAFETY_H

namespace EvaluationTests {
    // runs all king safety evaluation tests across shields, open files, pressure, and game phases
    void testWhitePawnShield();

    void testBlackPawnShield();

    void testMissingWhitePawnShield();

    void testMissingBlackPawnShield();

    void testWhiteKingNearOpenFile();

    void testBlackKingNearOpenFile();

    void testWhiteKingNearSemiOpenFile();

    void testBlackKingNearSemiOpenFile();

    void testKingZonePressure();

    void testMirroredKingSafetySymmetry();

    void testKingSafetyPhaseScaling();

    void testKingSafetyZeroAtPhaseZero();

    void testKingSafetyActiveAtPhaseTwentyFour();

    void testFriendlyPiecesAreNotEnemyAttackers();

    void testBlockedSlidingAttackersDoNotCreatePressure();

    void runKingSafetyTests();
} // namespace EvaluationTests

#endif // TEST_KING_SAFETY_H
