//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_MOVEVALIDATOR_H
#define CHESS_ENGINE_MOVEVALIDATOR_H

#include <string>
#include "ChessBoard.h"

class MoveValidator {
public:
    // returns true if the input position is valid
    static bool isValidPosition(const std::string &position);

    // returns true if the move is a valid knight move
    static bool isKnightMove(int startPos, int endPos);

    // master move validator function: returns true if the move is legal for the selected piece
    static bool isValidMove(ChessBoard &board, int startPos, int endPos);
};


#endif //CHESS_ENGINE_MOVEVALIDATOR_H
