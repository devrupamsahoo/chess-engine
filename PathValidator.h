//
// Created by sahoo on 25-06-2026.
//

#ifndef CHESS_ENGINE_PATHVALIDATOR_H
#define CHESS_ENGINE_PATHVALIDATOR_H

#include "ChessBoard.h"
#include "RayTable.h"

class PathValidator {
public:
    // returns true if any piece blocks the path between startPos and endPos
    static bool isPathBlocked(ChessBoard &board, int startPos, int endPos);
};


#endif //CHESS_ENGINE_PATHVALIDATOR_H
