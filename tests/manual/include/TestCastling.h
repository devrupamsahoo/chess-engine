//
// Created by devrupamsahoo on 05-11-2026.
//

#ifndef CHESS_ENGINE_TESTCASTLING_H
#define CHESS_ENGINE_TESTCASTLING_H


namespace ManualTests {
    // tests castling rules, restrictions, and castling-right updates across moves and undo/redo
    void testCastlingUndoRedo();

    void testQueensideCastlingUndoRedo();

    void testBlackKingsideCastling();

    void testCastlingThroughAttackedSquare();

    void testCastlingRightsAfterRookMoves();

    void testCastlingWithoutRights();

    void testCastlingPathBlocked();

    void testCastlingThroughAttackedSquare1();

    void testCastlingToAttackedSquare();

    void testQueensideCastlingWithAttackedBSquare();

    void testCastlingRightsAfterRookCapture();

    void testKingMoveRemovesBothCastlingRights();

    void testCastlingRightsNotRestoredAfterRookReturns();
}


#endif //CHESS_ENGINE_TESTCASTLING_H
