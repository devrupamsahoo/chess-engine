//
// Created by devrupamsahoo on 10-11-2026.
//

#include "TestMaterialAndPSTs.h"

#include <cstdlib>
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
        // loads a fen string into a chessboard instance
        bool loadPosition(
            ChessBoard &board,
            const std::string &fen
        ) {
            const bool loaded = FEN::load(board, fen);
            return loaded;
        }

        // verifies that material evaluation matches the expected white-perspective score
        void expectMaterialScore(
            const std::string &fen,
            int expectedScore,
            const std::string &testName
        ) {
            ChessBoard board;
            const bool loaded = loadPosition(board, fen);
            if (!loaded) {
                ManualTests::expect(false, testName + " (fen load failed)");
                return;
            }

            Evaluation evaluation;
            ManualTests::expect(
                evaluation.evaluateMaterial(board) == expectedScore,
                testName
            );
        }

        // verifies that piece-square table evaluation matches the expected score
        void expectPSTScore(
            const std::string &fen,
            int expectedScore,
            const std::string &testName
        ) {
            ChessBoard board;
            const bool loaded = loadPosition(board, fen);
            if (!loaded) {
                ManualTests::expect(false, testName + " (fen load failed)");
                return;
            }

            Evaluation evaluation;
            ManualTests::expect(
                evaluation.evaluatePieceSquareTables(board) == expectedScore,
                testName
            );
        }

        // checks whether a single-piece pst delta matches either direct or rank-mirrored indexing
        bool matchesTableDelta(
            int actualDelta,
            const int table[64],
            int squareA,
            int squareB
        ) {
            const int directDelta = table[squareA] - table[squareB];
            const int flippedDelta = table[squareA ^ 56] - table[squareB ^ 56];
            return actualDelta == directDelta || actualDelta == flippedDelta;
        }
    } // namespace

    void testEqualMaterialProducesEqualMaterialScore() {
        // verify starting position and symmetric positions yield zero material balance
        expectMaterialScore(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
            0,
            "equal material in starting position produces zero material score"
        );

        expectMaterialScore(
            "4k3/8/8/8/8/8/8/4K3 w - - 0 1",
            0,
            "kings-only position produces zero material score"
        );

        expectMaterialScore(
            "2r1k2q/1p2b1n1/8/8/8/8/1P2B1N1/2R1K2Q w - - 0 1",
            0,
            "equal mixed material produces zero material score"
        );
    }

    void testPawnMaterialAndPawnPST() {
        // verify single white and black pawn material values
        expectMaterialScore(
            "4k3/8/8/8/3P4/8/8/4K3 w - - 0 1",
            EvalConstants::PAWN_VALUE,
            "single white pawn adds PAWN_VALUE to material score"
        );

        expectMaterialScore(
            "4k3/8/8/3p4/8/8/8/4K3 w - - 0 1",
            -EvalConstants::PAWN_VALUE,
            "single black pawn subtracts PAWN_VALUE from material score"
        );

        // compare white pawn on d5 (square 35) vs a2 (square 8) with identical kings
        ChessBoard boardD5;
        ChessBoard boardA2;
        if (!loadPosition(boardD5, "4k3/8/8/3P4/8/8/8/4K3 w - - 0 1") ||
            !loadPosition(boardA2, "4k3/8/8/8/8/8/P7/4K3 w - - 0 1")) {
            ManualTests::expect(false, "pawn pst comparison fen loading");
            return;
        }

        Evaluation evaluation;
        const int pstDelta =
                evaluation.evaluatePieceSquareTables(boardD5) -
                evaluation.evaluatePieceSquareTables(boardA2);

        ManualTests::expect(
            matchesTableDelta(pstDelta, EvalConstants::PAWN_PST, 35, 8),
            "pawn pst difference matches EvalConstants::PAWN_PST entry delta"
        );
        ManualTests::expect(
            pstDelta > 0,
            "advanced central white pawn on d5 has higher pst than edge pawn on a2"
        );
    }

    void testKnightMaterialAndKnightPST() {
        // verify knight material balance for white and black
        expectMaterialScore(
            "4k3/8/8/8/3N4/8/8/4K3 w - - 0 1",
            EvalConstants::KNIGHT_VALUE,
            "single white knight adds KNIGHT_VALUE to material score"
        );

        expectMaterialScore(
            "4k3/8/8/3n4/8/8/8/4K3 w - - 0 1",
            -EvalConstants::KNIGHT_VALUE,
            "single black knight subtracts KNIGHT_VALUE from material score"
        );

        // compare white knight on central d4 (square 27) vs corner a1 (square 0)
        ChessBoard boardCenter;
        ChessBoard boardCorner;
        if (!loadPosition(boardCenter, "4k3/8/8/8/3N4/8/8/4K3 w - - 0 1") ||
            !loadPosition(boardCorner, "4k3/8/8/8/8/8/8/N3K3 w - - 0 1")) {
            ManualTests::expect(false, "knight pst comparison fen loading");
            return;
        }

        Evaluation evaluation;
        const int pstDelta =
                evaluation.evaluatePieceSquareTables(boardCenter) -
                evaluation.evaluatePieceSquareTables(boardCorner);

        ManualTests::expect(
            matchesTableDelta(pstDelta, EvalConstants::KNIGHT_PST, 27, 0),
            "knight pst difference matches EvalConstants::KNIGHT_PST entry delta"
        );
        ManualTests::expect(
            pstDelta > 0,
            "centralized white knight on d4 has higher pst than corner knight on a1"
        );
    }

    void testBishopMaterialAndBishopPST() {
        // verify bishop material balance for white and black
        expectMaterialScore(
            "4k3/8/8/8/3B4/8/8/4K3 w - - 0 1",
            EvalConstants::BISHOP_VALUE,
            "single white bishop adds BISHOP_VALUE to material score"
        );

        expectMaterialScore(
            "4k3/8/8/3b4/8/8/8/4K3 w - - 0 1",
            -EvalConstants::BISHOP_VALUE,
            "single black bishop subtracts BISHOP_VALUE from material score"
        );

        // compare white bishop on central d4 (square 27) vs corner a1 (square 0)
        ChessBoard boardCenter;
        ChessBoard boardCorner;
        if (!loadPosition(boardCenter, "4k3/8/8/8/3B4/8/8/4K3 w - - 0 1") ||
            !loadPosition(boardCorner, "4k3/8/8/8/8/8/8/B3K3 w - - 0 1")) {
            ManualTests::expect(false, "bishop pst comparison fen loading");
            return;
        }

        Evaluation evaluation;
        const int pstDelta =
                evaluation.evaluatePieceSquareTables(boardCenter) -
                evaluation.evaluatePieceSquareTables(boardCorner);

        ManualTests::expect(
            matchesTableDelta(pstDelta, EvalConstants::BISHOP_PST, 27, 0),
            "bishop pst difference matches EvalConstants::BISHOP_PST entry delta"
        );
        ManualTests::expect(
            pstDelta > 0,
            "centralized white bishop on d4 has higher pst than corner bishop on a1"
        );
    }

    void testRookMaterialAndRookPST() {
        // verify rook material balance for white and black
        expectMaterialScore(
            "4k3/8/8/8/8/8/3R4/4K3 w - - 0 1",
            EvalConstants::ROOK_VALUE,
            "single white rook adds ROOK_VALUE to material score"
        );

        expectMaterialScore(
            "4k3/3r4/8/8/8/8/8/4K3 w - - 0 1",
            -EvalConstants::ROOK_VALUE,
            "single black rook subtracts ROOK_VALUE from material score"
        );

        // compare white rook on 7th rank d7 (square 51) vs edge a2 (square 8)
        ChessBoard boardSeventh;
        ChessBoard boardEdge;
        if (!loadPosition(boardSeventh, "4k3/3R4/8/8/8/8/8/4K3 w - - 0 1") ||
            !loadPosition(boardEdge, "4k3/8/8/8/8/8/R7/4K3 w - - 0 1")) {
            ManualTests::expect(false, "rook pst comparison fen loading");
            return;
        }

        Evaluation evaluation;
        const int pstDelta =
                evaluation.evaluatePieceSquareTables(boardSeventh) -
                evaluation.evaluatePieceSquareTables(boardEdge);

        ManualTests::expect(
            matchesTableDelta(pstDelta, EvalConstants::ROOK_PST, 51, 8),
            "rook pst difference matches EvalConstants::ROOK_PST entry delta"
        );
        ManualTests::expect(
            pstDelta > 0,
            "white rook on d7 has higher pst than passive rook on a2"
        );
    }

    void testQueenMaterialAndQueenPST() {
        // verify queen material balance for white and black
        expectMaterialScore(
            "4k3/8/8/8/3Q4/8/8/4K3 w - - 0 1",
            EvalConstants::QUEEN_VALUE,
            "single white queen adds QUEEN_VALUE to material score"
        );

        expectMaterialScore(
            "4k3/8/8/3q4/8/8/8/4K3 w - - 0 1",
            -EvalConstants::QUEEN_VALUE,
            "single black queen subtracts QUEEN_VALUE from material score"
        );

        // compare white queen on central e4 (square 28) vs corner a1 (square 0)
        ChessBoard boardCenter;
        ChessBoard boardCorner;
        if (!loadPosition(boardCenter, "4k3/8/8/8/4Q3/8/8/4K3 b - - 0 1") ||
    !loadPosition(boardCorner, "4k3/8/8/8/8/8/8/Q3K3 b - - 0 1")) {
            ManualTests::expect(false, "queen pst comparison fen loading");
            return;
        }

        Evaluation evaluation;
        const int pstDelta =
                evaluation.evaluatePieceSquareTables(boardCenter) -
                evaluation.evaluatePieceSquareTables(boardCorner);

        ManualTests::expect(
            matchesTableDelta(pstDelta, EvalConstants::QUEEN_PST, 28, 0),
            "queen pst difference matches EvalConstants::QUEEN_PST entry delta"
        );
        ManualTests::expect(
            pstDelta > 0,
            "centralized white queen on e4 has higher pst than corner queen on a1"
        );
    }

    void testKingMiddleGamePST() {
        // in a full-material phase-24 position, castled king on g1 (square 6) outperforms exposed king on e4 (square 28)
        ChessBoard boardCastled;
        ChessBoard boardExposed;
        const std::string castledFen =
        "rnbqkbnr/3p4/1NB5/8/8/8/8/RNBQ1RK1 w - - 0 1";

        const std::string exposedFen =
                "rnbqkbnr/3p4/1NB5/8/3K4/8/8/RNBQ1R2 w - - 0 1";

        if (!loadPosition(boardCastled, castledFen) ||
            !loadPosition(boardExposed, exposedFen)) {
            ManualTests::expect(false, "king middle-game pst fen loading");
            return;
        }

        Evaluation evaluation;
        const int pstDelta =
                evaluation.evaluatePieceSquareTables(boardCastled) -
                evaluation.evaluatePieceSquareTables(boardExposed);

        ManualTests::expect(
            matchesTableDelta(pstDelta, EvalConstants::KING_MIDDLEGAME_PST, 6, 28),
            "king middle-game pst difference at phase 24 matches KING_MIDDLEGAME_PST delta"
        );
        ManualTests::expect(
            pstDelta > 0,
            "castled white king on g1 has higher middle-game pst than exposed king on e4"
        );
    }

    void testKingEndGamePST() {
        // in a kings-only phase-0 position, centralized king on d4 (square 27) outperforms corner king on a1 (square 0)
        ChessBoard boardCenter;
        ChessBoard boardCorner;
        if (!loadPosition(boardCenter, "4k3/8/8/8/3K4/8/8/8 w - - 0 1") ||
            !loadPosition(boardCorner, "4k3/8/8/8/8/8/8/K7 w - - 0 1")) {
            ManualTests::expect(false, "king end-game pst fen loading");
            return;
        }

        Evaluation evaluation;
        const int pstDelta =
                evaluation.evaluatePieceSquareTables(boardCenter) -
                evaluation.evaluatePieceSquareTables(boardCorner);

        ManualTests::expect(
            matchesTableDelta(pstDelta, EvalConstants::KING_ENDGAME_PST, 27, 0),
            "king end-game pst difference at phase 0 matches KING_ENDGAME_PST delta"
        );
        ManualTests::expect(
            pstDelta > 0,
            "centralized white king on d4 has higher end-game pst than corner king on a1"
        );
    }

    void testKingInterpolationAtPhaseZero() {
        // verify phase 0 uses pure endgame king pst without middlegame contribution
        ChessBoard boardD4;
        ChessBoard boardG1;
        if (!loadPosition(boardD4, "4k3/8/8/8/3K4/8/8/8 w - - 0 1") ||
            !loadPosition(boardG1, "4k3/8/8/8/8/8/8/6K1 w - - 0 1")) {
            ManualTests::expect(false, "king interpolation at phase 0 fen loading");
            return;
        }

        Evaluation evaluation;
        const int phase = GamePhase::calculateGamePhase(boardD4);
        const int pstDelta =
                evaluation.evaluatePieceSquareTables(boardD4) -
                evaluation.evaluatePieceSquareTables(boardG1);

        ManualTests::expect(
            phase == 0,
            "kings-only board has phase 0 for king interpolation test"
        );
        ManualTests::expect(
            matchesTableDelta(pstDelta, EvalConstants::KING_ENDGAME_PST, 27, 6),
            "king interpolation at phase 0 equals exact KING_ENDGAME_PST difference"
        );
    }

    void testKingInterpolationAtPhaseTwentyFour() {
        // verify phase 24 uses pure middlegame king pst without endgame contribution
        ChessBoard boardG1;
        ChessBoard boardD4;
        if (!loadPosition(
        boardG1,
        "rnbqkbnr/3p4/2B5/8/8/8/8/RNBQNRK1 w - - 0 1"
    ) ||
    !loadPosition(
        boardD4,
        "rnbqkbnr/3p4/2B5/8/3K4/8/8/RNBQNR2 w - - 0 1"
    )) {
            ManualTests::expect(false, "king interpolation at phase 24 fen loading");
            return;
        }

        Evaluation evaluation;
        const int phase = GamePhase::calculateGamePhase(boardG1);
        const int pstDelta =
                evaluation.evaluatePieceSquareTables(boardG1) -
                evaluation.evaluatePieceSquareTables(boardD4);

        ManualTests::expect(
            phase == 24,
            "full non-pawn material board has phase 24 for king interpolation test"
        );
        ManualTests::expect(
            matchesTableDelta(pstDelta, EvalConstants::KING_MIDDLEGAME_PST, 6, 27),
            "king interpolation at phase 24 equals exact KING_MIDDLEGAME_PST difference"
        );
    }

    void testWhiteBlackPSTMirroring() {
        // mirrored white and black piece placements cancel to zero in white-perspective pst
        expectPSTScore(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
            0,
            "starting position has zero net pst due to white/black mirroring"
        );

        expectPSTScore(
            "4k3/8/8/3n4/3N4/8/8/4K3 w - - 0 1",
            0,
            "mirrored knights on d4 and d5 cancel out in pst evaluation"
        );

        expectPSTScore(
            "2r1k3/8/5b2/3q4/3Q4/5B2/8/2R1K3 w - - 0 1",
            0,
            "mirrored queens, rooks, and bishops cancel out in pst evaluation"
        );

        // swapping colors across vertical mirror negates the net pst score
        ChessBoard whiteActive;
        ChessBoard blackActive;
        if (!loadPosition(whiteActive, "4k3/n7/8/8/3N4/8/8/4K3 w - - 0 1") ||
            !loadPosition(blackActive, "4k3/8/8/3n4/8/8/N7/4K3 w - - 0 1")) {
            ManualTests::expect(false, "white/black pst mirroring asymmetry fen loading");
            return;
        }

        Evaluation evaluation;
        const int whiteScore = evaluation.evaluatePieceSquareTables(whiteActive);
        const int blackScore = evaluation.evaluatePieceSquareTables(blackActive);

        ManualTests::expect(
            whiteScore == -blackScore && whiteScore > 0,
            "swapping white and black mirrored piece squares negates pst score"
        );
    }

    void testMaterialAccumulation() {
        // verify cumulative material calculation across multiple pieces of each type
        const int expectedWhiteHeavy =
                2 * EvalConstants::PAWN_VALUE +
                EvalConstants::KNIGHT_VALUE +
                EvalConstants::BISHOP_VALUE +
                EvalConstants::ROOK_VALUE +
                EvalConstants::QUEEN_VALUE;

        expectMaterialScore(
            "4k3/8/8/8/8/8/PP6/RNBQK3 w - - 0 1",
            expectedWhiteHeavy,
            "material accumulation sums multiple white piece values accurately"
        );

        const int expectedMixedBalance =
                (EvalConstants::QUEEN_VALUE + 2 * EvalConstants::ROOK_VALUE + 3 * EvalConstants::PAWN_VALUE) -
                (2 * EvalConstants::BISHOP_VALUE + EvalConstants::KNIGHT_VALUE + EvalConstants::PAWN_VALUE);

        expectMaterialScore(
            "2bbnk2/p7/8/8/8/8/PPP5/2RRQK2 w - - 0 1",
            expectedMixedBalance,
            "material accumulation computes net balance across unequal white and black armies"
        );
    }

    void testCapturingPieceChangesMaterialByExpectedValue() {
        // capturing a black rook on d4 with a white knight from f3 increases material by rook_value
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/3r4/5N2/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "capture material change fen loading");
            return;
        }

        Evaluation evaluation;
        const int beforeCapture = evaluation.evaluateMaterial(board);

        // f3 = 21, d4 = 27
        board.makeMove(21, 27);
        const int afterCapture = evaluation.evaluateMaterial(board);

        ManualTests::expect(
            afterCapture - beforeCapture == EvalConstants::ROOK_VALUE,
            "capturing black rook changes material score by exactly ROOK_VALUE"
        );

        board.undoMove();
        const int afterUndo = evaluation.evaluateMaterial(board);
        ManualTests::expect(
            afterUndo == beforeCapture,
            "undoing capture restores original material score"
        );
    }

    void runMaterialAndPieceSquareTablesTests() {
        testEqualMaterialProducesEqualMaterialScore();
        testPawnMaterialAndPawnPST();
        testKnightMaterialAndKnightPST();
        testBishopMaterialAndBishopPST();
        testRookMaterialAndRookPST();
        testQueenMaterialAndQueenPST();
        testKingMiddleGamePST();
        testKingEndGamePST();
        testKingInterpolationAtPhaseZero();
        testKingInterpolationAtPhaseTwentyFour();
        testWhiteBlackPSTMirroring();
        testMaterialAccumulation();
        testCapturingPieceChangesMaterialByExpectedValue();
    }
} // namespace EvaluationTests
