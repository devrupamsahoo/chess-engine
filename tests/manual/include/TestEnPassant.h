//
// Created by devrupamsahoo on 05-11-2026.
//

#ifndef CHESS_ENGINE_TESTENPASSANT_H
#define CHESS_ENGINE_TESTENPASSANT_H


namespace ManualTests {
    // tests en passant rules, expiration, king safety, and undo/redo behavior
    void testEnPassantUndoRedo();

    void testBlackEnPassantUndoRedo();

    void testEnPassantExpiration();

    void testEnPassantCannotExposeKing();

    void testEnPassantTargetClearedAfterKnightMove();
}


#endif //CHESS_ENGINE_TESTENPASSANT_H
