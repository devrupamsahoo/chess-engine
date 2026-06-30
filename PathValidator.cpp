//
// Created by sahoo on 25-06-2026.
//

#include "PathValidator.h"
#include "MovementPatterns.h"
#include "Utils.h"

// returns true if any piece blocks the path between startPos and endPos
bool PathValidator::isPathBlocked(ChessBoard &board, int startPos, int endPos) {
    // determine movement direction
    Direction moveDirection = MovementPatterns::getMoveDirection(startPos, endPos);

    // get all squares along the movement ray
    const std::vector<int>& ray = AttackTables::rayTable.getRay(startPos, moveDirection);

    // traverse the ray
    for (int square : ray) {

        // reached destination
        if (square == endPos) {
            break;
        }

        // another piece blocks the path
        if (Utils::checkBit(board.getAllOccupancy(), square)) {
            return true;
        }
    }

    return false;
}
