//
// Created by devrupamsahoo on 05-11-2026.
//

#ifndef CHESS_ENGINE_TESTKINGSAFETY_H
#define CHESS_ENGINE_TESTKINGSAFETY_H


namespace ManualTests {
    // tests king safety, check responses, and restrictions on pinned pieces
    void testKingCannotMoveIntoCheck();

    void testCannotIgnoreCheck();

    void testPinnedPieceCannotMove();

    void testKingsCannotBecomeAdjacent();

    void testKingCannotCaptureProtectedPiece();

    void testBlockCheck();

    void testDoubleCheckOnlyAllowsKingMove();

    void testPinnedPieceCanMoveAlongPin();

    void testKingCannotMoveMoreThanOneSquare();
}


#endif //CHESS_ENGINE_TESTKINGSAFETY_H
