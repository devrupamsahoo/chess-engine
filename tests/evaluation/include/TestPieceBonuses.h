//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef TEST_PIECE_BONUSES_H
#define TEST_PIECE_BONUSES_H

namespace EvaluationTests {
    // runs all piece bonus evaluation tests for bishops and rooks
    void testNoBishopPairBonusWithOneBishop();

    void testWhiteBishopPairPositive();

    void testBlackBishopPairNegative();

    void testWhiteRookOpenFileBonus();

    void testBlackRookOpenFileBonus();

    void testWhiteRookSemiOpenFileBonus();

    void testBlackRookSemiOpenFileBonus();

    void testWhiteRookSeventhRankBonus();

    void testBlackRookSecondRankBonus();

    void testBlockedRookFileHasNoOpenFileBonus();

    void testNonRookInputReceivesNoRookFileBonus();

    void testRookOnAnotherRankReceivesNoSeventhRankBonus();

    void runPieceBonusesTests();

} // namespace EvaluationTests

#endif // TEST_PIECE_BONUSES_H
