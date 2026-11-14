//
// Created by devrupamsahoo on 10-11-2026.
//

#include "TestPieceBonuses.h"

#include <cstdlib>
#include <string>

#include "ManualTestUtils.h"

#include "chess/core/ChessBoard.h"
#include "chess/core/Enums.h"
#include "chess/evaluation/EvaluationConstants.h"
#include "chess/evaluation/PieceBonuses.h"
#include "chess/notation/FEN.h"

namespace EvaluationTests {
    namespace {
        // loads a fen position into the board
        bool loadPosition(
            ChessBoard &board,
            const std::string &fen
        ) {
            const bool loaded = FEN::load(board, fen);
            return loaded;
        }
    } // namespace

    void testNoBishopPairBonusWithOneBishop() {
        // single white bishop and single black bishop receive zero bishop-pair bonus
        ChessBoard oneWhiteBishop;
        ChessBoard oneEachBishop;
        if (!loadPosition(oneWhiteBishop, "4k3/8/8/8/8/8/8/2B1K3 w - - 0 1") ||
            !loadPosition(oneEachBishop, "2b1k3/8/8/8/8/8/8/2B1K3 w - - 0 1")) {
            ManualTests::expect(false, "no bishop-pair bonus fen loading");
            return;
        }

        ManualTests::expect(
            PieceBonuses::evaluateBishopPair(oneWhiteBishop) == 0,
            "single white bishop receives no bishop-pair bonus"
        );
        ManualTests::expect(
            PieceBonuses::evaluateBishopPair(oneEachBishop) == 0,
            "one bishop per side receives no bishop-pair bonus"
        );
    }

    void testWhiteBishopPairPositive() {
        // two white bishops against zero black bishops give +bishop_pair_bonus
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/8/8/8/2B1KB2 w - - 0 1")) {
            ManualTests::expect(false, "white bishop pair bonus fen loading");
            return;
        }

        const int score = PieceBonuses::evaluateBishopPair(board);
        ManualTests::expect(
            score == EvalConstants::BISHOP_PAIR_BONUS && score > 0,
            "white bishop pair produces positive BISHOP_PAIR_BONUS"
        );
    }

    void testBlackBishopPairNegative() {
        // two black bishops against zero white bishops give -bishop_pair_bonus
        ChessBoard board;
        if (!loadPosition(board, "2b1kb2/8/8/8/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "black bishop pair bonus fen loading");
            return;
        }

        const int score = PieceBonuses::evaluateBishopPair(board);
        ManualTests::expect(
            score == -EvalConstants::BISHOP_PAIR_BONUS && score < 0,
            "black bishop pair produces negative BISHOP_PAIR_BONUS"
        );
    }

    void testWhiteRookOpenFileBonus() {
        // white rook on d1 (square 3) with no pawns on the d-file receives rook_open_file_bonus
        ChessBoard board;
        if (!loadPosition(board, "4k3/p7/8/8/8/8/P7/3RK3 w - - 0 1")) {
            ManualTests::expect(false, "white rook open-file bonus fen loading");
            return;
        }

        const int bonus = PieceBonuses::evaluateRookFile(board, 3, whiteRook);
        ManualTests::expect(
            bonus == EvalConstants::ROOK_OPEN_FILE_BONUS,
            "white rook on open d-file receives ROOK_OPEN_FILE_BONUS"
        );
    }

    void testBlackRookOpenFileBonus() {
        // black rook on d8 (square 59) with no pawns on the d-file receives open-file bonus magnitude
        ChessBoard board;
        if (!loadPosition(board, "3rk3/p7/8/8/8/8/P7/4K3 w - - 0 1")) {
            ManualTests::expect(false, "black rook open-file bonus fen loading");
            return;
        }

        const int bonus = PieceBonuses::evaluateRookFile(board, 59, blackRook);
        ManualTests::expect(
            std::abs(bonus) == EvalConstants::ROOK_OPEN_FILE_BONUS,
            "black rook on open d-file receives ROOK_OPEN_FILE_BONUS"
        );
    }

    void testWhiteRookSemiOpenFileBonus() {
        // white rook on d1 (square 3) with only enemy black pawn on d6 receives rook_semi_open_file_bonus
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/3p4/8/8/8/8/3RK3 w - - 0 1")) {
            ManualTests::expect(false, "white rook semi-open-file bonus fen loading");
            return;
        }

        const int bonus = PieceBonuses::evaluateRookFile(board, 3, whiteRook);
        ManualTests::expect(
            bonus == EvalConstants::ROOK_SEMI_OPEN_FILE_BONUS,
            "white rook on semi-open d-file receives ROOK_SEMI_OPEN_FILE_BONUS"
        );
    }

    void testBlackRookSemiOpenFileBonus() {
        // black rook on d8 (square 59) with only enemy white pawn on d3 receives semi-open-file bonus
        ChessBoard board;
        if (!loadPosition(board, "3rk3/8/8/8/8/3P4/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "black rook semi-open-file bonus fen loading");
            return;
        }

        const int bonus = PieceBonuses::evaluateRookFile(board, 59, blackRook);
        ManualTests::expect(
            std::abs(bonus) == EvalConstants::ROOK_SEMI_OPEN_FILE_BONUS,
            "black rook on semi-open d-file receives ROOK_SEMI_OPEN_FILE_BONUS"
        );
    }

    void testWhiteRookSeventhRankBonus() {
        // white rook on d7 (square 51, rank 7) receives rook_seventh_rank_bonus
        const int bonus = PieceBonuses::evaluateRookSeventhRank(51, whiteRook);
        ManualTests::expect(
            bonus == EvalConstants::ROOK_SEVENTH_RANK_BONUS,
            "white rook on seventh rank receives ROOK_SEVENTH_RANK_BONUS"
        );
    }

    void testBlackRookSecondRankBonus() {
        // black rook on d2 (square 11, rank 2) receives seventh-rank bonus magnitude
        const int bonus = PieceBonuses::evaluateRookSeventhRank(11, blackRook);
        ManualTests::expect(
            std::abs(bonus) == EvalConstants::ROOK_SEVENTH_RANK_BONUS,
            "black rook on second rank receives ROOK_SEVENTH_RANK_BONUS"
        );
    }

    void testBlockedRookFileHasNoOpenFileBonus() {
        // white rook on d1 (square 3) blocked by friendly pawn on d2 (and black pawn on d7) receives 0
        ChessBoard closedFileBoard;
        ChessBoard friendlyPawnBoard;
        if (!loadPosition(closedFileBoard, "3rk3/3p4/8/8/8/8/3P4/3RK3 w - - 0 1") ||
            !loadPosition(friendlyPawnBoard, "4k3/8/8/8/8/8/3P4/3RK3 w - - 0 1")) {
            ManualTests::expect(false, "blocked rook file fen loading");
            return;
        }

        ManualTests::expect(
            PieceBonuses::evaluateRookFile(closedFileBoard, 3, whiteRook) == 0,
            "white rook on closed file with both pawns receives no file bonus"
        );
        ManualTests::expect(
            PieceBonuses::evaluateRookFile(closedFileBoard, 59, blackRook) == 0,
            "black rook on closed file with both pawns receives no file bonus"
        );
        ManualTests::expect(
            PieceBonuses::evaluateRookFile(friendlyPawnBoard, 3, whiteRook) == 0,
            "white rook behind friendly pawn receives no open or semi-open file bonus"
        );
    }

    void testNonRookInputReceivesNoRookFileBonus() {
        // passing a non-rook piece enum to evaluaterookfile and evaluaterookseventhrank returns 0
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/8/8/8/3QK3 w - - 0 1")) {
            ManualTests::expect(false, "non-rook input fen loading");
            return;
        }

        ManualTests::expect(
            PieceBonuses::evaluateRookFile(board, 3, whiteQueen) == 0 &&
            PieceBonuses::evaluateRookFile(board, 3, whiteKnight) == 0 &&
            PieceBonuses::evaluateRookFile(board, 3, noPiece) == 0,
            "non-rook piece receives no rook-file bonus"
        );

        ManualTests::expect(
            PieceBonuses::evaluateRookSeventhRank(51, whiteQueen) == 0 &&
            PieceBonuses::evaluateRookSeventhRank(11, blackQueen) == 0,
            "non-rook piece receives no rook seventh-rank bonus"
        );
    }

    void testRookOnAnotherRankReceivesNoSeventhRankBonus() {
        // white rook on ranks 1..6 or 8 and black rook on ranks 1 or 3..8 receive 0 seventh-rank bonus
        ManualTests::expect(
            PieceBonuses::evaluateRookSeventhRank(3, whiteRook) == 0 &&
            PieceBonuses::evaluateRookSeventhRank(27, whiteRook) == 0 &&
            PieceBonuses::evaluateRookSeventhRank(59, whiteRook) == 0,
            "white rook not on seventh rank receives no seventh-rank bonus"
        );

        ManualTests::expect(
            PieceBonuses::evaluateRookSeventhRank(59, blackRook) == 0 &&
            PieceBonuses::evaluateRookSeventhRank(27, blackRook) == 0 &&
            PieceBonuses::evaluateRookSeventhRank(3, blackRook) == 0,
            "black rook not on second rank receives no seventh-rank bonus"
        );
    }

    void runPieceBonusesTests() {
        testNoBishopPairBonusWithOneBishop();
        testWhiteBishopPairPositive();
        testBlackBishopPairNegative();
        testWhiteRookOpenFileBonus();
        testBlackRookOpenFileBonus();
        testWhiteRookSemiOpenFileBonus();
        testBlackRookSemiOpenFileBonus();
        testWhiteRookSeventhRankBonus();
        testBlackRookSecondRankBonus();
        testBlockedRookFileHasNoOpenFileBonus();
        testNonRookInputReceivesNoRookFileBonus();
        testRookOnAnotherRankReceivesNoSeventhRankBonus();
    }
} // namespace EvaluationTests
