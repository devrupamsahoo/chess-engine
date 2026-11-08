//
// Created by devrupamsahoo on 07-11-2026.
//

#include "TestGameStatus.h"
#include "chess/core/GameSession.h"
#include "ManualTestUtils.h"

namespace ManualTests {
    void testGameStatus() {
        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "7k/6Q1/6K1/8/8/8/8/8 b - - 0 1"),
                "load checkmate position");

            ManualTests::expect(
                session.gameStatus() == GameStatus::checkmate,
                "detect checkmate");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "7k/5K2/6Q1/8/8/8/8/8 b - - 0 1"),
                "load stalemate position");

            ManualTests::expect(
                session.gameStatus() == GameStatus::stalemate,
                "detect stalemate");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "4r1k1/8/8/8/8/8/8/4K3 w - - 0 1"),
                "load check position");

            ManualTests::expect(
                session.gameStatus() == GameStatus::check,
                "detect check");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.gameStatus() == GameStatus::ongoing,
                "starting position is ongoing");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "4k3/8/8/8/8/8/8/4K3 w - - 0 1"),
                "load kings-only position");

            ManualTests::expect(
                session.gameStatus() == GameStatus::draw,
                "detect draw with kings only");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "4k3/8/8/8/8/8/8/1N2K3 w - - 0 1"),
                "load king and knight versus king position");

            ManualTests::expect(
                session.gameStatus() == GameStatus::draw,
                "detect draw with king and knight versus king");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "4k3/8/8/8/8/8/8/2B1K3 w - - 0 1"),
                "load king and bishop versus king position");

            ManualTests::expect(
                session.gameStatus() == GameStatus::draw,
                "detect draw with king and bishop versus king");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "4kb2/8/8/8/8/8/8/2B1K3 w - - 0 1"),
                "load same-colour bishops position");

            ManualTests::expect(
                session.gameStatus() == GameStatus::draw,
                "detect draw with bishops on the same colour");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "2b1k3/8/8/8/8/8/8/2B1K3 w - - 0 1"),
                "load opposite-colour bishops position");

            ManualTests::expect(
                session.gameStatus() == GameStatus::ongoing,
                "opposite-colour bishops are not an automatic draw");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "4k3/8/8/8/8/8/8/R3K3 w - - 100 1"),
                "load fifty-move-rule position");

            ManualTests::expect(
                session.gameStatus() == GameStatus::draw,
                "detect draw after one hundred halfmoves");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.loadFen(
                    "7k/6Q1/6K1/8/8/8/8/8 b - - 100 1"),
                "load checkmate position with one hundred halfmoves");

            ManualTests::expect(
                session.gameStatus() == GameStatus::checkmate,
                "checkmate takes priority over fifty-move draw");
        }

        {
            GameSession session;

            ManualTests::expect(
                session.makeMove(f2, f3),
                "white plays f3");

            ManualTests::expect(
                session.makeMove(e7, e5),
                "black plays e5");

            ManualTests::expect(
                session.makeMove(g2, g4),
                "white plays g4");

            ManualTests::expect(
                session.makeMove(d8, h4),
                "black delivers fool's mate");

            ManualTests::expect(
                session.gameStatus() == GameStatus::checkmate,
                "detect checkmate after a played move");

            ManualTests::expect(
                session.undo(),
                "undo checkmating move");

            ManualTests::expect(
                session.gameStatus() == GameStatus::ongoing,
                "undo restores ongoing game status");

            ManualTests::expect(
                session.redo(),
                "redo checkmating move");

            ManualTests::expect(
                session.gameStatus() == GameStatus::checkmate,
                "redo restores checkmate status");
        }
    }

    void testThreefoldRepetition() {
        GameSession session;

        ManualTests::expect(
            session.makeMove(g1, f3),
            "white knight moves out");

        ManualTests::expect(
            session.makeMove(g8, f6),
            "black knight moves out");

        ManualTests::expect(
            session.makeMove(f3, g1),
            "white knight returns");

        ManualTests::expect(
            session.makeMove(f6, g8),
            "black knight returns");

        ManualTests::expect(
            session.makeMove(g1, f3),
            "white repeats knight move");

        ManualTests::expect(
            session.makeMove(g8, f6),
            "black repeats knight move");

        ManualTests::expect(
            session.makeMove(f3, g1),
            "white repeats knight return");

        ManualTests::expect(
            session.makeMove(f6, g8),
            "black repeats knight return");

        ManualTests::expect(
            session.gameStatus() == GameStatus::draw,
            "detect threefold repetition");
    }
}
