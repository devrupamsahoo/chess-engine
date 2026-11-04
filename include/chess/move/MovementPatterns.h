//
// Created by sahoo on 25-06-2026.
//

#ifndef CHESS_ENGINE_MOVEMENTPATTERNS_H
#define CHESS_ENGINE_MOVEMENTPATTERNS_H

#include <cmath>
#include <stdexcept>
#include "chess/core/Enums.h"

class MovementPatterns {
public:
    // returns true if a move is horizontal
    static bool isHorizontalMove(int startPos, int endPos);

    // returns true if a move is vertical
    static bool isVerticalMove(int startPos, int endPos);

    // returns true if a move is diagonal
    static bool isDiagonalMove(int startPos, int endPos);

    // returns the sliding direction, or throws if the squares are not aligned
    static Direction getMoveDirection(int startPos, int endPos);
};


#endif //CHESS_ENGINE_MOVEMENTPATTERNS_H
