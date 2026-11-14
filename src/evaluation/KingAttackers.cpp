//
// Created by devrupamsahoo on 10-11-2026.
//

#include "chess/evaluation/KingAttackers.h"
#include <algorithm>
#include "chess/move/AttackValidator.h"
#include "chess/core/Utils.h"
#include "chess/evaluation/EvaluationConstants.h"

// counts pieces of one type that attack a square, pawns and knights use tables, sliders walk rays
int KingAttackers::countAttackers(const ChessBoard &board, int square, bool attackerIsWhite, Piece piece) {
    int count = 0;

    const bool pieceIsWhite =
        piece >= whitePawn && piece <= whiteKing;

    if (pieceIsWhite != attackerIsWhite) {
        return 0;
    }

    uint64_t pieces = board.getPieceBitboard(piece);
    uint64_t occupancy = board.getAllOccupancy();

    // Handle pawn and knight attacks
    if (piece == whitePawn || piece == blackPawn ||
        piece == whiteKnight || piece == blackKnight) {

        // pawn attack tables are indexed by colour, zero white, one black
        int color = attackerIsWhite ? 0 : 1;

        while (pieces) {
            int fromSquare = __builtin_ctzll(pieces);
            pieces &= pieces - 1;

            uint64_t attackMask;

            if (piece == whitePawn || piece == blackPawn) {
                attackMask = AttackTables::PawnAttacks[color][fromSquare];
            } else {
                attackMask = AttackTables::KnightAttacks[fromSquare];
            }

            // this attacker hits the target square
            if (attackMask & (1ULL << square)) {
                ++count;
            }
        }

        return count;
    }

    // Sliding attack directions
    Direction bishopDirs[4] = {
        upLeft, upRight, downLeft, downRight
    };

    Direction rookDirs[4] = {
        up, down, left, right
    };

    Direction queenDirs[8] = {
        upLeft, upRight, downLeft, downRight,
        up, down, left, right
    };

    // pick the direction set that matches the slider type
    const Direction *dirs = nullptr;
    int directionCount = 0;

    if (piece == whiteBishop || piece == blackBishop) {
        dirs = bishopDirs;
        directionCount = 4;
    }
    else if (piece == whiteRook || piece == blackRook) {
        dirs = rookDirs;
        directionCount = 4;
    }
    else if (piece == whiteQueen || piece == blackQueen) {
        dirs = queenDirs;
        directionCount = 8;
    }
    else {
        return 0;
    }

    // Check sliding attacks
    while (pieces) {
        int fromSquare = __builtin_ctzll(pieces);
        pieces &= pieces - 1;

        for (int i = 0; i < directionCount; ++i) {
            const std::vector<int> &ray = AttackTables::rayTable.getRay(fromSquare, dirs[i]);

            // walk outward until the target is reached or a blocker stops the ray
            for (int target : ray) {

                if (target == square) {
                    ++count;
                    break;
                }

                // any occupied square blocks the ray
                if (Utils::checkBit(occupancy, target)) {
                    break;
                }
            }
        }
    }

    return count;
}

// checks whether one specific piece attacks the target square
bool KingAttackers::pieceAttacksSquare(
    const ChessBoard& board,
    int fromSquare,
    int targetSquare,
    Piece piece
) {
    // pawn attacks come directly from the precomputed colour table
    if (piece == whitePawn || piece == blackPawn) {
        const int color = piece == whitePawn ? 0 : 1;

        return (AttackTables::PawnAttacks[color][fromSquare] &
                (1ULL << targetSquare)) != 0;
    }

    // knight attacks come directly from the precomputed knight table
    if (piece == whiteKnight || piece == blackKnight) {
        return (AttackTables::KnightAttacks[fromSquare] &
                (1ULL << targetSquare)) != 0;
    }

    // direction sets for sliding pieces
    Direction bishopDirections[4] = {
        upLeft, upRight, downLeft, downRight
    };

    Direction rookDirections[4] = {
        up, down, left, right
    };

    Direction queenDirections[8] = {
        upLeft, upRight, downLeft, downRight,
        up, down, left, right
    };

    // select the directions belonging to this piece type
    const Direction* directions = nullptr;
    int directionCount = 0;

    if (piece == whiteBishop || piece == blackBishop) {
        directions = bishopDirections;
        directionCount = 4;
    }
    else if (piece == whiteRook || piece == blackRook) {
        directions = rookDirections;
        directionCount = 4;
    }
    else if (piece == whiteQueen || piece == blackQueen) {
        directions = queenDirections;
        directionCount = 8;
    }
    else {
        return false;
    }

    // occupied squares stop sliding attacks
    const uint64_t occupancy = board.getAllOccupancy();

    // walk every ray from this exact attacker square
    for (int i = 0; i < directionCount; ++i) {
        const auto& ray =
            AttackTables::rayTable.getRay(fromSquare, directions[i]);

        for (int square : ray) {
            // reaching the target means this piece attacks it
            if (square == targetSquare) {
                return true;
            }

            // any occupied square blocks the remaining ray
            if (Utils::checkBit(occupancy, square)) {
                break;
            }
        }
    }

    return false;
}

// counts distinct enemy pieces attacking squares next to the king, not used by the main evaluation at the moment
int KingAttackers::countUniqueKingAttackers(const ChessBoard &board, int kingSquare, bool attackerIsWhite) {
    // no king on the board means no pressure
    if (kingSquare < 0 || kingSquare >= squareCount) {
        return 0;
    }

    // bitboard of attackers already counted so none is counted twice
    uint64_t countedAttackers = 0;

    int kingFile = kingSquare % 8;
    int kingRank = kingSquare / 8;

    for (int rankOffset = -1; rankOffset <= 1; ++rankOffset) {
        for (int fileOffset = -1; fileOffset <= 1; ++fileOffset) {

            // skip the king square itself
            if (rankOffset == 0 && fileOffset == 0) {
                continue;
            }

            int targetRank = kingRank + rankOffset;
            int targetFile = kingFile + fileOffset;

            // skip neighbours that fall off the board
            if (targetRank < 0 || targetRank >= 8 ||
                targetFile < 0 || targetFile >= 8) {
                continue;
            }

            int targetSquare = targetRank * 8 + targetFile;

            // attacker piece types of the chosen colour
            Piece attackers[5];

            if (attackerIsWhite) {
                attackers[0] = whitePawn;
                attackers[1] = whiteKnight;
                attackers[2] = whiteBishop;
                attackers[3] = whiteRook;
                attackers[4] = whiteQueen;
            }else {
                attackers[0] = blackPawn;
                attackers[1] = blackKnight;
                attackers[2] = blackBishop;
                attackers[3] = blackRook;
                attackers[4] = blackQueen;
            }

            for (Piece piece : attackers) {

                uint64_t pieces = board.getPieceBitboard(piece);

                while (pieces) {
                    int fromSquare = __builtin_ctzll(pieces);
                    pieces &= pieces - 1;

                    uint64_t attackerBit = 1ULL << fromSquare;

                    // already counted, skip it
                    if (countedAttackers & attackerBit) {
                        continue;
                    }

                    // mark this exact piece only when it attacks the target square
                    if (pieceAttacksSquare(
                            board,
                            fromSquare,
                            targetSquare,
                            piece)) {
                        countedAttackers |= attackerBit;
                            }
                }
            }
        }
    }

    return __builtin_popcountll(countedAttackers);
}

// weighted pressure from distinct attackers hitting the king zone plus a bonus by attacker count
int KingAttackers::evaluateUniqueKingAttackers(
    const ChessBoard &board,
    int kingSquare,
    bool attackerIsWhite
) {
    // a missing or invalid king cannot have attackers
    if (kingSquare < 0 || kingSquare >= squareCount) {
        return 0;
    }

    // attacker piece types of the chosen colour, pawn to queen
    Piece attackers[5] = {
        attackerIsWhite ? whitePawn   : blackPawn,
        attackerIsWhite ? whiteKnight : blackKnight,
        attackerIsWhite ? whiteBishop : blackBishop,
        attackerIsWhite ? whiteRook   : blackRook,
        attackerIsWhite ? whiteQueen  : blackQueen
    };

    // pressure weight per attacker type, same order as above
    const int weights[5] = {5, 10, 10, 15, 20};

    // king zone is the set of squares next to the king
    uint64_t kingZone = AttackTables::KingAttacks[kingSquare];
    uint64_t occupancy = board.getAllOccupancy();

    uint64_t countedAttackers = 0;
    int pressure = 0;

    // Checks whether a sliding piece attacks any square in the king zone.
    auto attacksAlongRay = [&](int fromSquare, Direction direction) {
        const auto &ray =
            AttackTables::rayTable.getRay(fromSquare, direction);

        for (int square : ray) {
            uint64_t squareBit = 1ULL << square;

            if (kingZone & squareBit) {
                return true;
            }

            if (occupancy & squareBit) {
                break;
            }
        }

        return false;
    };

    // loop over attacker types, the index selects the weight and the attack test
    for (int i = 0; i < 5; ++i) {
        uint64_t pieces = board.getPieceBitboard(attackers[i]);

        while (pieces) {
            int fromSquare = __builtin_ctzll(pieces);
            pieces &= pieces - 1;

            // true when this piece attacks any king zone square
            bool attacksZone = false;

            switch (i) {
                case 0: // Pawn
                    attacksZone =
                        (AttackTables::PawnAttacks[attackerIsWhite ? 0 : 1]
                            [fromSquare] & kingZone) != 0;
                    break;

                case 1: // Knight
                    attacksZone =
                        (AttackTables::KnightAttacks[fromSquare] & kingZone) != 0;
                    break;

                case 2: // Bishop
                    attacksZone =
                        attacksAlongRay(fromSquare, upLeft) ||
                        attacksAlongRay(fromSquare, upRight) ||
                        attacksAlongRay(fromSquare, downLeft) ||
                        attacksAlongRay(fromSquare, downRight);
                    break;

                case 3: // Rook
                    attacksZone =
                        attacksAlongRay(fromSquare, up) ||
                        attacksAlongRay(fromSquare, down) ||
                        attacksAlongRay(fromSquare, left) ||
                        attacksAlongRay(fromSquare, right);
                    break;

                case 4: // Queen
                    attacksZone =
                        attacksAlongRay(fromSquare, upLeft) ||
                        attacksAlongRay(fromSquare, upRight) ||
                        attacksAlongRay(fromSquare, downLeft) ||
                        attacksAlongRay(fromSquare, downRight) ||
                        attacksAlongRay(fromSquare, up) ||
                        attacksAlongRay(fromSquare, down) ||
                        attacksAlongRay(fromSquare, left) ||
                        attacksAlongRay(fromSquare, right);
                    break;
            }

            // count the piece once and add its weight
            if (attacksZone) {
                countedAttackers |= 1ULL << fromSquare;
                pressure += weights[i];
            }
        }
    }

    // number of distinct attackers, capped to the bonus table size
    int attackerCount = __builtin_popcountll(countedAttackers);
    attackerCount = std::min(attackerCount, 5);

    return pressure + EvalConstants::KING_ATTACKER_COUNT_BONUS[attackerCount];
}
