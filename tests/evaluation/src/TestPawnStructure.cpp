//
// Created by devrupamsahoo on 10-11-2026.
//

#include "TestPawnStructure.h"

#include <string>

#include "ManualTestUtils.h"

#include "chess/core/ChessBoard.h"
#include "chess/core/Enums.h"
#include "chess/evaluation/EvaluationConstants.h"
#include "chess/evaluation/PawnStructure.h"
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

    void testDoubledWhitePawns() {
        // two white pawns on the d-file (d2 and d3) incur -doubled_pawn_penalty
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/8/3P4/3P4/4K3 w - - 0 1")) {
            ManualTests::expect(false, "doubled white pawns fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluateDoubledPawns(board) == -EvalConstants::DOUBLED_PAWN_PENALTY,
            "doubled white pawns receive -DOUBLED_PAWN_PENALTY"
        );
    }

    void testDoubledBlackPawns() {
        // two black pawns on the d-file (d7 and d6) give +doubled_pawn_penalty to white
        ChessBoard board;
        if (!loadPosition(board, "4k3/3p4/3p4/8/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "doubled black pawns fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluateDoubledPawns(board) == EvalConstants::DOUBLED_PAWN_PENALTY,
            "doubled black pawns produce +DOUBLED_PAWN_PENALTY for white"
        );
    }

    void testTriplePawnsReceiveTwoPenalties() {
        // three white pawns on the d-file (d2, d3, d4) receive two doubled-pawn penalties
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/3P4/3P4/3P4/4K3 w - - 0 1")) {
            ManualTests::expect(false, "triple pawns fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluateDoubledPawns(board) == -2 * EvalConstants::DOUBLED_PAWN_PENALTY,
            "triple white pawns on same file receive two DOUBLED_PAWN_PENALTY penalties"
        );
    }

    void testIsolatedWhitePawn() {
        // single white pawn on d4 with no friendly pawns on c-file or e-file receives -isolated_pawn_penalty
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/3P4/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "isolated white pawn fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluateIsolatedPawns(board) == -EvalConstants::ISOLATED_PAWN_PENALTY,
            "isolated white pawn receives -ISOLATED_PAWN_PENALTY"
        );
    }

    void testIsolatedBlackPawn() {
        // single black pawn on d5 with no friendly pawns on c-file or e-file gives +isolated_pawn_penalty
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/3p4/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "isolated black pawn fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluateIsolatedPawns(board) == EvalConstants::ISOLATED_PAWN_PENALTY,
            "isolated black pawn produces +ISOLATED_PAWN_PENALTY for white"
        );
    }

    void testPawnSupportedByFriendlyPawnOnLeftAdjacentFile() {
        // white pawn on d4 with friendly pawn on c3 is not isolated
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/3P4/2P5/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "pawn supported on left adjacent file fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluateIsolatedPawns(board) == 0,
            "white pawn with friendly pawn on left adjacent file is not isolated"
        );
    }

    void testPawnSupportedByFriendlyPawnOnRightAdjacentFile() {
        // white pawn on d4 with friendly pawn on e3 is not isolated
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/3P4/4P3/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "pawn supported on right adjacent file fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluateIsolatedPawns(board) == 0,
            "white pawn with friendly pawn on right adjacent file is not isolated"
        );
    }

    void testPassedWhitePawn() {
        // white pawn on d5 with no opposing pawns ahead on c, d, or e files receives passed bonus
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/3P4/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "passed white pawn fen loading");
            return;
        }

        const int score = PawnStructure::evaluatePassedPawns(board);
        ManualTests::expect(
            score == EvalConstants::PASSED_PAWN_BONUS[4] && score > 0,
            "passed white pawn on rank 5 receives positive PASSED_PAWN_BONUS"
        );
    }

    void testPassedBlackPawn() {
        // black pawn on d4 with no opposing pawns ahead on c, d, or e files receives negative passed bonus
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/3p4/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "passed black pawn fen loading");
            return;
        }

        const int score = PawnStructure::evaluatePassedPawns(board);
        ManualTests::expect(
            score == -EvalConstants::PASSED_PAWN_BONUS[4] && score < 0,
            "passed black pawn on rank 4 produces negative PASSED_PAWN_BONUS for white"
        );
    }

    void testBlockedWhitePawnIsNotPassed() {
        // white pawn on d4 directly opposed by black pawn on d5 is not passed
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/3p4/3P4/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "blocked white pawn fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluatePassedPawns(board) == 0,
            "white pawn opposed by black pawn on same file is not passed"
        );
    }

    void testBlockedBlackPawnIsNotPassed() {
        // black pawn on e6 directly opposed by white pawn on e5 is not passed
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/4p3/4P3/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "blocked black pawn fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluatePassedPawns(board) == 0,
            "black pawn opposed by white pawn on same file is not passed"
        );
    }

    void testAdvancedPassedPawnReceivesLargerBonus() {
        // white passed pawn on d6 (rank 6) receives strictly larger bonus than on d3 (rank 3)
        ChessBoard boardRank6;
        ChessBoard boardRank3;
        if (!loadPosition(boardRank6, "4k3/8/3P4/8/8/8/8/4K3 w - - 0 1") ||
            !loadPosition(boardRank3, "4k3/8/8/8/8/3P4/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "advanced passed pawn comparison fen loading");
            return;
        }

        const int bonusRank6 = PawnStructure::evaluatePassedPawns(boardRank6);
        const int bonusRank3 = PawnStructure::evaluatePassedPawns(boardRank3);

        ManualTests::expect(
            bonusRank6 > bonusRank3 && bonusRank3 > 0,
            "advanced passed pawn on rank 6 receives larger bonus than passed pawn on rank 3"
        );
    }

    void testEdgeFilePassedPawn() {
        // passed white pawns on a-file and h-file are evaluated without file-mask wraparound errors
        ChessBoard boardAFile;
        ChessBoard boardHFile;
        if (!loadPosition(boardAFile, "4k3/7p/8/P6P/8/8/8/4K3 w - - 0 1") ||
            !loadPosition(boardHFile, "4k3/p7/8/P6P/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "edge-file passed pawn fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluatePassedPawns(boardAFile) == EvalConstants::PASSED_PAWN_BONUS[4],
            "a-file passed pawn is not blocked by black pawn on h-file"
        );
        ManualTests::expect(
            PawnStructure::evaluatePassedPawns(boardHFile) == EvalConstants::PASSED_PAWN_BONUS[4],
            "h-file passed pawn is not blocked by black pawn on a-file"
        );
    }

    void testEnemyPawnOnAdjacentFilePreventsPassedPawnStatus() {
        // white pawn on d4 and black pawn on e5 stand ahead of each other on adjacent files
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/4p3/3P4/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "enemy pawn on adjacent file fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluatePassedPawns(board) == 0,
            "enemy pawn ahead on adjacent file prevents passed-pawn bonus"
        );
    }

    void testEnemyPawnBehindPawnDoesNotPreventPassedPawnStatus() {
        // white pawn on d5 is already past black pawn on e4 (which is blocked by white pawn on e3)
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/3P4/4p3/4P3/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "enemy pawn behind passed pawn fen loading");
            return;
        }

        ManualTests::expect(
            PawnStructure::evaluatePassedPawns(board) == EvalConstants::PASSED_PAWN_BONUS[4],
            "enemy pawn behind white pawn on adjacent file does not prevent passed-pawn status"
        );
    }

    void runPawnStructureTests() {
        testDoubledWhitePawns();
        testDoubledBlackPawns();
        testTriplePawnsReceiveTwoPenalties();
        testIsolatedWhitePawn();
        testIsolatedBlackPawn();
        testPawnSupportedByFriendlyPawnOnLeftAdjacentFile();
        testPawnSupportedByFriendlyPawnOnRightAdjacentFile();
        testPassedWhitePawn();
        testPassedBlackPawn();
        testBlockedWhitePawnIsNotPassed();
        testBlockedBlackPawnIsNotPassed();
        testAdvancedPassedPawnReceivesLargerBonus();
        testEdgeFilePassedPawn();
        testEnemyPawnOnAdjacentFilePreventsPassedPawnStatus();
        testEnemyPawnBehindPawnDoesNotPreventPassedPawnStatus();
    }
} // namespace EvaluationTests
