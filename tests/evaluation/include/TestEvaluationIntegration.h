//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef TEST_EVALUATION_INTEGRATION_H
#define TEST_EVALUATION_INTEGRATION_H

namespace EvaluationTests {
    // runs all end-to-end evaluation integration and board state invariance tests
    void testKingsOnlyLegalPositionEvaluatesConsistently();

    void testStartingPositionEvaluatesConsistently();

    void testAddingWhiteQueenIncreasesFullEvaluation();

    void testAddingBlackQueenDecreasesFullEvaluation();

    void testMirroredColorsProduceOppositeWhitePerspectiveScore();

    void testSwitchingSideToMoveNegatesFullEvaluation();

    void testRepeatedEvaluationIsDeterministic();

    void testEvaluationDoesNotMutateFEN();

    void testEvaluationDoesNotChangeSideToMove();

    void testEvaluationDoesNotChangeCastlingRights();

    void testEvaluationDoesNotChangeEnPassantTarget();

    void testEvaluationDoesNotChangeMoveCounters();

    void testEvaluationDoesNotChangeUndoRedoAvailability();

    void testMaterialIsNotIncludedInEvaluatePieceSquareTables();

    void testPSTIsNotIncludedInEvaluateMaterial();

    void testEveryEvaluationFeatureIsIncludedExactlyOnce();

    void runEvaluationIntegrationTests();
} // namespace EvaluationTests

#endif // TEST_EVALUATION_INTEGRATION_H
