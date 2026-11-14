//
// Created by devrupamsahoo on 10-11-2026.
//

#include "TestKingSafety.h"

#include <cstdlib>
#include <string>

#include "ManualTestUtils.h"

#include "chess/core/ChessBoard.h"
#include "chess/core/Enums.h"
#include "chess/evaluation/GamePhase.h"
#include "chess/evaluation/KingSafety.h"
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

        // checks that color-mirrored positions produce opposite king safety scores
        void expectComponentSymmetry(
            const std::string &fenA,
            const std::string &fenB,
            const std::string &testName
        ) {
            ChessBoard boardA;
            ChessBoard boardB;
            if (!loadPosition(boardA, fenA) || !loadPosition(boardB, fenB)) {
                ManualTests::expect(false, testName + " (fen load failed)");
                return;
            }

            const int phaseA = GamePhase::calculateGamePhase(boardA);
            const int phaseB = GamePhase::calculateGamePhase(boardB);
            const int scoreA = KingSafety::evaluateKingSafety(boardA, phaseA);
            const int scoreB = KingSafety::evaluateKingSafety(boardB, phaseB);

            ManualTests::expect(
                phaseA == phaseB && scoreA == -scoreB && scoreA != 0,
                testName
            );
        }
    } // namespace

    void testWhitePawnShield() {
        // white king on g1 with intact f2/g2/h2 pawn shield is safer than with advanced/broken shield
        ChessBoard intactShield;
        ChessBoard brokenShield;
        if (!loadPosition(intactShield, "rnbq1rk1/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1RK1 w - - 0 1") ||
            !loadPosition(brokenShield, "rnbq1rk1/pppppppp/8/5PPP/8/8/PPPPP3/RNBQ1RK1 w - - 0 1")) {
            ManualTests::expect(false, "white pawn shield fen loading");
            return;
        }

        const int phase = GamePhase::calculateGamePhase(intactShield);
        const int intactScore = KingSafety::evaluateKingSafety(intactShield, phase);
        const int brokenScore = KingSafety::evaluateKingSafety(brokenShield, phase);

        ManualTests::expect(
            intactScore > brokenScore,
            "intact white pawn shield around g1 scores higher than broken shield"
        );
    }

    void testBlackPawnShield() {
        // black king on g8 with intact f7/g7/h7 pawn shield reduces white-perspective king safety score
        ChessBoard intactBlackShield;
        ChessBoard brokenBlackShield;
        if (!loadPosition(intactBlackShield, "rnbq1rk1/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1RK1 w - - 0 1") ||
            !loadPosition(brokenBlackShield, "rnbq1rk1/ppppp3/8/8/5ppp/8/PPPPPPPP/RNBQ1RK1 w - - 0 1")) {
            ManualTests::expect(false, "black pawn shield fen loading");
            return;
        }

        const int phase = GamePhase::calculateGamePhase(intactBlackShield);
        const int intactScore = KingSafety::evaluateKingSafety(intactBlackShield, phase);
        const int brokenScore = KingSafety::evaluateKingSafety(brokenBlackShield, phase);

        ManualTests::expect(
            intactScore < brokenScore,
            "intact black pawn shield around g8 lowers white-perspective king safety score"
        );
    }

    void testMissingWhitePawnShield() {
        // white king on g1 missing f2, g2, h2 pawns against black's intact shield yields negative score
        ChessBoard board;
        if (!loadPosition(board, "rnbq1rk1/pppppppp/8/8/8/8/PPPPP3/RNBQ1RK1 w - - 0 1")) {
            ManualTests::expect(false, "missing white pawn shield fen loading");
            return;
        }

        const int phase = GamePhase::calculateGamePhase(board);
        ManualTests::expect(
            KingSafety::evaluateKingSafety(board, phase) < 0,
            "missing white pawn shield produces negative king safety score"
        );
    }

    void testMissingBlackPawnShield() {
        // black king on g8 missing f7, g7, h7 pawns against white's intact shield yields positive score
        ChessBoard board;
        if (!loadPosition(board, "rnbq1rk1/ppppp3/8/8/8/8/PPPPPPPP/RNBQ1RK1 w - - 0 1")) {
            ManualTests::expect(false, "missing black pawn shield fen loading");
            return;
        }

        const int phase = GamePhase::calculateGamePhase(board);
        ManualTests::expect(
            KingSafety::evaluateKingSafety(board, phase) > 0,
            "missing black pawn shield produces positive king safety score"
        );
    }

    void testWhiteKingNearOpenFile() {
        // white king on g1 when g-file is completely open is penalized more than when g-file is closed
        ChessBoard closedFile;
        ChessBoard openFile;
        if (!loadPosition(closedFile, "k6r/pppppppp/8/8/8/8/PPPPPPPP/R5K1 w - - 0 1") ||
            !loadPosition(openFile, "k6r/pppppp1p/7p/8/8/8/PPPPPP1P/R5K1 w - - 0 1")) {
            ManualTests::expect(false, "white king near open file fen loading");
            return;
        }

        const int phase = GamePhase::calculateGamePhase(openFile);
        ManualTests::expect(
            KingSafety::evaluateKingSafety(openFile, phase) < KingSafety::evaluateKingSafety(closedFile, phase),
            "open g-file in front of white king on g1 lowers white king safety"
        );
    }

    void testBlackKingNearOpenFile() {
        // black king on g8 when g-file is completely open increases white-perspective safety score
        ChessBoard closedFile;
        ChessBoard openFile;
        if (!loadPosition(closedFile, "rnbq1rk1/pppppppp/8/8/8/8/PPPPPPPP/K7 w - - 0 1") ||
            !loadPosition(openFile, "rnbq1rk1/pppppp1p/8/8/8/8/PPPPPPPP/K7 w - - 0 1")) {
            ManualTests::expect(false, "black king near open file fen loading");
            return;
        }

        const int phase = GamePhase::calculateGamePhase(openFile);
        ManualTests::expect(
            KingSafety::evaluateKingSafety(openFile, phase) > KingSafety::evaluateKingSafety(closedFile, phase),
            "open g-file in front of black king on g8 lowers black king safety"
        );
    }

    void testWhiteKingNearSemiOpenFile() {
        // white king on g1 missing its g2 pawn (semi-open g-file) has lower safety than with g2 present
        ChessBoard closedFile;
        ChessBoard semiOpenFile;
        if (!loadPosition(closedFile, "rnbq1rk1/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1RK1 w - - 0 1") ||
            !loadPosition(semiOpenFile, "rnbq1rk1/pppppppp/8/8/8/8/PPPPPP1P/RNBQ1RK1 w - - 0 1")) {
            ManualTests::expect(false, "white king near semi-open file fen loading");
            return;
        }

        const int phase = GamePhase::calculateGamePhase(semiOpenFile);
        ManualTests::expect(
            KingSafety::evaluateKingSafety(semiOpenFile, phase) < KingSafety::evaluateKingSafety(closedFile, phase),
            "semi-open file near white king reduces white king safety"
        );
    }

    void testBlackKingNearSemiOpenFile() {
        // black king on g8 missing its g7 pawn (semi-open g-file) increases white-perspective score
        ChessBoard closedFile;
        ChessBoard semiOpenFile;
        if (!loadPosition(closedFile, "rnbq1rk1/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1RK1 w - - 0 1") ||
            !loadPosition(semiOpenFile, "rnbq1rk1/pppppp1p/8/8/8/8/PPPPPPPP/RNBQ1RK1 w - - 0 1")) {
            ManualTests::expect(false, "black king near semi-open file fen loading");
            return;
        }

        const int phase = GamePhase::calculateGamePhase(semiOpenFile);
        ManualTests::expect(
            KingSafety::evaluateKingSafety(semiOpenFile, phase) > KingSafety::evaluateKingSafety(closedFile, phase),
            "semi-open file near black king reduces black king safety"
        );
    }

    void testKingZonePressure() {
        // black queen, rook, bishop, and knight attacking white's king zone around g1 reduce white king safety
        ChessBoard calmBoard;
        ChessBoard pressuredBoard;
        if (!loadPosition(calmBoard, "rnbq1rk1/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1RK1 w - - 0 1") ||
            !loadPosition(pressuredBoard, "r1b2rk1/pppppppp/8/8/8/5nq1/PPPPPPPP/RNBQ1RK1 w - - 0 1")) {
            ManualTests::expect(false, "king-zone pressure fen loading");
            return;
        }

        const int phase = GamePhase::calculateGamePhase(pressuredBoard);
        ManualTests::expect(
            KingSafety::evaluateKingSafety(pressuredBoard, phase) < KingSafety::evaluateKingSafety(calmBoard, phase),
            "enemy attackers pressuring white king zone lower king safety score"
        );
    }

    void testMirroredKingSafetySymmetry() {
        // mirroring an asymmetric king safety position flips the sign of evaluatekingsafety
        expectComponentSymmetry(
            "rnbq1rk1/ppppp3/8/8/8/5N1Q/PPPPPPPP/RNB2RK1 w - - 0 1",
            "rnb2rk1/pppppppp/5n1q/8/8/8/PPPPP3/RNBQ1RK1 w - - 0 1",
            "mirrored king safety positions produce exact opposite scores"
        );
    }

    void testKingSafetyPhaseScaling() {
        // king safety magnitude scales proportionally with gamephase
        ChessBoard board;
        if (!loadPosition(board, "rnbq1rk1/ppppp3/8/8/8/5N1Q/PPPPPPPP/RNB2RK1 w - - 0 1")) {
            ManualTests::expect(false, "king safety phase scaling fen loading");
            return;
        }

        const int scorePhase24 = KingSafety::evaluateKingSafety(board, 24);
        const int scorePhase12 = KingSafety::evaluateKingSafety(board, 12);
        const int scorePhase6 = KingSafety::evaluateKingSafety(board, 6);

        ManualTests::expect(
            scorePhase24 > scorePhase12 && scorePhase12 > scorePhase6 && scorePhase6 > 0,
            "king safety score scales monotonically with game phase"
        );
    }

    void testKingSafetyZeroAtPhaseZero() {
        // at phase 0, phase-scaled king safety evaluates to 0 even with asymmetric shields and attackers
        ChessBoard board;
        if (!loadPosition(board, "rnbq1rk1/ppppp3/8/8/8/5N1Q/PPPPPPPP/RNB2RK1 w - - 0 1")) {
            ManualTests::expect(false, "king safety zero at phase 0 fen loading");
            return;
        }

        ManualTests::expect(
            KingSafety::evaluateKingSafety(board, 0) == 0,
            "king safety evaluates to zero at game phase 0"
        );
    }

    void testKingSafetyActiveAtPhaseTwentyFour() {
        // at phase 24, asymmetric king safety position produces a nonzero evaluation
        ChessBoard board;
        if (!loadPosition(board, "rnbq1rk1/ppppp3/8/8/8/5N1Q/PPPPPPPP/RNB2RK1 w - - 0 1")) {
            ManualTests::expect(false, "king safety active at phase 24 fen loading");
            return;
        }

        ManualTests::expect(
            KingSafety::evaluateKingSafety(board, 24) > 0,
            "king safety is active and nonzero at game phase 24"
        );
    }

    void testFriendlyPiecesAreNotEnemyAttackers() {
        // white queen and knight defending white's own king zone do not penalize white like black attackers do
        ChessBoard friendlyDefenders;
        ChessBoard enemyAttackers;
        if (!loadPosition(friendlyDefenders, "r1b2rk1/pppppppp/8/8/8/5NQ1/PPPPPPPP/RNB2RK1 w - - 0 1") ||
            !loadPosition(enemyAttackers, "r1b2rk1/pppppppp/8/8/8/5nq1/PPPPPPPP/RNBQ1RK1 w - - 0 1")) {
            ManualTests::expect(false, "friendly pieces not enemy attackers fen loading");
            return;
        }

        const int friendlyScore = KingSafety::evaluateKingSafety(friendlyDefenders, 24);
        const int enemyScore = KingSafety::evaluateKingSafety(enemyAttackers, 24);

        ManualTests::expect(
            friendlyScore >= 0 && friendlyScore > enemyScore,
            "friendly pieces near own king are not counted as enemy king attackers"
        );
    }

    void testBlockedSlidingAttackersDoNotCreatePressure() {
        // black queen and rook on g-file and h-file blocked by pawns outside the king zone create less pressure than unblocked
        ChessBoard blockedSliders;
        ChessBoard unblockedSliders;
        if (!loadPosition(blockedSliders, "4rrk1/pppppp1p/6q1/6p1/8/8/PPPPPPPP/RNBQ1RK1 w - - 0 1") ||
    !loadPosition(unblockedSliders, "4rrk1/pppppppp/8/8/8/6q1/PPPPPPPP/RNBQ1RK1 w - - 0 1")) {
            ManualTests::expect(false, "blocked sliding attackers fen loading");
            return;
        }

        const int blockedScore = KingSafety::evaluateKingSafety(blockedSliders, 24);
        const int unblockedScore = KingSafety::evaluateKingSafety(unblockedSliders, 24);

        ManualTests::expect(
            blockedScore > unblockedScore,
            "blocked sliding attackers do not create king-zone attack pressure"
        );
    }

    void runKingSafetyTests() {
        testWhitePawnShield();
        testBlackPawnShield();
        testMissingWhitePawnShield();
        testMissingBlackPawnShield();
        testWhiteKingNearOpenFile();
        testBlackKingNearOpenFile();
        testWhiteKingNearSemiOpenFile();
        testBlackKingNearSemiOpenFile();
        testKingZonePressure();
        testMirroredKingSafetySymmetry();
        testKingSafetyPhaseScaling();
        testKingSafetyZeroAtPhaseZero();
        testKingSafetyActiveAtPhaseTwentyFour();
        testFriendlyPiecesAreNotEnemyAttackers();
        testBlockedSlidingAttackersDoNotCreatePressure();
    }
} // namespace EvaluationTests
