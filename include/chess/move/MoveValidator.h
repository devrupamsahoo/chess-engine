//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_MOVEVALIDATOR_H
#define CHESS_ENGINE_MOVEVALIDATOR_H

#include "chess/core/ChessBoard.h"

// validates piece movement rules and determines whether a proposed move is legal
class MoveValidator {
public:
    // returns true if the move is a valid knight move
    static bool isKnightMove(int startPos, int endPos);

    // master move validator function: returns true if the move is legal for the selected piece
    static bool isValidMove(ChessBoard &board, int startPos, int endPos);

};


#endif //CHESS_ENGINE_MOVEVALIDATOR_H
