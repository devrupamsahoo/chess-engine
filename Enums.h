//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_ENUMS_H
#define CHESS_ENGINE_ENUMS_H

#include <string_view>

// piece values are bitboard indices, so keep their order aligned with pieceToCharMap
// number of real pieces; noPiece is a sentinel and is not a bitboard
enum Piece {
    whitePawn,
    whiteKnight,
    whiteBishop,
    whiteRook,
    whiteQueen,
    whiteKing,

    blackPawn,
    blackKnight,
    blackBishop,
    blackRook,
    blackQueen,
    blackKing,

    noPiece
};

constexpr int pieceCount = 12;

// FEN letters indexed by Piece value, with uppercase for white and lowercase for black
constexpr std::string_view pieceToCharMap = "PNBRQKpnbrqk";

// square values follow rank-major order: a1 = 0, each file adds 1, and each rank adds 8
enum Square {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

constexpr int squareCount = 64;

// movement directions used for ray tracing
enum Direction {
    up,         // +8
    down,       // -8
    left,       // -1
    right,      // +1

    upLeft,     // +7
    upRight,    // +9
    downLeft,   // -9
    downRight   // -7
};

constexpr int directionCount = 8;

// identifies the castle recorded in a move; noCastle means the move was not castling
enum CastlingType {
    noCastle,
    whiteKingside,
    whiteQueenside,
    blackKingside,
    blackQueenside
};

#endif //CHESS_ENGINE_ENUMS_H
