//
// Created by devrupamsahoo on 10-11-2026.
//

#include "chess/evaluation/Mobility.h"

#include "chess/evaluation/EvaluationConstants.h"
#include "chess/move/AttackValidator.h"


// knight mobility difference, positive favours white, not used by the combined mobility at the moment
int Mobility::evaluateKnightMobility(const ChessBoard &board) {
    uint64_t whiteOccupancy = 0;
    uint64_t blackOccupancy = 0;

    // Calculate occupancy for each side.
    for (int i = 0; i < 6; ++i) {
        whiteOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i)
        );

        blackOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i + 6)
        );
    }

    int whiteMobility = 0;
    int blackMobility = 0;

    // White knights.
    uint64_t whiteKnights = board.getPieceBitboard(whiteKnight);

    while (whiteKnights) {
        int square = __builtin_ctzll(whiteKnights);
        whiteKnights &= whiteKnights - 1;

        uint64_t attacks = AttackTables::KnightAttacks[square];

        // knights may move to empty or enemy squares, not onto own pieces
        uint64_t availableSquares = attacks & ~whiteOccupancy;

        whiteMobility += __builtin_popcountll(availableSquares);
    }

    // Black knights.
    uint64_t blackKnights = board.getPieceBitboard(blackKnight);

    while (blackKnights) {
        int square = __builtin_ctzll(blackKnights);
        blackKnights &= blackKnights - 1;

        uint64_t attacks = AttackTables::KnightAttacks[square];

        uint64_t availableSquares = attacks & ~blackOccupancy;

        blackMobility += __builtin_popcountll(availableSquares);
    }

    // weighted difference, positive favours white
    return (whiteMobility - blackMobility) * EvalConstants::KNIGHT_MOBILITY_WEIGHT;
}

// bishop mobility difference, positive favours white, not used by the combined mobility at the moment
int Mobility::evaluateBishopMobility(const ChessBoard &board) {
    uint64_t whiteOccupancy = 0;
    uint64_t blackOccupancy = 0;
    uint64_t occupancy = board.getAllOccupancy();

    // piece indices zero to five are white, six to eleven are black, build occupancy per colour
    for (int i = 0; i < 6; ++i) {
        whiteOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i)
        );

        blackOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i + 6)
        );
    }

    // the four diagonal directions
    const Direction diagonalDirections[4] = {
        upLeft, upRight, downLeft, downRight
    };

    int whiteMobility = 0;
    int blackMobility = 0;

    // counts reachable squares for every bishop of one colour
    auto calculateMobility = [&](Piece bishop, uint64_t friendlyOccupancy) {
        int mobility = 0;

        uint64_t bishops = board.getPieceBitboard(bishop);

        while (bishops) {
            int fromSquare = __builtin_ctzll(bishops);
            bishops &= bishops - 1;

            for (Direction direction : diagonalDirections) {
                const auto &ray =
                    AttackTables::rayTable.getRay(fromSquare, direction);

                for (int square : ray) {
                    uint64_t squareBit = 1ULL << square;

                    // A friendly piece blocks movement onto this square.
                    if (friendlyOccupancy & squareBit) {
                        break;
                    }

                    // Count empty squares and enemy-occupied capture squares.
                    ++mobility;

                    // Any occupied square blocks further movement.
                    if (occupancy & squareBit) {
                        break;
                    }
                }
            }
        }

        return mobility;
    };

    whiteMobility = calculateMobility(whiteBishop, whiteOccupancy);
    blackMobility = calculateMobility(blackBishop, blackOccupancy);

    // weighted difference, positive favours white
    return (whiteMobility - blackMobility) * EvalConstants::BISHOP_MOBILITY_WEIGHT;
}

// rook mobility difference, positive favours white, not used by the combined mobility at the moment
int Mobility::evaluateRookMobility(const ChessBoard &board) {
    uint64_t whiteOccupancy = 0;
    uint64_t blackOccupancy = 0;
    uint64_t occupancy = board.getAllOccupancy();

    // piece indices zero to five are white, six to eleven are black, build occupancy per colour
    for (int i = 0; i < 6; ++i) {
        whiteOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i)
        );

        blackOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i + 6)
        );
    }

    // the four straight directions
    const Direction rookDirections[4] = {
        up, down, left, right
    };

    // counts reachable squares for every rook of one colour
    auto calculateMobility = [&](Piece rook, uint64_t friendlyOccupancy) {
        int mobility = 0;

        uint64_t rooks = board.getPieceBitboard(rook);

        while (rooks) {
            int fromSquare = __builtin_ctzll(rooks);
            rooks &= rooks - 1;

            for (Direction direction : rookDirections) {
                const auto &ray =
                    AttackTables::rayTable.getRay(fromSquare, direction);

                for (int square : ray) {
                    uint64_t squareBit = 1ULL << square;

                    // own piece blocks the move
                    if (friendlyOccupancy & squareBit) {
                        break;
                    }

                    // empty squares and capture squares count as mobility
                    ++mobility;

                    // any piece ends the ray
                    if (occupancy & squareBit) {
                        break;
                    }
                }
            }
        }

        return mobility;
    };

    int whiteMobility =
        calculateMobility(whiteRook, whiteOccupancy);

    int blackMobility =
        calculateMobility(blackRook, blackOccupancy);

    // weighted difference, positive favours white
    return (whiteMobility - blackMobility) * EvalConstants::ROOK_MOBILITY_WEIGHT;
}

// queen mobility difference, positive favours white, not used by the combined mobility at the moment
int Mobility::evaluateQueenMobility(const ChessBoard &board) {
    uint64_t whiteOccupancy = 0;
    uint64_t blackOccupancy = 0;
    uint64_t occupancy = board.getAllOccupancy();

    // piece indices zero to five are white, six to eleven are black, build occupancy per colour
    for (int i = 0; i < 6; ++i) {
        whiteOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i)
        );

        blackOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i + 6)
        );
    }

    // all eight directions
    const Direction queenDirections[8] = {
        up, down, left, right,
        upLeft, upRight, downLeft, downRight
    };

    // counts reachable squares for every queen of one colour
    auto calculateMobility = [&](Piece queen, uint64_t friendlyOccupancy) {
        int mobility = 0;

        uint64_t queens = board.getPieceBitboard(queen);

        while (queens) {
            int fromSquare = __builtin_ctzll(queens);
            queens &= queens - 1;

            for (Direction direction : queenDirections) {
                const auto &ray =
                    AttackTables::rayTable.getRay(fromSquare, direction);

                for (int square : ray) {
                    uint64_t squareBit = 1ULL << square;

                    // own piece blocks the move
                    if (friendlyOccupancy & squareBit) {
                        break;
                    }

                    // empty squares and capture squares count as mobility
                    ++mobility;

                    // any piece ends the ray
                    if (occupancy & squareBit) {
                        break;
                    }
                }
            }
        }

        return mobility;
    };

    int whiteMobility =
        calculateMobility(whiteQueen, whiteOccupancy);

    int blackMobility =
        calculateMobility(blackQueen, blackOccupancy);

    // weighted difference, positive favours white
    return (whiteMobility - blackMobility) * EvalConstants::QUEEN_MOBILITY_WEIGHT;
}

// shared mobility counter for sliding pieces over the given directions
int Mobility::calculateSlidingMobility(
    const ChessBoard &board,
    Piece piece,
    uint64_t friendlyOccupancy,
    const Direction directions[],
    int directionCount
) {
    // whole board occupancy, used to stop rays
    uint64_t occupancy = board.getAllOccupancy();
    uint64_t pieces = board.getPieceBitboard(piece);

    int mobility = 0;

    while (pieces) {
        int fromSquare = __builtin_ctzll(pieces);
        pieces &= pieces - 1;

        // one ray per direction
        for (int i = 0; i < directionCount; ++i) {
            const auto &ray =
                AttackTables::rayTable.getRay(fromSquare, directions[i]);

            for (int square : ray) {
                uint64_t squareBit = 1ULL << square;

                // own piece blocks the move
                if (friendlyOccupancy & squareBit) {
                    break;
                }

                // empty squares and capture squares count as mobility
                ++mobility;

                // any piece ends the ray
                if (occupancy & squareBit) {
                    break;
                }
            }
        }
    }

    return mobility;
}

// total mobility difference for all piece types, positive favours white
int Mobility::evaluateMobility(const ChessBoard &board) {
    uint64_t whiteOccupancy = 0;
    uint64_t blackOccupancy = 0;

    // piece indices zero to five are white, six to eleven are black, build occupancy per colour
    for (int i = 0; i < 6; ++i) {
        whiteOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i)
        );

        blackOccupancy |= board.getPieceBitboard(
            static_cast<Piece>(i + 6)
        );
    }

    // direction sets shared by the sliding piece helper
    const Direction diagonalDirections[4] = {
        upLeft, upRight, downLeft, downRight
    };

    const Direction straightDirections[4] = {
        up, down, left, right
    };

    const Direction queenDirections[8] = {
        up, down, left, right,
        upLeft, upRight, downLeft, downRight
    };

    int whiteMobility = 0;
    int blackMobility = 0;

    // Knights
    auto calculateKnightMobility = [&](Piece knight, uint64_t friendly) {
        int mobility = 0;
        uint64_t pieces = board.getPieceBitboard(knight);

        while (pieces) {
            int square = __builtin_ctzll(pieces);
            pieces &= pieces - 1;

            uint64_t attacks = AttackTables::KnightAttacks[square];
            mobility += __builtin_popcountll(attacks & ~friendly);
        }

        return mobility;
    };

    // White
    whiteMobility += calculateKnightMobility(whiteKnight, whiteOccupancy)
                     * EvalConstants::KNIGHT_MOBILITY_WEIGHT;

    whiteMobility += calculateSlidingMobility(
        board, whiteBishop, whiteOccupancy,
        diagonalDirections, 4
    ) *EvalConstants:: BISHOP_MOBILITY_WEIGHT;

    whiteMobility += calculateSlidingMobility(
        board, whiteRook, whiteOccupancy,
        straightDirections, 4
    ) * EvalConstants::ROOK_MOBILITY_WEIGHT;

    whiteMobility += calculateSlidingMobility(
        board, whiteQueen, whiteOccupancy,
        queenDirections, 8
    ) * EvalConstants::QUEEN_MOBILITY_WEIGHT;

    // Black
    blackMobility += calculateKnightMobility(blackKnight, blackOccupancy)
                     * EvalConstants::KNIGHT_MOBILITY_WEIGHT;

    blackMobility += calculateSlidingMobility(
        board, blackBishop, blackOccupancy,
        diagonalDirections, 4
    ) * EvalConstants::BISHOP_MOBILITY_WEIGHT;

    blackMobility += calculateSlidingMobility(
        board, blackRook, blackOccupancy,
        straightDirections, 4
    ) * EvalConstants::ROOK_MOBILITY_WEIGHT;

    blackMobility += calculateSlidingMobility(
        board, blackQueen, blackOccupancy,
        queenDirections, 8
    ) * EvalConstants::QUEEN_MOBILITY_WEIGHT;

    // weights are already applied above
    return whiteMobility - blackMobility;
}
