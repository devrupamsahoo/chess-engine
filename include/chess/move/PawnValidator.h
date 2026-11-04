//
// Created by sahoo on 26-06-2026.
//

#ifndef CHESS_ENGINE_PAWNVALIDATOR_H
#define CHESS_ENGINE_PAWNVALIDATOR_H

#include "chess/core/ChessBoard.h"

class PawnValidator {
public:
    // returns true if pawn is moving forward one step
    static bool isPawnForwardMove(ChessBoard &board, int startPos, int endPos);

    // returns true if the pawn can move two squares forward from its starting rank
    static bool isPawnInitialDoubleMove(ChessBoard &board, int startPos, int endPos);

    // returns true if the pawn follows its movement and capture rules
    static bool isPawnMove(ChessBoard &board, int startPos, int endPos);

    // returns true if pawn can capture diagonally
    static bool isPawnCaptureMove(ChessBoard &board, int startPos, int endPos);

    // returns true if the pawn moves diagonally to the current en passant target
    static bool isEnPassantMove(ChessBoard &board, int startPos, int endPos);

    // returns true if this side's pawn reaches its promotion rank
    static bool isPawnPromotion(int endPos, bool whitePlayer);

    // promotes a pawn that has reached the last rank
    static void promotePawn(ChessBoard &board, int endPos, char promotedPieceChar, bool isWhitePlayer);

};


#endif //CHESS_ENGINE_PAWNVALIDATOR_H
