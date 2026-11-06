//
// Created by devrupamsahoo on 05-11-2026.
//

#ifndef CHESS_ENGINE_TESTPIECEMOVEMENT_H
#define CHESS_ENGINE_TESTPIECEMOVEMENT_H


namespace ManualTests {
    // tests piece movement patterns, friendly-piece restrictions, captures, and blocked sliding paths
    void testKnightJumpsOverPieces();

    void testCannotCaptureFriendlyPiece();

    void testBishopBlockedByFriendlyPiece();

    void testRookBlockedByFriendlyPiece();

    void testQueenDiagonalMovement();

    void testRookCanCaptureEnemyPiece();

    void testRookCannotJumpOverEnemyPiece();

    void testBishopCanCaptureEnemyPiece();

    void testQueenCannotJumpOverEnemyPiece();
}


#endif //CHESS_ENGINE_TESTPIECEMOVEMENT_H
