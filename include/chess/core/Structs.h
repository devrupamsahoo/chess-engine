//
// Created by sahoo on 29-06-2026.
//

#ifndef CHESS_ENGINE_STRUCTS_H
#define CHESS_ENGINE_STRUCTS_H

#include "chess/core/Enums.h"


// wk/wq and bk/bq represent White/Black kingside/queenside castling rights
struct CastlingRights {
    bool wk = true;
    bool wq = true;
    bool bk = true;
    bool bq = true;
};

// stores the move and state needed for undo and redo
struct Move {
    // identifies the source and destination squares of the move
    int startSquare;
    int endSquare;

    // records the piece moved and any piece captured by the move
    Piece movedPiece;
    Piece capturedPiece;

    // store the replacement piece so undo can restore the original pawn.
    Piece promotedPiece = noPiece;

    // identifies which castling operation this move represents, if any
    CastlingType castlingType = noCastle;

    // preserve castling rights before and after the move for undo and redo
    CastlingRights oldCastlingRights;
    CastlingRights newCastlingRights;

    // preserve the en passant target before and after the move for undo and redo
    int oldEnPassantTarget = -1;
    int newEnPassantTarget = -1;

    // marks captures where the captured pawn is not on the destination square
    bool isEnPassantCapture = false;

    // preserve FEN move counters before and after the move for undo and redo
    int oldHalfmoveClock = 0;
    int newHalfmoveClock = 0;
    int oldFullmoveNumber = 1;
    int newFullmoveNumber = 1;
};


#endif //CHESS_ENGINE_STRUCTS_H
