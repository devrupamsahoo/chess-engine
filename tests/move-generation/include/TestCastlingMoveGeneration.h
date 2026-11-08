//
// Created by devrupamsahoo on 08-11-2026.
//

#ifndef CHESS_ENGINE_TESTCASTLINGMOVEGENERATION_H
#define CHESS_ENGINE_TESTCASTLINGMOVEGENERATION_H


namespace MoveGenerationTests {
    bool testCastlingThroughAttackIsFiltered();

    bool testValidCastlingIsKept();

    bool testWhiteQueensideCastlingIsKept();

    bool testBlackKingsideCastlingIsKept();

    bool testBlackCastlingThroughAttackIsFiltered();
}


#endif //CHESS_ENGINE_TESTCASTLINGMOVEGENERATION_H
