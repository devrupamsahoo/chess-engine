//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef TEST_GAME_PHASE_H
#define TEST_GAME_PHASE_H

namespace EvaluationTests {
    // runs all game phase calculation and interpolation boundary tests
    void testStartingPositionPhaseTwentyFour();

    void testRemovingOneKnightLowersPhaseByOne();

    void testRemovingOneBishopLowersPhaseByOne();

    void testRemovingOneRookLowersPhaseByTwo();

    void testRemovingOneQueenLowersPhaseByFour();

    void testPawnsDoNotAffectPhase();

    void testKingsOnlyPhaseZero();

    void testPhaseClampingBehavior();

    void testFullMaterialPositionUsesMiddleGamePhase();

    void testEndgamePositionUsesEndGamePhase();

    void runGamePhaseTests();
} // namespace EvaluationTests

#endif // TEST_GAME_PHASE_H
