//
// Created by devrupamsahoo on 05-11-2026.
//

#ifndef CHESS_ENGINE_TESTFENANDMOVECOUNTERS_H
#define CHESS_ENGINE_TESTFENANDMOVECOUNTERS_H


namespace ManualTests {
    // tests FEN serialization and move-counter updates across different move types
    void testFenRoundTrip();

    void testMoveCountersAfterQuietMoves();

    void testPawnMoveResetsHalfmoveClock();

    void testCaptureResetsHalfmoveClock();
}


#endif //CHESS_ENGINE_TESTFENANDMOVECOUNTERS_H
