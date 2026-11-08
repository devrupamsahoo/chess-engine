//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../../include/chess/movegen/MoveGeneration.h"
#include <bit>
#include "chess/move/AttackValidator.h"
#include "chess/move/KingValidator.h"

// helper functions used only inside this file
namespace {
    // adds a pawn move to the list, expanding it into four moves if the pawn reaches the last rank
    void addPawnMove(
        std::vector<PseudoLegalMove> &moves,
        int from,
        int to,
        bool white,
        bool isEnPassant = false) {
        // last rank for the pawn's color, where promotion happens
        const int lastRank = white ? 7 : 0;

        // converts the target square into its rank and checks if it is the promotion rank
        if (to / 8 == lastRank) {
            // pieces a pawn can promote to, in the color of the moving side
            const Piece choices[] = {
                white ? whiteQueen : blackQueen,
                white ? whiteRook : blackRook,
                white ? whiteBishop : blackBishop,
                white ? whiteKnight : blackKnight
            };

            // one separate move for every promotion choice
            for (Piece choice: choices) {
                moves.push_back({from, to, choice, isEnPassant});
            }
        } else {
            // normal pawn move, noPiece means there is no promotion
            moves.push_back({from, to, noPiece, isEnPassant});
        }
    }
}

// generates every move the side to move can make without checking if its own king is left in check
std::vector<PseudoLegalMove> MoveGeneration::generatePseudoLegalMoves(const ChessBoard &board) {
    std::vector<PseudoLegalMove> moves;

    // common setup used by all piece types

    // side to move decides which color's pieces are generated
    const bool whiteToMove = board.isWhiteTurn();

    // enemy king is never a capture target, so its square is excluded from every move
    const Piece enemyKing = whiteToMove ? blackKing : whiteKing;
    const uint64_t enemyKingBit = board.getPieceBitboard(enemyKing);

    // squares occupied by own pieces, a piece can't move onto them
    const uint64_t ownPieces = whiteToMove ? board.getWhiteOccupancy() : board.getBlackOccupancy();

    // knight moves
    const Piece knight = whiteToMove ? whiteKnight : blackKnight;

    uint64_t knights = board.getPieceBitboard(knight);

    // iterate through every knight on the board
    while (knights != 0) {
        // extracts the least significant set bit, which represents one knight
        const int from = std::countr_zero(knights);

        // removes the processed knight from the bitboard
        knights &= knights - 1;

        // precomputed knight attacks, minus own pieces and the enemy king
        uint64_t targets = AttackTables::KnightAttacks[from] & ~ownPieces & ~enemyKingBit;

        // every remaining target square becomes one move
        while (targets != 0) {
            const int to = std::countr_zero(targets);

            targets &= targets - 1;

            moves.push_back({from, to});
        }
    }

    // king moves (one square moves only, castling is handled later)
    const Piece king = whiteToMove ? whiteKing : blackKing;

    uint64_t kings = board.getPieceBitboard(king);

    while (kings != 0) {
        const int from = std::countr_zero(kings);

        kings &= kings - 1;

        // precomputed king attacks, minus own pieces and the enemy king, attacked squares are filtered later
        uint64_t targets = AttackTables::KingAttacks[from] & ~ownPieces & ~enemyKingBit;

        while (targets != 0) {
            const int to = std::countr_zero(targets);

            targets &= targets - 1;

            moves.push_back({from, to});
        }
    }

    // bishop moves
    const Piece bishop = whiteToMove ? whiteBishop : blackBishop;
    uint64_t bishops = board.getPieceBitboard(bishop);

    // bishops slide only along the four diagonals
    const Direction diagonalDirections[] = {
        upLeft, upRight, downLeft, downRight,
    };

    while (bishops != 0) {
        const int from = std::countr_zero(bishops);
        bishops &= bishops - 1;

        // walks each diagonal ray until something blocks it
        for (Direction direction: diagonalDirections) {
            // all squares in this direction from the bishop's square, nearest first
            const auto &ray = AttackTables::rayTable.getRay(from, direction);

            for (int to: ray) {
                // piece standing on the current square, noPiece if empty
                const Piece target = board.getPieceAt(to);

                // stops before any king, kings can't be captured
                if (target == king || target == (whiteToMove ? blackKing : whiteKing)) {
                    break;
                }

                // stops before own piece, everything behind it is blocked too
                if ((ownPieces & (1ULL << to)) != 0) {
                    break;
                }

                // square is empty or holds an enemy piece, so the move is valid
                moves.push_back({from, to});

                // an enemy piece can be captured but the ray can't continue past it
                if (target != noPiece) {
                    break;
                }
            }
        }
    }

    // rook moves
    const Piece rook = whiteToMove ? whiteRook : blackRook;
    uint64_t rooks = board.getPieceBitboard(rook);

    // rooks slide only along ranks and files
    const Direction straightDirections[] = {
        up, down, left, right
    };

    while (rooks != 0) {
        const int from = std::countr_zero(rooks);
        rooks &= rooks - 1;

        // walks each straight ray until something blocks it
        for (Direction direction: straightDirections) {
            const auto &ray = AttackTables::rayTable.getRay(from, direction);

            for (int to: ray) {
                const Piece target = board.getPieceAt(to);

                // stops before own piece
                if ((ownPieces & (1ULL << to)) != 0) {
                    break;
                }

                // stops before the enemy king, it can't be captured
                if (target == (whiteToMove ? blackKing : whiteKing)) {
                    break;
                }

                moves.push_back({from, to});

                // capture ends the ray
                if (target != noPiece) {
                    break;
                }
            }
        }
    }

    // queen moves
    const Piece queen = whiteToMove ? whiteQueen : blackQueen;
    uint64_t queens = board.getPieceBitboard(queen);

    // queen combines rook and bishop, so it slides in all eight directions
    const Direction queenDirections[] = {
        up, down, left, right,
        upLeft, upRight, downLeft, downRight
    };

    while (queens != 0) {
        const int from = std::countr_zero(queens);
        queens &= queens - 1;

        // walks each of the eight rays until something blocks it
        for (Direction direction: queenDirections) {
            const auto &ray = AttackTables::rayTable.getRay(from, direction);

            for (int to: ray) {
                const Piece target = board.getPieceAt(to);

                // stops before own piece
                if ((ownPieces & (1ULL << to)) != 0) {
                    break;
                }

                // stops before the enemy king
                if (target == (whiteToMove ? blackKing : whiteKing)) {
                    break;
                }

                moves.push_back({from, to});

                // capture ends the ray
                if (target != noPiece) {
                    break;
                }
            }
        }
    }

    // pawn forward moves
    const Piece pawn = whiteToMove ? whitePawn : blackPawn;
    uint64_t pawns = board.getPieceBitboard(pawn);

    // white moves up the board (+8 per rank), black moves down (-8 per rank)
    const int step = whiteToMove ? 8 : -8;

    // rank where pawns begin, a double move is allowed only from here
    const int startingRank = whiteToMove ? 1 : 6;

    while (pawns != 0) {
        const int from = std::countr_zero(pawns);
        pawns &= pawns - 1;

        // square directly in front of the pawn
        const int oneStep = from + step;

        // blocked pawn can't move forward at all, not even two squares
        if (board.getPieceAt(oneStep) != noPiece) {
            continue;
        }

        // single forward move, handles promotion if it reaches the last rank
        addPawnMove(moves, from, oneStep, whiteToMove);

        // forward double move
        const int rank = from / 8;
        const int twoSteps = from + 2 * step;

        // double move needs the pawn on its starting rank and the second square empty
        if (rank == startingRank && board.getPieceAt(twoSteps) == noPiece) {
            addPawnMove(moves, from, twoSteps, whiteToMove);
        }
    }

    // pawn diagonal captures
    // squares occupied by enemy pieces, a pawn can only capture diagonally onto these
    const uint64_t enemyPieces = whiteToMove ? board.getBlackOccupancy() : board.getWhiteOccupancy();

    // pawn bitboard was consumed by the loop above, so it is loaded again
    pawns = board.getPieceBitboard(pawn);

    while (pawns != 0) {
        const int from = std::countr_zero(pawns);
        pawns &= pawns - 1;

        // diagonal squares the pawn attacks, only those holding an enemy piece are valid captures
        uint64_t targets = AttackTables::PawnAttacks[whiteToMove ? 0 : 1][from] & enemyPieces;

        while (targets != 0) {
            const int to = std::countr_zero(targets);

            targets &= targets - 1;

            // enemy king can't be captured, handles promotion if the capture reaches the last rank
            if (board.getPieceAt(to) != enemyKing) {
                addPawnMove(moves, from, to, whiteToMove);
            }
        }
    }

    // en passant capture
    // square behind a pawn that just moved two squares, -1 if en passant is not available
    const int enPassantTarget = board.getEnPassantTarget();

    if (enPassantTarget != -1) {
        pawns = board.getPieceBitboard(pawn);

        while (pawns != 0) {
            const int from = std::countr_zero(pawns);
            pawns &= pawns - 1;

            const uint64_t pawnAttacks = AttackTables::PawnAttacks[whiteToMove ? 0 : 1][from];

            // pawn can capture en passant if it attacks the en passant square, which is empty so the normal capture loop misses it
            if ((pawnAttacks & (1ULL << enPassantTarget)) != 0) {
                // last argument marks the move as en passant
                addPawnMove(moves, from, enPassantTarget, whiteToMove, true);
            }
        }
    }

    // castling candidates
    // only rights and empty squares are checked here, king in check or crossing an attacked square is verified later
    const CastlingRights &rights = board.getCastlingRights();

    if (whiteToMove) {
        // white kingside, needs the right, king on e1, rook on h1 and f1, g1 empty
        if (rights.wk && board.getPieceAt(e1) == whiteKing &&
            board.getPieceAt(h1) == whiteRook &&
            board.getPieceAt(f1) == noPiece &&
            board.getPieceAt(g1) == noPiece) {
            moves.push_back({e1, g1, noPiece, false, whiteKingside});
        }

        // white queenside, needs the right, king on e1, rook on a1 and d1, c1, b1 empty
        if (rights.wq && board.getPieceAt(e1) == whiteKing &&
            board.getPieceAt(a1) == whiteRook &&
            board.getPieceAt(d1) == noPiece &&
            board.getPieceAt(c1) == noPiece &&
            board.getPieceAt(b1) == noPiece) {
            moves.push_back({e1, c1, noPiece, false, whiteQueenside});
        }
    } else {
        // black kingside, needs the right, king on e8, rook on h8 and f8, g8 empty
        if (rights.bk && board.getPieceAt(e8) == blackKing &&
            board.getPieceAt(h8) == blackRook &&
            board.getPieceAt(f8) == noPiece &&
            board.getPieceAt(g8) == noPiece) {
            moves.push_back({e8, g8, noPiece, false, blackKingside});
        }

        // black queenside, needs the right, king on e8, rook on a8 and d8, c8, b8 empty
        if (rights.bq && board.getPieceAt(e8) == blackKing &&
            board.getPieceAt(a8) == blackRook &&
            board.getPieceAt(d8) == noPiece &&
            board.getPieceAt(c8) == noPiece &&
            board.getPieceAt(b8) == noPiece) {
            moves.push_back({e8, c8, noPiece, false, blackQueenside});
        }
    }

    return moves;
}

// removes pseudo legal moves that leave the mover's own king in check
std::vector<PseudoLegalMove> MoveGeneration::filterLegalMoves(const ChessBoard &board, const std::vector<PseudoLegalMove> &candidates) {
    const bool whiteToMove = board.isWhiteTurn();

    std::vector<PseudoLegalMove> legalMoves;

    for (const PseudoLegalMove &candidate: candidates) {
        ChessBoard copyBoard(board);

        if (candidate.castling != noCastle) {
            if (!copyBoard.executeCastle(candidate.startSquare, candidate.endSquare, whiteToMove)) {
                continue;
            }
        } else {
            copyBoard.makeMove(candidate.startSquare, candidate.endSquare, candidate.promotion, true);
        }

        if (!KingValidator::isKingInCheck(copyBoard, whiteToMove)) {
            legalMoves.push_back(candidate);
        }
    }


    return legalMoves;
}

std::vector<PseudoLegalMove> MoveGeneration::generateLegalMoves(const ChessBoard& board) {
    const auto pseudoMoves = generatePseudoLegalMoves(board);

    return filterLegalMoves(board, pseudoMoves);
}