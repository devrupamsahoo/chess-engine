//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_CHESSBOARD_H
#define CHESS_ENGINE_CHESSBOARD_H

#include <cstdint>
#include <array>
#include "Enums.h"
#include "Structs.h"
#include "MoveHistory.h"

class ChessBoard {
private:
    // array of 12 bitboards
    std::array<uint64_t, pieceCount> board{};

    // initial king position
    int blackKingPosition;
    int whiteKingPosition;

    // occupancy bitboards for instant occupancy lookups
    uint64_t whiteOccupancy = 0;
    uint64_t blackOccupancy = 0;
    uint64_t allOccupancy = 0;

    bool whiteTurn = true;

    // for castling
    CastlingRights castlingRights;

    // en passant
    int enPassantTarget = -1;

    // object to keep track of move history
    MoveHistory moveHistory;
public:
    // ctor
    ChessBoard();

    // initialize board
    void initBoard();

    // get occupancy bitboards
    uint64_t getWhiteOccupancy() const;
    uint64_t getBlackOccupancy() const;
    uint64_t getAllOccupancy() const;

    // update occupancy bitboards
    void updateOccupancy();

    // prints entire board
    void printBoard();

    // prints one bitboard
    void printBitboard(Piece pieceType);

    // returns true if it is White's turn
    bool isWhiteTurn();

    // switches player's turn
    void switchTurn();

    // returns the piece occupying the specified square
    Piece getPieceAt(int square);

    // returns true if the selected coordinate contains a piece belonging to the player whose turn it is
    bool isValidPiece(bool isWhitePlayer, int square);

    // moves a piece from start to end and replaces the origin square with empty space
    void movePiece(int startPos, int endPos);

    // returns the bitboard for the specified piece type
    uint64_t& getPieceBitboard(Piece pieceType);

    // returns the read-only bitboard for the specified piece type
    const uint64_t& getPieceBitboard(Piece pieceType) const;

    // returns the king position
    int getKingPos(bool isWhiteKing);

    // updates king position
    void updateKingPosition();

    // makes a move and stores the history
    void makeMove(int startPos, int endPos);

    // undo a move and delete its history
    void undoMove();

    // returns the current castling rights
    const CastlingRights& getCastlingRights() const;

    // executes a castling move
    void executeCastle(int startPos, int endPos, bool isWhitePlayer);

    // returns the current en passant target square
    int getEnPassantTarget() const;

    // dtor
    ~ChessBoard() = default;
};


#endif //CHESS_ENGINE_CHESSBOARD_H
