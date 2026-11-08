//
// Created by devrupamsahoo on 05-11-2026.
//


#include "../include/TestPerft.h"

#include "ManualTestUtils.h"
#include "chess/core/GameSession.h"

#include <cstdint>

namespace MoveGenerationTests {
    std::uint64_t perft(GameSession session, int depth) {
        if (depth == 0) {
            return 1;
        }

        std::uint64_t positionCount = 0;

        for (const PseudoLegalMove &move: session.legalMoves()) {
            GameSession nextSession = session;

            if (!nextSession.makeMove(
                move.startSquare,
                move.endSquare,
                move.promotion)) {
                return 0;
            }

            positionCount += perft(nextSession, depth - 1);
        }

        return positionCount;
    }

    struct PerftCase {
        const char *name;
        const char *fen;
        int depth;
        std::uint64_t expectedPositions;
    };

    bool testPerftCase(const PerftCase &testCase) {
        GameSession session;

        if (!session.loadFen(testCase.fen)) {
            return false;
        }

        return perft(session, testCase.depth) ==
               testCase.expectedPositions;
    }

    void runPerftTests() {
        constexpr PerftCase perftCases[] = {
            {
                "starting position depth one is 20",
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                1,
                20
            },
            {
                "starting position depth two is 400",
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                2,
                400
            },
            {
                "starting position depth three is 8902",
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                3,
                8902
            },
            {
                "kiwipete depth one is 48",
                "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
                1,
                48
            },
            {
                "kiwipete depth two is 2039",
                "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
                2,
                2039
            },
            {
                "kiwipete depth three is 97862",
                "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
                3,
                97862
            },
            {
                "endgame position depth one is 14",
                "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
                1,
                14
            },
            {
                "endgame position depth two is 191",
                "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
                2,
                191
            },
            {
                "endgame position depth three is 2812",
                "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
                3,
                2812
            },
            {
                "promotion position depth one is 6",
                "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
                1,
                6
            },
            {
                "promotion position depth two is 264",
                "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
                2,
                264
            },
            {
                "promotion position depth three is 9467",
                "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
                3,
                9467
            },
            {
                "advanced pawn position depth one is 44",
                "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
                1,
                44
            },
            {
                "advanced pawn position depth two is 1486",
                "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
                2,
                1486
            },
            {
                "advanced pawn position depth three is 62379",
                "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
                3,
                62379
            },
            {
                "tactical position depth one is 46",
                "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
                1,
                46
            },
            {
                "tactical position depth two is 2079",
                "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
                2,
                2079
            },
            {
                "tactical position depth three is 89890",
                "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
                3,
                89890
            }
        };

        for (const PerftCase &testCase: perftCases) {
            ManualTests::expect(
                testPerftCase(testCase),
                testCase.name);
        }
    }

    void runExtendedPerftTests() {
        constexpr PerftCase extendedCases[] = {
            {
                "starting position depth four is 197281",
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                4,
                197281
            },
            {
                "kiwipete depth four is 4085603",
                "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
                4,
                4085603
            },
            {
                "endgame position depth four is 43238",
                "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
                4,
                43238
            },
            {
                "promotion position depth four is 422333",
                "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
                4,
                422333
            },
            {
                "advanced pawn position depth four is 2103487",
                "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
                4,
                2103487
            },
            {
                "tactical position depth four is 3894594",
                "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
                4,
                3894594
            }
        };

        for (const PerftCase &testCase: extendedCases) {
            ManualTests::expect(
                testPerftCase(testCase),
                testCase.name);
        }
    }
}
