//
// Created by devrupamsahoo on 10-11-2026.
//

#include "TestEvaluation.h"

#include "ManualTestUtils.h"

#include "chess/core/ChessBoard.h"
#include "chess/evaluation/Evaluation.h"
#include "chess/evaluation/GamePhase.h"
#include "chess/evaluation/KingSafety.h"
#include "chess/evaluation/PawnStructure.h"
#include "chess/evaluation/PieceBonuses.h"
#include "chess/notation/FEN.h"

namespace {
    bool loadPosition(ChessBoard &board, const std::string &fen) {
        return FEN::load(board, fen);
    }
}

namespace EvaluationTests {
    // verifies that adding a queen creates a large positive material change
    void testMaterialAdvantage() {
        ChessBoard withoutQueen;
        ChessBoard withQueen;

        const bool loadedWithoutQueen =
                loadPosition(
                    withoutQueen,
                    "4k3/8/8/8/8/8/8/4K3 w - - 0 1"
                );

        const bool loadedWithQueen =
                loadPosition(
                    withQueen,
                    "4k3/8/8/8/8/8/8/3QK3 w - - 0 1"
                );

        ManualTests::expect(
            loadedWithoutQueen && loadedWithQueen,
            "evaluation material positions load"
        );

        Evaluation evaluation;

        const int withoutQueenScore =
                evaluation.evaluate(withoutQueen);

        const int withQueenScore =
                evaluation.evaluate(withQueen);

        ManualTests::expect(
            withQueenScore > withoutQueenScore + 700,
            "evaluation rewards a queen advantage"
        );
    }

    // verifies that the evaluation changes sign for the side to move
    void testSideToMovePerspective() {
        ChessBoard whiteToMove;
        ChessBoard blackToMove;

        const bool loadedWhite =
                loadPosition(
                    whiteToMove,
                    "4k3/8/8/8/8/8/8/3QK3 w - - 0 1"
                );

        const bool loadedBlack =
                loadPosition(
                    blackToMove,
                    "4k3/8/8/8/8/8/8/3QK3 b - - 0 1"
                );

        ManualTests::expect(
            loadedWhite && loadedBlack,
            "evaluation side-to-move positions load"
        );

        Evaluation evaluation;

        const int whiteScore =
                evaluation.evaluate(whiteToMove);

        const int blackScore =
                evaluation.evaluate(blackToMove);

        ManualTests::expect(
            whiteScore == -blackScore,
            "evaluation is relative to the side to move"
        );
    }

    // verifies that having two bishops receives the bishop-pair bonus
    void testBishopPairBonus() {
        ChessBoard oneBishop;
        ChessBoard twoBishops;

        const bool loadedOneBishop =
                loadPosition(
                    oneBishop,
                    "4k3/8/8/8/8/8/8/2B1K3 w - - 0 1"
                );

        const bool loadedTwoBishops =
                loadPosition(
                    twoBishops,
                    "4k3/8/8/8/8/8/8/1BB1K3 w - - 0 1"
                );

        ManualTests::expect(
            loadedOneBishop && loadedTwoBishops,
            "bishop-pair positions load"
        );

        const int oneBishopScore =
                PieceBonuses::evaluateBishopPair(oneBishop);

        const int twoBishopScore =
                PieceBonuses::evaluateBishopPair(twoBishops);

        ManualTests::expect(
            twoBishopScore > oneBishopScore,
            "evaluation rewards the bishop pair"
        );
    }

    // verifies that a rook on an open file receives a bonus
    void testRookOpenFileBonus() {
        ChessBoard board;

        const bool loaded =
                loadPosition(
                    board,
                    "4k3/8/8/8/8/8/8/R3K3 w - - 0 1"
                );

        ManualTests::expect(
            loaded,
            "rook open-file position loads"
        );

        const int score =
                PieceBonuses::evaluateRookFile(board, a1, whiteRook);

        ManualTests::expect(
            score == 20,
            "evaluation rewards a rook on an open file"
        );
    }

    // verifies doubled, isolated, and passed pawn behavior
    void testPawnStructureTerms() {
        ChessBoard singlePawn;
        ChessBoard doubledPawns;
        ChessBoard blockedPawn;

        const bool loadedSinglePawn =
                loadPosition(
                    singlePawn,
                    "4k3/8/8/8/4P3/8/8/4K3 w - - 0 1"
                );

        const bool loadedDoubledPawns =
                loadPosition(
                    doubledPawns,
                    "4k3/8/8/8/4P3/8/4P3/4K3 w - - 0 1"
                );

        const bool loadedBlockedPawn =
                loadPosition(
                    blockedPawn,
                    "4k3/4p3/4P3/8/8/8/8/4K3 w - - 0 1"
                );

        ManualTests::expect(
            loadedSinglePawn &&
            loadedDoubledPawns &&
            loadedBlockedPawn,
            "pawn-structure positions load"
        );

        const int singlePawnScore =
                PawnStructure::evaluateDoubledPawns(singlePawn);

        const int doubledPawnScore =
                PawnStructure::evaluateDoubledPawns(doubledPawns);

        ManualTests::expect(
            singlePawnScore > doubledPawnScore,
            "evaluation penalizes doubled pawns"
        );

        const int passedPawnScore =
                PawnStructure::evaluatePassedPawns(singlePawn);

        const int blockedPawnScore =
                PawnStructure::evaluatePassedPawns(blockedPawn);

        ManualTests::expect(
            passedPawnScore > blockedPawnScore,
            "evaluation rewards passed pawns"
        );
    }

    // verifies that a pawn shield improves king safety
    void testKingSafetyTerms() {
        ChessBoard withShield;
        ChessBoard withoutShield;

        const bool loadedWithShield =
                loadPosition(
                    withShield,
                    "q3k3/8/8/8/3PPP2/8/8/Q3K3 w - - 0 1"
                );

        const bool loadedWithoutShield =
                loadPosition(
                    withoutShield,
                    "q3k3/8/8/8/8/8/8/Q3K3 w - - 0 1"
                );

        ManualTests::expect(
            loadedWithShield && loadedWithoutShield,
            "king-safety positions load"
        );

        const int shieldPhase =
                GamePhase::calculateGamePhase(withShield);

        const int noShieldPhase =
                GamePhase::calculateGamePhase(withoutShield);

        const int shieldScore =
                KingSafety::evaluateKingSafety(
                    withShield,
                    shieldPhase
                );

        const int noShieldScore =
                KingSafety::evaluateKingSafety(
                    withoutShield,
                    noShieldPhase
                );

        ManualTests::expect(
            shieldScore > noShieldScore,
            "evaluation rewards a king pawn shield"
        );
    }
} // namespace EvaluationTests
