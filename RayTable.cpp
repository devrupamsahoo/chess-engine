//
// Created by sahoo on 27-06-2026.
//

#include "RayTable.h"

// initializes the ray lookup table
RayTable::RayTable() {
    initializeRays();
}

namespace {
    struct Offset {
        int dr;
        int df;
    };
}

// precomputes rays for every square in every direction
void RayTable::initializeRays() {
    constexpr Offset offsets[directionCount] = {
        { 1,  0},   // up
        {-1,  0},   // down
        { 0, -1},   // left
        { 0,  1},   // right
        { 1, -1},   // upLeft
        { 1,  1},   // upRight
        {-1, -1},   // downLeft
        {-1,  1}    // downRight
    };

    // generate rays for every square on the board
    for (int square = 0; square < squareCount; square++) {
        int rank = square >> 3;
        int file = square & 7;

        // generate rays in all 8 directions from this square
        for (int direction = 0; direction < directionCount; direction++) {
            int currentRank = rank;
            int currentFile = file;

            // walk in the current direction until the board edge
            while (true) {
                currentRank += offsets[direction].dr;
                currentFile += offsets[direction].df;

                if (currentRank < 0 || currentRank > 7 || currentFile < 0 || currentFile > 7) {
                    break;
                }

                // store every reachable square in this ray
                rays[square][direction].push_back(currentRank * 8 + currentFile);
            }
        }
    }
}

// returns the precomputed ray from the given square and direction
const std::vector<int>& RayTable::getRay(int square, Direction direction) const {
    return rays[square][direction];
}

namespace AttackTables {
    RayTable rayTable;
}
