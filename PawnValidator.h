//
// Created by sahoo on 26-06-2026.
//

#ifndef CHESS_ENGINE_PAWNVALIDATOR_H
#define CHESS_ENGINE_PAWNVALIDATOR_H

#include "ChessBoard.h"

class PawnValidator {
public:
    // returns true if pawn is moving forward one step
    static bool isPawnForwardMove(ChessBoard &board, int startPos, int endPos);

    // returns true if the pawn can move two squares forward from its starting rank
    static bool isPawnInitialDoubleMove(ChessBoard &board, int startPos, int endPos);

    // master pawn validator: returns true if the pawn move is legal
    static bool isPawnMove(ChessBoard &board, int startPos, int endPos);

    // returns true if pawn can capture diagonally
    static bool isPawnCaptureMove(ChessBoard &board, int startPos, int endPos);

    // returns true if en passant move is allowed
    static bool isEnPassantMove(ChessBoard &board, int startPos, int endPos);

    // promotes a pawn that has reached the last rank
    static void promotePawn(ChessBoard &board, int endPos, bool isWhitePlayer);
};


#endif //CHESS_ENGINE_PAWNVALIDATOR_H
