//
// Created by sahoo on 25-06-2026.
//

#include "MovementPatterns.h"

// returns true if a move is horizontal
bool MovementPatterns::isHorizontalMove(int startPos, int endPos) {
    return (startPos >> 3) == (endPos >> 3);
}

// returns true if a move is vertical
bool MovementPatterns::isVerticalMove(int startPos, int endPos) {
    return (startPos & 7) == (endPos & 7);
}

// returns true if a move is diagonal
bool MovementPatterns::isDiagonalMove(int startPos, int endPos) {
    int rankDiff = std::abs((startPos >> 3) - (endPos >> 3));
    int fileDiff = std::abs((startPos & 7) - (endPos & 7));

    return rankDiff == fileDiff;
}

// returns the direction a piece is trying to move
Direction MovementPatterns::getMoveDirection(int startPos, int endPos) {
    int startRank = startPos >> 3;
    int startFile = startPos & 7;

    int endRank = endPos >> 3;
    int endFile = endPos & 7;

    int rankDiff = endRank - startRank;
    int fileDiff = endFile - startFile;

    // vertical movement
    if (isVerticalMove(startPos, endPos)) {
        if (rankDiff > 0) {
            return up;
        }
        return down;
    }

    // horizontal movement
    if (isHorizontalMove(startPos, endPos)) {
        if (fileDiff > 0) {
            return right;
        }
        return left;
    }

    // diagonal movement
    if (isDiagonalMove(startPos, endPos)) {
        if (rankDiff > 0 && fileDiff > 0) {
            return upRight;
        }

        if (rankDiff > 0 && fileDiff < 0) {
            return upLeft;
        }

        if (rankDiff < 0 && fileDiff > 0) {
            return downRight;
        }

        return downLeft;
    }

    throw std::invalid_argument("Move has no sliding direction.");
}
