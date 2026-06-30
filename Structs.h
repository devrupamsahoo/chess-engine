//
// Created by sahoo on 29-06-2026.
//

#ifndef CHESS_ENGINE_STRUCTS_H
#define CHESS_ENGINE_STRUCTS_H

#include "Enums.h"


// stores castling rights
struct CastlingRights {
    bool wk = true;
    bool wq = true;
    bool bk = true;
    bool bq = true;
};

// stores all information required to undo a move
struct Move {
    int startSquare;
    int endSquare;
    Piece movedPiece;
    Piece capturedPiece;

    CastlingType castlingType = noCastle;
    CastlingRights oldCastlingRights;

    int oldEnPassantTarget = -1;
    bool isEnPassantCapture = false;
};


#endif //CHESS_ENGINE_STRUCTS_H
