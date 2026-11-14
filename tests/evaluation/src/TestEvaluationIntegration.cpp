//
// Created by devrupamsahoo on 10-11-2026.
//

#include "TestEvaluationIntegration.h"

#include <string>

#include "ManualTestUtils.h"

#include "chess/core/ChessBoard.h"
#include "chess/core/Enums.h"
#include "chess/evaluation/Evaluation.h"
#include "chess/evaluation/EvaluationConstants.h"
#include "chess/evaluation/GamePhase.h"
#include "chess/evaluation/KingSafety.h"
#include "chess/evaluation/Mobility.h"
#include "chess/evaluation/PawnStructure.h"
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

        // verifies that evaluating a position leaves its serialized fen unchanged
        void expectFENUnchanged(
            const std::string &fen,
            const std::string &testName
        ) {
            ChessBoard board;
            if (!loadPosition(board, fen)) {
                ManualTests::expect(false, testName + " (fen load failed)");
                return;
            }

            const std::string beforeFen = FEN::serialize(board);
            Evaluation evaluation;
            (void) evaluation.evaluate(board);
            (void) evaluation.evaluateMaterial(board);
            (void) evaluation.evaluatePieceSquareTables(board);
            const std::string afterFen = FEN::serialize(board);

            ManualTests::expect(
                beforeFen == afterFen,
                testName
            );
        }
    } // namespace

    void testKingsOnlyLegalPositionEvaluatesConsistently() {
        // symmetric kings-only position evaluates to 0 consistently
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "kings-only position fen loading");
            return;
        }

        Evaluation evaluation;
        const int first = evaluation.evaluate(board);
        const int second = evaluation.evaluate(board);

        ManualTests::expect(
            first == 0 && first == second,
            "kings-only symmetric position evaluates consistently to 0"
        );
    }

    void testStartingPositionEvaluatesConsistently() {
        // standard starting position evaluates to 0 consistently
        ChessBoard board;
        if (!loadPosition(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")) {
            ManualTests::expect(false, "starting position fen loading");
            return;
        }

        Evaluation evaluation;
        const int first = evaluation.evaluate(board);
        const int second = evaluation.evaluate(board);

        ManualTests::expect(
            first == 0 && first == second,
            "starting position evaluates consistently to 0"
        );
    }

    void testAddingWhiteQueenIncreasesFullEvaluation() {
        // adding a white queen on d1 with white to move increases full evaluation substantially
        ChessBoard baseBoard;
        ChessBoard queenBoard;
        if (!loadPosition(baseBoard, "4k3/8/8/8/8/8/8/4K3 w - - 0 1") ||
            !loadPosition(queenBoard, "4k3/8/8/8/8/8/8/3QK3 w - - 0 1")) {
            ManualTests::expect(false, "adding white queen fen loading");
            return;
        }

        Evaluation evaluation;
        const int baseScore = evaluation.evaluate(baseBoard);
        const int queenScore = evaluation.evaluate(queenBoard);

        ManualTests::expect(
            queenScore - baseScore >= EvalConstants::QUEEN_VALUE,
            "adding white queen increases full evaluation for white to move"
        );
    }

    void testAddingBlackQueenDecreasesFullEvaluation() {
        // adding a black queen on d8 with white to move decreases full evaluation substantially
        ChessBoard baseBoard;
        ChessBoard blackQueenBoard;
        if (!loadPosition(baseBoard, "4k3/8/8/8/8/8/8/4K3 w - - 0 1") ||
            !loadPosition(blackQueenBoard, "3qk3/8/8/8/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "adding black queen fen loading");
            return;
        }

        Evaluation evaluation;
        const int baseScore = evaluation.evaluate(baseBoard);
        const int blackQueenScore = evaluation.evaluate(blackQueenBoard);

        ManualTests::expect(
            blackQueenScore - baseScore <= -EvalConstants::QUEEN_VALUE,
            "adding black queen decreases full evaluation for white to move"
        );
    }

    void testMirroredColorsProduceOppositeWhitePerspectiveScore() {
        // mirroring all pieces vertically and swapping colors negates white-perspective evaluation
        ChessBoard whiteAdvantage;
        ChessBoard blackAdvantage;
        if (!loadPosition(whiteAdvantage, "r3k2r/ppp2ppp/8/3P4/3N4/2B5/PPP2PPP/R2Q1RK1 w kq - 0 1") ||
            !loadPosition(blackAdvantage, "r2q1rk1/ppp2ppp/2b5/3n4/3p4/8/PPP2PPP/R3K2R w KQ - 0 1")) {
            ManualTests::expect(false, "mirrored colors fen loading");
            return;
        }

        Evaluation evaluation;
        const int scoreWhiteAdv = evaluation.evaluate(whiteAdvantage);
        const int scoreBlackAdv = evaluation.evaluate(blackAdvantage);

        ManualTests::expect(
            scoreWhiteAdv == -scoreBlackAdv && scoreWhiteAdv > 0,
            "mirrored colors produce opposite white-perspective full evaluation scores"
        );
    }

    void testSwitchingSideToMoveNegatesFullEvaluation() {
        // switching turn on an asymmetric position negates evaluate(board)
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/3P4/3N4/8/8/3QK3 w - - 0 1")) {
            ManualTests::expect(false, "switching side to move fen loading");
            return;
        }

        Evaluation evaluation;
        const int whiteTurnScore = evaluation.evaluate(board);
        board.switchTurn();
        const int blackTurnScore = evaluation.evaluate(board);

        ManualTests::expect(
            whiteTurnScore == -blackTurnScore && whiteTurnScore > 0,
            "switching side to move negates full evaluation score"
        );
    }

    void testRepeatedEvaluationIsDeterministic() {
        // evaluating the same board multiple times yields identical results
        ChessBoard board;
        if (!loadPosition(board, "r1bq1rk1/pp2ppbp/2np1np1/8/3NP3/2N1BP2/PPPQ2PP/2KR1B1R b - - 3 8")) {
            ManualTests::expect(false, "repeated evaluation deterministic fen loading");
            return;
        }

        Evaluation evaluation;
        const int first = evaluation.evaluate(board);
        const int second = evaluation.evaluate(board);
        const int third = evaluation.evaluate(board);

        ManualTests::expect(
            first == second && second == third,
            "repeated evaluation calls return identical scores"
        );
    }

    void testEvaluationDoesNotMutateFEN() {
        // verify serialized fen is identical before and after evaluation
        expectFENUnchanged(
            "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
            "evaluation does not mutate serialized FEN"
        );
    }

    void testEvaluationDoesNotChangeSideToMove() {
        // verify board.iswhiteturn() is preserved for both white and black to move
        ChessBoard whiteBoard;
        ChessBoard blackBoard;
        if (!loadPosition(whiteBoard, "4k3/8/8/8/3N4/8/8/4K3 w - - 0 1") ||
            !loadPosition(blackBoard, "4k3/8/8/8/3N4/8/8/4K3 b - - 0 1")) {
            ManualTests::expect(false, "evaluation side to move invariance fen loading");
            return;
        }

        Evaluation evaluation;
        (void) evaluation.evaluate(whiteBoard);
        (void) evaluation.evaluate(blackBoard);

        ManualTests::expect(
            whiteBoard.isWhiteTurn() && !blackBoard.isWhiteTurn(),
            "evaluation does not change side to move"
        );
    }

    void testEvaluationDoesNotChangeCastlingRights() {
        // verify castling rights remain intact after evaluation
        ChessBoard board;
        const std::string fen = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1";
        if (!loadPosition(board, fen)) {
            ManualTests::expect(false, "evaluation castling rights invariance fen loading");
            return;
        }

        const CastlingRights &rightsBefore = board.getCastlingRights();
        const std::string fenBefore = FEN::serialize(board);

        Evaluation evaluation;
        (void) evaluation.evaluate(board);

        const CastlingRights &rightsAfter = board.getCastlingRights();
        const std::string fenAfter = FEN::serialize(board);

        ManualTests::expect(
            &rightsBefore == &rightsAfter && fenBefore == fenAfter,
            "evaluation does not change castling rights"
        );
    }

    void testEvaluationDoesNotChangeEnPassantTarget() {
        // verify en-passant target square is unchanged by evaluation
        ChessBoard board;
        if (!loadPosition(board, "rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2")) {
            ManualTests::expect(false, "evaluation en-passant target invariance fen loading");
            return;
        }

        const int epBefore = board.getEnPassantTarget();
        Evaluation evaluation;
        (void) evaluation.evaluate(board);
        const int epAfter = board.getEnPassantTarget();

        ManualTests::expect(
            epBefore == epAfter && epBefore == 43,
            "evaluation does not change en-passant target square"
        );
    }

    void testEvaluationDoesNotChangeMoveCounters() {
        // verify halfmove clock and serialized fullmove counter are unchanged by evaluation
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/3N4/8/8/4K3 w - - 17 42")) {
            ManualTests::expect(false, "evaluation move counters invariance fen loading");
            return;
        }

        const int halfmoveBefore = board.getHalfmoveClock();
        const std::string fenBefore = FEN::serialize(board);

        Evaluation evaluation;
        (void) evaluation.evaluate(board);

        const int halfmoveAfter = board.getHalfmoveClock();
        const std::string fenAfter = FEN::serialize(board);

        ManualTests::expect(
            halfmoveBefore == 17 && halfmoveAfter == 17 && fenBefore == fenAfter,
            "evaluation does not change move counters"
        );
    }

    void testEvaluationDoesNotChangeUndoRedoAvailability() {
        // verify canundo() and canredo() states remain unchanged across evaluation calls
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/3N4/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "evaluation undo/redo invariance fen loading");
            return;
        }

        Evaluation evaluation;
        board.makeMove(27, 44); // d4 -> e6
        const bool undoBefore1 = board.canUndo();
        const bool redoBefore1 = board.canRedo();
        (void) evaluation.evaluate(board);
        const bool undoAfter1 = board.canUndo();
        const bool redoAfter1 = board.canRedo();

        board.undoMove();
        const bool undoBefore2 = board.canUndo();
        const bool redoBefore2 = board.canRedo();
        (void) evaluation.evaluate(board);
        const bool undoAfter2 = board.canUndo();
        const bool redoAfter2 = board.canRedo();

        ManualTests::expect(
            undoBefore1 && !redoBefore1 &&
            undoBefore1 == undoAfter1 && redoBefore1 == redoAfter1 &&
            !undoBefore2 && redoBefore2 &&
            undoBefore2 == undoAfter2 && redoBefore2 == redoAfter2,
            "evaluation does not change undo or redo availability"
        );
    }

    void testMaterialIsNotIncludedInEvaluatePieceSquareTables() {
        // adding a white queen changes material by 900 while pst stays within small positional bounds
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/3Q4/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "material not included in pst fen loading");
            return;
        }

        Evaluation evaluation;
        const int pstScore = evaluation.evaluatePieceSquareTables(board);
        const int materialScore = evaluation.evaluateMaterial(board);

        ManualTests::expect(
            materialScore == EvalConstants::QUEEN_VALUE &&
            pstScore > -100 && pstScore < 100,
            "material value is not included in evaluatePieceSquareTables()"
        );
    }

    void testPSTIsNotIncludedInEvaluateMaterial() {
        // relocating a white knight from corner a1 to center d4 changes pst while material stays 320
        ChessBoard cornerKnight;
        ChessBoard centerKnight;
        if (!loadPosition(cornerKnight, "4k3/8/8/8/8/8/8/N3K3 w - - 0 1") ||
            !loadPosition(centerKnight, "4k3/8/8/8/3N4/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "pst not included in material fen loading");
            return;
        }

        Evaluation evaluation;
        const int cornerMat = evaluation.evaluateMaterial(cornerKnight);
        const int centerMat = evaluation.evaluateMaterial(centerKnight);
        const int cornerPST = evaluation.evaluatePieceSquareTables(cornerKnight);
        const int centerPST = evaluation.evaluatePieceSquareTables(centerKnight);

        ManualTests::expect(
            cornerMat == EvalConstants::KNIGHT_VALUE &&
            centerMat == EvalConstants::KNIGHT_VALUE &&
            centerPST != cornerPST,
            "piece-square table values are not included in evaluateMaterial()"
        );
    }

    void testEveryEvaluationFeatureIsIncludedExactlyOnce() {
        // construct a position activating material, pst, bishop pair, rook open file + 7th rank,
        // doubled/isolated/passed pawns, mobility, and king safety, then verify exact sum
        ChessBoard board;
        // white: king on g1 (6), rook on d7 (51), bishops on c4 (26) and f4 (29), pawns on a2 (8), a3 (16), c6 (42)
        // black: king on g8 (62), knight on b8 (57), pawn on h7 (55)
        if (!loadPosition(board, "1n4k1/3R3p/2P5/8/2B2B2/P7/P7/6K1 b - - 0 1")) {
            ManualTests::expect(false, "every evaluation feature included once fen loading");
            return;
        }

        Evaluation evaluation;
        const int phase = GamePhase::calculateGamePhase(board);

        const int material = evaluation.evaluateMaterial(board);
        const int pst = evaluation.evaluatePieceSquareTables(board);
        const int bishopPair = PieceBonuses::evaluateBishopPair(board);
        const int rookFile = PieceBonuses::evaluateRookFile(board, 51, whiteRook);
        const int rookSeventh = PieceBonuses::evaluateRookSeventhRank(51, whiteRook);
        const int doubledPawns = PawnStructure::evaluateDoubledPawns(board);
        const int isolatedPawns = PawnStructure::evaluateIsolatedPawns(board);
        const int passedPawns = PawnStructure::evaluatePassedPawns(board);
        const int kingSafety = KingSafety::evaluateKingSafety(board, phase);
        const int mobility = Mobility::evaluateMobility(board);

        const int expectedTotal =
                material +
                pst +
                bishopPair +
                rookFile +
                rookSeventh +
                doubledPawns +
                isolatedPawns +
                passedPawns +
                kingSafety +
                mobility;

        const int actualTotal = evaluation.evaluate(board);

        ManualTests::expect(
            material != 0 &&
            pst != 0 &&
            bishopPair != 0 &&
            rookFile != 0 &&
            rookSeventh != 0 &&
            doubledPawns != 0 &&
            isolatedPawns != 0 &&
            passedPawns != 0 &&
            mobility != 0 &&
            actualTotal == -expectedTotal,
            "every evaluation feature is included in evaluate() exactly once"
        );
    }

    void runEvaluationIntegrationTests() {
        testKingsOnlyLegalPositionEvaluatesConsistently();
        testStartingPositionEvaluatesConsistently();
        testAddingWhiteQueenIncreasesFullEvaluation();
        testAddingBlackQueenDecreasesFullEvaluation();
        testMirroredColorsProduceOppositeWhitePerspectiveScore();
        testSwitchingSideToMoveNegatesFullEvaluation();
        testRepeatedEvaluationIsDeterministic();
        testEvaluationDoesNotMutateFEN();
        testEvaluationDoesNotChangeSideToMove();
        testEvaluationDoesNotChangeCastlingRights();
        testEvaluationDoesNotChangeEnPassantTarget();
        testEvaluationDoesNotChangeMoveCounters();
        testEvaluationDoesNotChangeUndoRedoAvailability();
        testMaterialIsNotIncludedInEvaluatePieceSquareTables();
        testPSTIsNotIncludedInEvaluateMaterial();
        testEveryEvaluationFeatureIsIncludedExactlyOnce();
    }
} // namespace EvaluationTests
