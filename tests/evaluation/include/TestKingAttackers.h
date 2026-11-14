//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef TEST_KING_ATTACKERS_H
#define TEST_KING_ATTACKERS_H

namespace EvaluationTests {
    // runs all king attacker counting and king zone danger evaluation tests
    void testOneAttackingPawn();

    void testOneAttackingKnight();

    void testOneAttackingBishop();

    void testOneAttackingRook();

    void testOneAttackingQueen();

    void testDifferentAttackerTypes();

    void testMultipleAttackersOfSameType();

    void testSameAttackerAttackingMultipleKingZoneSquaresCountsOnce();

    void testBlockedBishop();

    void testBlockedRook();

    void testBlockedQueen();

    void testAttackerOutsideKingZone();

    void testStrongerAttackerIncreasesPressure();

    void testAttackerCountBonusCap();

    void testWhiteBlackKingAttackersSymmetry();

    void testInvalidKingSquareNegativeOne();

    void testInvalidKingSquareGreaterOrEqualSixtyFour();

    void testCountAttackersDoesNotCountWrongColorPieces();

    void testCountAttackersDoesNotSeeThroughBlockers();

    void testOccupiedTargetCaptureSquareCountsAsAttacked();

    void runKingAttackersTests();
} // namespace EvaluationTests

#endif // TEST_KING_ATTACKERS_H
