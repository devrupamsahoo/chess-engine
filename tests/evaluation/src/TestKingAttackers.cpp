//
// Created by devrupamsahoo on 10-11-2026.
//

#include "TestKingAttackers.h"
#include <string>
#include "ManualTestUtils.h"
#include "chess/core/ChessBoard.h"
#include "chess/core/Enums.h"
#include "chess/evaluation/EvaluationConstants.h"
#include "chess/evaluation/KingAttackers.h"
#include "chess/notation/FEN.h"
#include <algorithm>

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

    void testOneAttackingPawn() {
        // white pawn on f6 attacks g7 (square 54) in black king's zone around g8 (square 62)
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/5P2/8/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "one attacking pawn fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 54, true, whitePawn) == 1,
            "countAttackers detects one attacking white pawn on g7"
        );
        ManualTests::expect(
            KingAttackers::evaluateUniqueKingAttackers(board, 62, true) > 0,
            "one attacking pawn in king zone produces positive danger score"
        );
    }

    void testOneAttackingKnight() {
        // white knight on f5 attacks g7 (square 54) and h6 (square 47) near black king on g8 (square 62)
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/5N2/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "one attacking knight fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 54, true, whiteKnight) == 1,
            "countAttackers detects one attacking white knight on g7"
        );
        ManualTests::expect(
            KingAttackers::evaluateUniqueKingAttackers(board, 62, true) > 0,
            "one attacking knight in king zone produces positive danger score"
        );
    }

    void testOneAttackingBishop() {
        // white bishop on c3 attacks g7 (square 54) in black king's zone around g8 (square 62)
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/8/8/2B5/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "one attacking bishop fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 54, true, whiteBishop) == 1,
            "countAttackers detects one attacking white bishop on g7"
        );
        ManualTests::expect(
            KingAttackers::evaluateUniqueKingAttackers(board, 62, true) > 0,
            "one attacking bishop in king zone produces positive danger score"
        );
    }

    void testOneAttackingRook() {
        // white rook on g1 attacks g7 (square 54) in black king's zone around g8 (square 62)
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/8/8/8/8/4K1R1 b - - 0 1")) {
            ManualTests::expect(false, "one attacking rook fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 54, true, whiteRook) == 1,
            "countAttackers detects one attacking white rook on g7"
        );
        ManualTests::expect(
            KingAttackers::evaluateUniqueKingAttackers(board, 62, true) > 0,
            "one attacking rook in king zone produces positive danger score"
        );
    }

    void testOneAttackingQueen() {
        // white queen on g2 attacks g7 (square 54) in black king's zone around g8 (square 62)
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/8/8/8/6Q1/4K3 b - - 0 1")) {
            ManualTests::expect(false, "one attacking queen fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 54, true, whiteQueen) == 1,
            "countAttackers detects one attacking white queen on g7"
        );
        ManualTests::expect(
            KingAttackers::evaluateUniqueKingAttackers(board, 62, true) > 0,
            "one attacking queen in king zone produces positive danger score"
        );
    }

    void testDifferentAttackerTypes() {
        // combining a white queen and a white knight attacking black's king zone increases danger over either alone
        ChessBoard knightOnly;
        ChessBoard queenOnly;
        ChessBoard combined;
        if (!loadPosition(knightOnly, "6k1/8/8/5N2/8/8/8/4K3 b - - 0 1") ||
            !loadPosition(queenOnly, "6k1/8/8/8/8/8/6Q1/4K3 b - - 0 1") ||
            !loadPosition(combined, "6k1/8/8/5N2/8/8/6Q1/4K3 b - - 0 1")) {
            ManualTests::expect(false, "different attacker types fen loading");
            return;
        }

        const int knightDanger = KingAttackers::evaluateUniqueKingAttackers(knightOnly, 62, true);
        const int queenDanger = KingAttackers::evaluateUniqueKingAttackers(queenOnly, 62, true);
        const int combinedDanger = KingAttackers::evaluateUniqueKingAttackers(combined, 62, true);

        ManualTests::expect(
            combinedDanger > queenDanger && combinedDanger > knightDanger,
            "combining different attacker types increases king danger score"
        );
    }

    void testMultipleAttackersOfSameType() {
        // two white knights attacking g7 (square 54) from f5 and e6 are both counted
        ChessBoard oneKnight;
        ChessBoard twoKnights;
        if (!loadPosition(oneKnight, "6k1/8/8/5N2/8/8/8/4K3 w - - 0 1") ||
            !loadPosition(twoKnights, "6k1/8/4N3/5N2/8/8/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "multiple attackers of same type fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(twoKnights, 54, true, whiteKnight) == 2,
            "countAttackers counts two knights attacking the same square"
        );
        ManualTests::expect(
            KingAttackers::evaluateUniqueKingAttackers(twoKnights, 62, true) >
            KingAttackers::evaluateUniqueKingAttackers(oneKnight, 62, true),
            "two attacking knights produce higher king danger than one attacking knight"
        );
    }

    void testSameAttackerAttackingMultipleKingZoneSquaresCountsOnce() {
        // black king on h8 (square 63) has king zone {g8, h8, g7, h7}
        // white queen on e5 attacks only g7 in that zone, whereas white queen on f6 attacks g7 and h8
        ChessBoard singleSquareQueen;
        ChessBoard multiSquareQueen;
        if (!loadPosition(singleSquareQueen, "7k/8/8/4Q3/8/8/8/4K3 b - - 0 1") ||
            !loadPosition(multiSquareQueen, "7k/8/5Q2/8/8/8/8/4K3 b - - 0 1")) {
            ManualTests::expect(false, "same attacker multiple king-zone squares fen loading");
            return;
        }

        const int dangerSingleSquare = KingAttackers::evaluateUniqueKingAttackers(singleSquareQueen, 63, true);
        const int dangerMultiSquare = KingAttackers::evaluateUniqueKingAttackers(multiSquareQueen, 63, true);

        ManualTests::expect(
            dangerSingleSquare == dangerMultiSquare && dangerSingleSquare > 0,
            "same queen attacking multiple king-zone squares counts only once"
        );
    }

    void testBlockedBishop() {
        // white bishop on c3 blocked by white pawn on e5 does not attack black king zone around g8
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/4P3/8/2B5/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "blocked bishop fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 54, true, whiteBishop) == 0 &&
            KingAttackers::evaluateUniqueKingAttackers(board, 62, true) == 0,
            "blocked bishop does not attack king zone"
        );
    }

    void testBlockedRook() {
        // white rook on g1 blocked by white pawn on g4 does not attack black king zone around g8
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/8/6P1/8/8/4K1R1 w - - 0 1")) {
            ManualTests::expect(false, "blocked rook fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 54, true, whiteRook) == 0 &&
            KingAttackers::evaluateUniqueKingAttackers(board, 62, true) == 0,
            "blocked rook does not attack king zone"
        );
    }

    void testBlockedQueen() {
        // white queen on c3 blocked along diagonal and files by pawns on c4, d4, d3 does not attack king zone around g8
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/8/2PP4/2QP4/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "blocked queen fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 54, true, whiteQueen) == 0 &&
            KingAttackers::evaluateUniqueKingAttackers(board, 62, true) == 0,
            "blocked queen does not attack king zone"
        );
    }

    void testAttackerOutsideKingZone() {
        // white rook on a1 and knight on b2 do not attack black king zone around h8 (square 63)
        ChessBoard board;
        if (!loadPosition(board, "7k/8/8/8/8/8/1N6/R3K3 w - - 0 1")) {
            ManualTests::expect(false, "attacker outside king zone fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::evaluateUniqueKingAttackers(board, 63, true) == 0,
            "pieces attacking only squares outside the king zone produce zero danger"
        );
    }

    void testStrongerAttackerIncreasesPressure() {
        // replacing a pawn attacker with a queen attacker alongside a knight increases danger score
        ChessBoard pawnAndKnight;
        ChessBoard queenAndKnight;
        if (!loadPosition(pawnAndKnight, "6k1/8/5P2/5N2/8/8/8/4K3 b - - 0 1") ||
            !loadPosition(queenAndKnight, "6k1/8/8/5N2/8/8/6Q1/4K3 b - - 0 1")) {
            ManualTests::expect(false, "stronger attacker increases pressure fen loading");
            return;
        }

        const int weakerDanger = KingAttackers::evaluateUniqueKingAttackers(pawnAndKnight, 62, true);
        const int strongerDanger = KingAttackers::evaluateUniqueKingAttackers(queenAndKnight, 62, true);

        ManualTests::expect(
            strongerDanger > weakerDanger,
            "stronger queen attacker produces greater king danger than pawn attacker"
        );
    }

    void testAttackerCountBonusCap() {
        // six or more white pieces attacking black king zone around g8 stay bounded by king_danger_table[20]
        ChessBoard manyAttackers;
        if (!loadPosition(manyAttackers, "6k1/8/4NPP1/5N2/8/2B3R1/6Q1/4K1R1 w - - 0 1")) {
            ManualTests::expect(false, "attacker-count bonus cap fen loading");
            return;
        }

        const int danger = KingAttackers::evaluateUniqueKingAttackers(manyAttackers, 62, true);
        ManualTests::expect(
            danger > 0 && danger <= EvalConstants::KING_DANGER_TABLE[20],
            "attacker count bonus and danger index clamp safely within KING_DANGER_TABLE bounds"
        );
    }

    void testWhiteBlackKingAttackersSymmetry() {
        // mirrored white attack on black king (g8 = 62) equals black attack on white king (g1 = 6)
        ChessBoard whiteAttacking;
        ChessBoard blackAttacking;
        if (!loadPosition(whiteAttacking, "6k1/8/8/5N2/8/2B5/6Q1/4K1R1 b - - 0 1") ||
    !loadPosition(blackAttacking, "4k1r1/6q1/2b5/8/5n2/8/8/6K1 w - - 0 1")) {
            ManualTests::expect(false, "white/black king attackers symmetry fen loading");
            return;
        }

        const int whiteDanger = KingAttackers::evaluateUniqueKingAttackers(whiteAttacking, 62, true);
        const int blackDanger = KingAttackers::evaluateUniqueKingAttackers(blackAttacking, 6, false);

        ManualTests::expect(
            whiteDanger == blackDanger && whiteDanger > 0,
            "white and black king attacker evaluations are symmetric under board mirroring"
        );
    }

    void testInvalidKingSquareNegativeOne() {
        // invalid king square -1 returns 0 safely without out-of-bounds access
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/5N2/8/8/6Q1/4K3 b - - 0 1")) {
            ManualTests::expect(false, "invalid king square -1 fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::evaluateUniqueKingAttackers(board, -1, true) == 0 &&
            KingAttackers::evaluateUniqueKingAttackers(board, -1, false) == 0,
            "evaluateUniqueKingAttackers safely returns 0 for invalid king square -1"
        );
    }

    void testInvalidKingSquareGreaterOrEqualSixtyFour() {
        // invalid king squares >= 64 return 0 safely without out-of-bounds access
        ChessBoard board;
        if (!loadPosition(board, "6k1/8/8/5N2/8/8/6Q1/4K3 b - - 0 1")) {
            ManualTests::expect(false, "invalid king square >= 64 fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::evaluateUniqueKingAttackers(board, 64, true) == 0 &&
            KingAttackers::evaluateUniqueKingAttackers(board, 100, false) == 0,
            "evaluateUniqueKingAttackers safely returns 0 for invalid king squares >= 64"
        );
    }

    void testCountAttackersDoesNotCountWrongColorPieces() {
        // black knight on f3 attacks e5 (square 36), so counting white knights attacking e5 returns 0
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/8/5n2/8/4K3 w - - 0 1")) {
            ManualTests::expect(false, "countAttackers wrong-color pieces fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 36, true, whiteKnight) == 0 &&
            KingAttackers::countAttackers(board, 36, true, blackKnight) == 0,
            "countAttackers does not count pieces of the wrong color"
        );
    }

    void testCountAttackersDoesNotSeeThroughBlockers() {
        // white rook on e1 with blocker on e3 does not attack e5 (square 36)
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/8/8/4P3/8/4R1K1 w - - 0 1")) {
            ManualTests::expect(false, "countAttackers blocker fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 36, true, whiteRook) == 0,
            "countAttackers does not see through intervening blockers"
        );
    }

    void testOccupiedTargetCaptureSquareCountsAsAttacked() {
        // white rook on e1 attacks e5 (square 36) even when e5 is occupied by a black pawn
        ChessBoard board;
        if (!loadPosition(board, "4k3/8/8/4p3/8/8/8/4R1K1 w - - 0 1")) {
            ManualTests::expect(false, "occupied target capture square fen loading");
            return;
        }

        ManualTests::expect(
            KingAttackers::countAttackers(board, 36, true, whiteRook) == 1,
            "occupied target capture square is counted as attacked by sliding piece"
        );
    }

    void runKingAttackersTests() {
        testOneAttackingPawn();
        testOneAttackingKnight();
        testOneAttackingBishop();
        testOneAttackingRook();
        testOneAttackingQueen();
        testDifferentAttackerTypes();
        testMultipleAttackersOfSameType();
        testSameAttackerAttackingMultipleKingZoneSquaresCountsOnce();
        testBlockedBishop();
        testBlockedRook();
        testBlockedQueen();
        testAttackerOutsideKingZone();
        testStrongerAttackerIncreasesPressure();
        testAttackerCountBonusCap();
        testWhiteBlackKingAttackersSymmetry();
        testInvalidKingSquareNegativeOne();
        testInvalidKingSquareGreaterOrEqualSixtyFour();
        testCountAttackersDoesNotCountWrongColorPieces();
        testCountAttackersDoesNotSeeThroughBlockers();
        testOccupiedTargetCaptureSquareCountsAsAttacked();
    }
} // namespace EvaluationTests
