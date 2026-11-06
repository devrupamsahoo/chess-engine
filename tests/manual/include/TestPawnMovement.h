//
// Created by devrupamsahoo on 05-11-2026.
//

#ifndef CHESS_ENGINE_TESTPAWNMOVEMENT_H
#define CHESS_ENGINE_TESTPAWNMOVEMENT_H


namespace ManualTests {
    // tests pawn movement rules, captures, blocked paths, double steps, and en passant target updates
    void testPawnForwardBlocked();

    void testPawnCaptures();

    void testPawnCannotMoveBackward();

    void testPawnDoubleStepFromStartingPosition();

    void testPawnCannotDoubleStepFromNonStartingPosition();

    void testPawnDoubleStepBlockedAtIntermediateSquare();

    void testPawnCannotCaptureStraightForward();

    void testBlackPawnDoubleStepUpdatesEnPassantTarget();
}


#endif //CHESS_ENGINE_TESTPAWNMOVEMENT_H
