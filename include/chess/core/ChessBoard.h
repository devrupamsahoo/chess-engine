//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_CHESSBOARD_H
#define CHESS_ENGINE_CHESSBOARD_H

#include <cstdint>
#include <array>
#include "chess/core/Enums.h"
#include "chess/core/Structs.h"
#include "chess/history/MoveHistory.h"

class FEN;

class ChessBoard {
private:
    friend class FEN;

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

    // plies since the last pawn move or capture, used by FEN
    int halfmoveClock = 0;

    // FEN move number, incremented after Black moves
    int fullmoveNumber = 1;

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



    // returns true if it is White's turn
    bool isWhiteTurn() const;

    // switches player's turn
    void switchTurn();

    // returns the piece occupying the specified square
    Piece getPieceAt(int square) const;

    // returns true if the selected coordinate contains a piece belonging to the player whose turn it is
    bool isValidPiece(bool isWhitePlayer, int square);

    // moves a piece from start to end and replaces the origin square with empty space
    void movePiece(int startPos, int endPos);

    // returns the bitboard for the specified piece type
    uint64_t& getPieceBitboard(Piece pieceType);

    // returns the read-only bitboard for the specified piece type
    const uint64_t& getPieceBitboard(Piece pieceType) const;

    // returns the king position
    int getKingPos(bool isWhiteKing) const;

    // updates king position
    void updateKingPosition();

    // apply a move and record the state needed to undo or redo it
    void makeMove(int startPos, int endPos, Piece promotedPiece = noPiece, bool isSimulation = false);

    // restore the previous position and save the move for redo when this is a real undo
    void undoMove(bool isSimulation = false);

    // report whether the move history contains a move to undo
    bool canUndo() const;

    // report whether the redo history contains a move to replay
    bool canRedo() const;

    // performs redo
    void redoMove();

    // returns the current castling rights
    const CastlingRights& getCastlingRights() const;

    // validate and execute castling, then save the resulting position state for undo and redo
    bool executeCastle(int startPos, int endPos, bool isWhitePlayer);

    // returns the current en passant target square
    int getEnPassantTarget() const;

    // dtor
    ~ChessBoard() = default;
};


#endif //CHESS_ENGINE_CHESSBOARD_H
