//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef TEST_MOBILITY_H
#define TEST_MOBILITY_H

namespace EvaluationTests {
    // runs all piece mobility evaluation tests
    void testKnightMobilityOnOpenBoard();

    void testBishopMobilityOnOpenBoard();

    void testRookMobilityOnOpenBoard();

    void testQueenMobilityOnOpenBoard();

    void testOwnPieceBlocking();

    void testEnemyCaptureSquareIncluded();

    void testSquaresBeyondEnemyPieceExcluded();

    void testCornerAndEdgePositions();

    void testWhiteBlackMobilitySymmetry();

    void testRemovingBlockerIncreasesMobility();

    void testAddingBlockerDecreasesMobility();

    void testFriendlySquaresAreNotCounted();

    void testMobilityWeightsAreRespected();

    void testAggregateMobilityIsDeterministic();

    void runMobilityTests();
} // namespace EvaluationTests

#endif // TEST_MOBILITY_H
