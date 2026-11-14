//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef CHESS_ENGINE_TESTMATERIALANDPIECESQUARETABLES_H
#define CHESS_ENGINE_TESTMATERIALANDPIECESQUARETABLES_H


namespace EvaluationTests {
    // runs all material and piece-square table evaluation tests
    void testEqualMaterialProducesEqualMaterialScore();

    void testPawnMaterialAndPawnPST();

    void testKnightMaterialAndKnightPST();

    void testBishopMaterialAndBishopPST();

    void testRookMaterialAndRookPST();

    void testQueenMaterialAndQueenPST();

    void testKingMiddleGamePST();

    void testKingEndGamePST();

    void testKingInterpolationAtPhaseZero();

    void testKingInterpolationAtPhaseTwentyFour();

    void testWhiteBlackPSTMirroring();

    void testMaterialAccumulation();

    void testCapturingPieceChangesMaterialByExpectedValue();

    void runMaterialAndPieceSquareTablesTests();
} // namespace EvaluationTests


#endif //CHESS_ENGINE_TESTMATERIALANDPIECESQUARETABLES_H
