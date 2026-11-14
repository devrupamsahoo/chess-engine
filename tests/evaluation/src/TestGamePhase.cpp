//
// Created by devrupamsahoo on 10-11-2026.
//

#include "TestGamePhase.h"

#include <string>

#include "ManualTestUtils.h"

#include "chess/core/ChessBoard.h"
#include "chess/core/Enums.h"
#include "chess/evaluation/Evaluation.h"
#include "chess/evaluation/EvaluationConstants.h"
#include "chess/evaluation/GamePhase.h"
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

        // checks that a fen position produces the expected game phase
        void expectPhase(
            const std::string &fen,
            int expectedPhase,
            const std::string &testName
        ) {
            ChessBoard board;
            const bool loaded = loadPosition(board, fen);
            if (!loaded) {
                ManualTests::expect(false, testName + " (fen load failed)");
                return;
            }

            ManualTests::expect(
                GamePhase::calculateGamePhase(board) == expectedPhase,
                testName
            );
        }
    } // namespace

    void testStartingPositionPhaseTwentyFour() {
        // standard starting position has 4 knights (4), 4 bishops (4), 4 rooks (8), and 2 queens (8) = 24
        expectPhase(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
            24,
            "starting position has game phase 24"
        );
    }

    void testRemovingOneKnightLowersPhaseByOne() {
        // removing white's b1 knight lowers phase from 24 to 23
        expectPhase(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R1BQKBNR w KQkq - 0 1",
            23,
            "removing one knight lowers game phase by 1"
        );
    }

    void testRemovingOneBishopLowersPhaseByOne() {
        // removing white's c1 bishop lowers phase from 24 to 23
        expectPhase(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RN1QKBNR w KQkq - 0 1",
            23,
            "removing one bishop lowers game phase by 1"
        );
    }

    void testRemovingOneRookLowersPhaseByTwo() {
        // removing white's a1 rook lowers phase from 24 to 22
        expectPhase(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/1NBQKBNR w Kkq - 0 1",
            22,
            "removing one rook lowers game phase by 2"
        );
    }

    void testRemovingOneQueenLowersPhaseByFour() {
        // removing white's d1 queen lowers phase from 24 to 20
        expectPhase(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1",
            20,
            "removing one queen lowers game phase by 4"
        );
    }

    void testPawnsDoNotAffectPhase() {
        // all 16 pawns with only kings contribute 0 phase
        expectPhase(
            "4k3/pppppppp/8/8/8/8/PPPPPPPP/4K3 w - - 0 1",
            0,
            "pawns with kings only have game phase 0"
        );

        // removing all 16 pawns from starting position keeps phase at 24
        expectPhase(
            "rnbqkbnr/8/8/8/8/8/8/RNBQKBNR w KQkq - 0 1",
            24,
            "removing all pawns from starting position leaves game phase at 24"
        );
    }

    void testKingsOnlyPhaseZero() {
        // bare kings have phase 0
        expectPhase(
            "4k3/8/8/8/8/8/8/4K3 w - - 0 1",
            0,
            "kings-only position has game phase 0"
        );
    }

    void testPhaseClampingBehavior() {
        // position with promoted extra queens exceeding 24 raw phase points clamps to 24
        ChessBoard promotedBoard;
        if (!loadPosition(
        promotedBoard,
        "rnbqkbnr/pppppppp/8/8/8/8/QPPPPPPP/RNBQKBNR w KQkq - 0 1"
    )) {
            ManualTests::expect(false, "phase clamping fen loading");
            return;
        }

        const int clampedPhase = GamePhase::calculateGamePhase(promotedBoard);
        ManualTests::expect(
            clampedPhase == 24,
            "game phase with promoted extra queens is clamped to maximum 24"
        );

        // kings-only position clamps cleanly at 0
        ChessBoard emptyBoard;
        if (!loadPosition(emptyBoard, "4k3/8/8/8/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "phase lower clamping fen loading");
            return;
        }

        ManualTests::expect(
            GamePhase::calculateGamePhase(emptyBoard) == 0,
            "game phase for minimum material is 0 and non-negative"
        );
    }

    void testFullMaterialPositionUsesMiddleGamePhase() {
        // verify full-material board reports phase 24 and evaluates king pst with middlegame table
        ChessBoard castledBoard;
        ChessBoard centerKingBoard;
        if (!loadPosition(castledBoard, "rnbqkbnr/3p4/2B5/8/8/8/8/RNBQNRK1 w - - 0 1") ||
            !loadPosition(centerKingBoard, "rnbqkbnr/3p4/2B5/8/3K4/8/8/RNBQNR2 w - - 0 1")) {
            ManualTests::expect(false, "full-material middle-game phase fen loading");
            return;
        }

        Evaluation evaluation;
        const int phase = GamePhase::calculateGamePhase(castledBoard);
        const int pstDiff =
                evaluation.evaluatePieceSquareTables(castledBoard) -
                evaluation.evaluatePieceSquareTables(centerKingBoard);

        const int directMgDiff =
                EvalConstants::KING_MIDDLEGAME_PST[6] - EvalConstants::KING_MIDDLEGAME_PST[27];
        const int flippedMgDiff =
                EvalConstants::KING_MIDDLEGAME_PST[6 ^ 56] - EvalConstants::KING_MIDDLEGAME_PST[27 ^ 56];

        ManualTests::expect(
            phase == 24 && (pstDiff == directMgDiff || pstDiff == flippedMgDiff),
            "full-material position uses middle-game phase 24 for king pst evaluation"
        );
    }

    void testEndgamePositionUsesEndGamePhase() {
        // verify kings-only board reports phase 0 and evaluates king pst with endgame table
        ChessBoard centerKingBoard;
        ChessBoard cornerKingBoard;
        if (!loadPosition(centerKingBoard, "4k3/8/8/8/3K4/8/8/8 w - - 0 1") ||
            !loadPosition(cornerKingBoard, "4k3/8/8/8/8/8/8/K7 w - - 0 1")) {
            ManualTests::expect(false, "endgame position phase fen loading");
            return;
        }

        Evaluation evaluation;
        const int phase = GamePhase::calculateGamePhase(centerKingBoard);
        const int pstDiff =
                evaluation.evaluatePieceSquareTables(centerKingBoard) -
                evaluation.evaluatePieceSquareTables(cornerKingBoard);

        const int directEgDiff =
                EvalConstants::KING_ENDGAME_PST[27] - EvalConstants::KING_ENDGAME_PST[0];
        const int flippedEgDiff =
                EvalConstants::KING_ENDGAME_PST[27 ^ 56] - EvalConstants::KING_ENDGAME_PST[0 ^ 56];

        ManualTests::expect(
            phase == 0 && (pstDiff == directEgDiff || pstDiff == flippedEgDiff),
            "endgame position uses end-game phase 0 for king pst evaluation"
        );
    }

    void runGamePhaseTests() {
        testStartingPositionPhaseTwentyFour();
        testRemovingOneKnightLowersPhaseByOne();
        testRemovingOneBishopLowersPhaseByOne();
        testRemovingOneRookLowersPhaseByTwo();
        testRemovingOneQueenLowersPhaseByFour();
        testPawnsDoNotAffectPhase();
        testKingsOnlyPhaseZero();
        testPhaseClampingBehavior();
        testFullMaterialPositionUsesMiddleGamePhase();
        testEndgamePositionUsesEndGamePhase();
    }
} // namespace EvaluationTests
