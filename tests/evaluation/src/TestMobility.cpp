//
// Created by devrupamsahoo on 10-11-2026.
//

#include "TestMobility.h"

#include <string>

#include "ManualTestUtils.h"

#include "chess/core/ChessBoard.h"
#include "chess/core/Enums.h"
#include "chess/evaluation/EvaluationConstants.h"
#include "chess/evaluation/Mobility.h"
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

    void testKnightMobilityOnOpenBoard() {
        // white knight on d4 with kings on b1 and b8 has 8 available jumps
        ChessBoard board;
        if (!loadPosition(board, "1k6/8/8/8/3N4/8/8/1K6 w - - 0 1")) {
            ManualTests::expect(false, "knight mobility on open board fen loading");
            return;
        }

        ManualTests::expect(
            Mobility::evaluateMobility(board) == 8 * EvalConstants::KNIGHT_MOBILITY_WEIGHT,
            "knight on d4 on open board has 8 * KNIGHT_MOBILITY_WEIGHT mobility"
        );
    }

    void testBishopMobilityOnOpenBoard() {
        // white bishop on d4 with kings on b1 and b8 has 13 available diagonal squares
        ChessBoard board;
        if (!loadPosition(board, "1k6/8/8/8/3B4/8/8/1K6 w - - 0 1")) {
            ManualTests::expect(false, "bishop mobility on open board fen loading");
            return;
        }

        ManualTests::expect(
            Mobility::evaluateMobility(board) == 13 * EvalConstants::BISHOP_MOBILITY_WEIGHT,
            "bishop on d4 on open board has 13 * BISHOP_MOBILITY_WEIGHT mobility"
        );
    }

    void testRookMobilityOnOpenBoard() {
        // white rook on d4 with kings on b1 and b8 has 14 available orthogonal squares
        ChessBoard board;
        if (!loadPosition(board, "1k6/8/8/8/3R4/8/8/1K6 w - - 0 1")) {
            ManualTests::expect(false, "rook mobility on open board fen loading");
            return;
        }

        ManualTests::expect(
            Mobility::evaluateMobility(board) == 14 * EvalConstants::ROOK_MOBILITY_WEIGHT,
            "rook on d4 on open board has 14 * ROOK_MOBILITY_WEIGHT mobility"
        );
    }

    void testQueenMobilityOnOpenBoard() {
        // white queen on d4 with kings on b1 and b8 has 27 available squares
        ChessBoard board;
        if (!loadPosition(board, "1k6/8/8/8/3Q4/8/8/1K6 w - - 0 1")) {
            ManualTests::expect(false, "queen mobility on open board fen loading");
            return;
        }

        ManualTests::expect(
            Mobility::evaluateMobility(board) == 27 * EvalConstants::QUEEN_MOBILITY_WEIGHT,
            "queen on d4 on open board has 27 * QUEEN_MOBILITY_WEIGHT mobility"
        );
    }

    void testOwnPieceBlocking() {
        // friendly pawns adjacent to white rook on d4 block its rays
        ChessBoard openBoard;
        ChessBoard blockedBoard;
        if (!loadPosition(openBoard, "1k6/8/8/8/3R4/8/8/1K6 w - - 0 1") ||
            !loadPosition(blockedBoard, "1k6/8/8/3P4/2PRP3/3P4/8/1K6 w - - 0 1")) {
            ManualTests::expect(false, "own-piece blocking mobility fen loading");
            return;
        }

        ManualTests::expect(
            Mobility::evaluateMobility(blockedBoard) == 0 &&
            Mobility::evaluateMobility(openBoard) > Mobility::evaluateMobility(blockedBoard),
            "own pawns surrounding rook completely block its mobility"
        );
    }

    void testEnemyCaptureSquareIncluded() {
        // white rook on a1 boxed in on b1 by white king and on a2 by enemy pawn (1 capture square) vs friendly pawn (0 squares)
        ChessBoard enemyOnA2;
        ChessBoard friendlyOnA2;
        if (!loadPosition(enemyOnA2, "6k1/8/8/8/8/8/p7/RK6 w - - 0 1") ||
            !loadPosition(friendlyOnA2, "6k1/8/8/8/8/8/P7/RK6 w - - 0 1")) {
            ManualTests::expect(false, "enemy capture square included fen loading");
            return;
        }

        ManualTests::expect(
            Mobility::evaluateMobility(enemyOnA2) == EvalConstants::ROOK_MOBILITY_WEIGHT &&
            Mobility::evaluateMobility(friendlyOnA2) == 0,
            "enemy capture square is included in piece mobility count"
        );
    }

    void testSquaresBeyondEnemyPieceExcluded() {
        // white rook on d1 with enemy pawn on d3 can reach d2 and d3 (2 vertical squares), not d4..d8
        ChessBoard enemyOnD3;
        ChessBoard enemyOnD6;
        if (!loadPosition(enemyOnD3, "1k6/8/8/8/8/3p4/8/1K1R4 w - - 0 1") ||
            !loadPosition(enemyOnD6, "1k6/8/3p4/8/8/8/8/1K1R4 w - - 0 1")) {
            ManualTests::expect(false, "squares beyond enemy piece excluded fen loading");
            return;
        }

        const int diff = Mobility::evaluateMobility(enemyOnD6) - Mobility::evaluateMobility(enemyOnD3);
        ManualTests::expect(
            diff == 3 * EvalConstants::ROOK_MOBILITY_WEIGHT,
            "squares beyond an enemy piece along a ray are excluded from mobility"
        );
    }

    void testCornerAndEdgePositions() {
        // white knight in corner a1 (2 moves) < edge a4 (4 moves) < center d4 (8 moves)
        ChessBoard cornerKnight;
        ChessBoard edgeKnight;
        ChessBoard centerKnight;
        if (!loadPosition(cornerKnight, "6k1/8/8/8/8/8/8/N5K1 w - - 0 1") ||
            !loadPosition(edgeKnight, "6k1/8/8/8/N7/8/8/6K1 w - - 0 1") ||
            !loadPosition(centerKnight, "6k1/8/8/8/3N4/8/8/6K1 w - - 0 1")) {
            ManualTests::expect(false, "corner and edge mobility fen loading");
            return;
        }

        const int cornerScore = Mobility::evaluateMobility(cornerKnight);
        const int edgeScore = Mobility::evaluateMobility(edgeKnight);
        const int centerScore = Mobility::evaluateMobility(centerKnight);

        ManualTests::expect(
            cornerScore == 2 * EvalConstants::KNIGHT_MOBILITY_WEIGHT &&
            edgeScore == 4 * EvalConstants::KNIGHT_MOBILITY_WEIGHT &&
            centerScore == 8 * EvalConstants::KNIGHT_MOBILITY_WEIGHT,
            "knight mobility increases from corner (2) to edge (4) to center (8)"
        );
    }

    void testWhiteBlackMobilitySymmetry() {
        // mirrored white and black positions have opposite mobility scores
        ChessBoard whiteAdvantage;
        ChessBoard blackAdvantage;
        ChessBoard symmetric;
        if (!loadPosition(whiteAdvantage, "n5k1/8/8/8/3N4/8/8/6K1 w - - 0 1") ||
            !loadPosition(blackAdvantage, "6k1/8/8/3n4/8/8/8/N5K1 w - - 0 1") ||
            !loadPosition(symmetric, "1k6/8/8/3n4/3N4/8/8/1K6 w - - 0 1")) {
            ManualTests::expect(false, "white/black mobility symmetry fen loading");
            return;
        }

        const int whiteScore = Mobility::evaluateMobility(whiteAdvantage);
        const int blackScore = Mobility::evaluateMobility(blackAdvantage);
        const int symScore = Mobility::evaluateMobility(symmetric);

        ManualTests::expect(
            whiteScore == -blackScore && whiteScore > 0 && symScore == 0,
            "mobility evaluation is symmetric between white and black"
        );
    }

    void testRemovingBlockerIncreasesMobility() {
        // removing friendly pawn on e5 from in front of white bishop on d4 increases mobility by 4 squares
        ChessBoard blockedBishop;
        ChessBoard openBishop;
        if (!loadPosition(blockedBishop, "1k6/8/8/4P3/3B4/8/8/1K6 w - - 0 1") ||
            !loadPosition(openBishop, "1k6/8/8/8/3B4/8/8/1K6 w - - 0 1")) {
            ManualTests::expect(false, "removing blocker increases mobility fen loading");
            return;
        }

        const int blockedScore = Mobility::evaluateMobility(blockedBishop);
        const int openScore = Mobility::evaluateMobility(openBishop);

        ManualTests::expect(
            openScore - blockedScore == 4 * EvalConstants::BISHOP_MOBILITY_WEIGHT,
            "removing friendly blocker on e5 increases d4 bishop mobility by 4 squares"
        );
    }

    void testAddingBlockerDecreasesMobility() {
        // adding a friendly pawn on d5 above white queen on d4 blocks 4 north-ray squares (d5, d6, d7, d8)
        ChessBoard openQueen;
        ChessBoard blockedQueen;
        if (!loadPosition(openQueen, "1k6/8/8/8/3Q4/8/8/1K6 w - - 0 1") ||
            !loadPosition(blockedQueen, "1k6/8/8/3P4/3Q4/8/8/1K6 w - - 0 1")) {
            ManualTests::expect(false, "adding blocker decreases mobility fen loading");
            return;
        }

        const int openScore = Mobility::evaluateMobility(openQueen);
        const int blockedScore = Mobility::evaluateMobility(blockedQueen);

        ManualTests::expect(
            openScore - blockedScore == 4 * EvalConstants::QUEEN_MOBILITY_WEIGHT,
            "adding friendly blocker on d5 decreases d4 queen mobility by 4 squares"
        );
    }

    void testFriendlySquaresAreNotCounted() {
        // white knight on a1 with friendly pawns on b3 and c2 has 0 legal target squares
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/8/8/1P6/2P5/N5K1 w - - 0 1")) {
            ManualTests::expect(false, "friendly squares not counted fen loading");
            return;
        }

        ManualTests::expect(
            Mobility::evaluateMobility(board) == 0,
            "squares occupied by friendly pieces are not counted in mobility"
        );
    }

    void testMobilityWeightsAreRespected() {
        // verify each piece type scales each available square by its configured mobility weight
        ManualTests::expect(
            EvalConstants::KNIGHT_MOBILITY_WEIGHT == 4 &&
            EvalConstants::BISHOP_MOBILITY_WEIGHT == 3 &&
            EvalConstants::ROOK_MOBILITY_WEIGHT == 2 &&
            EvalConstants::QUEEN_MOBILITY_WEIGHT == 1,
            "mobility weight constants match configured values (4, 3, 2, 1)"
        );

        // 1-square mobility for each piece in corner a1 (with b2, a2, b1 blocked by friendly pieces as needed)
        ChessBoard oneMoveRook;
        ChessBoard oneMoveBishop;
        ChessBoard oneMoveQueen;
        if (!loadPosition(oneMoveRook, "6k1/8/8/8/8/8/p7/RK6 w - - 0 1") ||
            !loadPosition(oneMoveBishop, "6k1/8/8/8/8/8/1p6/B5K1 w - - 0 1") ||
            !loadPosition(oneMoveQueen, "6k1/8/8/8/8/8/pP6/QK6 w - - 0 1")) {
            ManualTests::expect(false, "mobility weights respected fen loading");
            return;
        }

        ManualTests::expect(
            Mobility::evaluateMobility(oneMoveRook) == EvalConstants::ROOK_MOBILITY_WEIGHT &&
            Mobility::evaluateMobility(oneMoveBishop) == EvalConstants::BISHOP_MOBILITY_WEIGHT &&
            Mobility::evaluateMobility(oneMoveQueen) == EvalConstants::QUEEN_MOBILITY_WEIGHT,
            "single-square mobility for rook, bishop, and queen equals their respective weights"
        );
    }

    void testAggregateMobilityIsDeterministic() {
        // repeated mobility evaluations on a complex middlegame board return identical results
        ChessBoard board;
        if (!loadPosition(board, "r1bq1rk1/ppp2ppp/2np1n2/2b1p3/2B1P3/2NP1N2/PPP2PPP/R1BQ1RK1 w - - 4 6")) {
            ManualTests::expect(false, "aggregate mobility deterministic fen loading");
            return;
        }

        const int first = Mobility::evaluateMobility(board);
        const int second = Mobility::evaluateMobility(board);
        const int third = Mobility::evaluateMobility(board);

        ManualTests::expect(
            first == second && second == third && first == 0,
            "aggregate mobility is deterministic across repeated calls"
        );
    }

    void runMobilityTests() {
        testKnightMobilityOnOpenBoard();
        testBishopMobilityOnOpenBoard();
        testRookMobilityOnOpenBoard();
        testQueenMobilityOnOpenBoard();
        testOwnPieceBlocking();
        testEnemyCaptureSquareIncluded();
        testSquaresBeyondEnemyPieceExcluded();
        testCornerAndEdgePositions();
        testWhiteBlackMobilitySymmetry();
        testRemovingBlockerIncreasesMobility();
        testAddingBlockerDecreasesMobility();
        testFriendlySquaresAreNotCounted();
        testMobilityWeightsAreRespected();
        testAggregateMobilityIsDeterministic();
    }
} // namespace EvaluationTests
