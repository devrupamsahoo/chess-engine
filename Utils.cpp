//
// Created by sahoo on 23-06-2026.
//

#include <stdexcept>
#include "Utils.h"

// returns true if the specified square's bit is set
bool Utils::checkBit(uint64_t board, int square) {
    // reject invalid squares before shifting; an invalid shift count is undefined behavior
    if (square < 0 || square >= 64) {
        throw std::out_of_range("Square index must be between 0 and 63");
    }

    return (board & (1ULL << square)) != 0;
}

// sets a bit
void Utils::setBit(uint64_t &board, int square) {
    // reject invalid squares before shifting; an invalid shift count is undefined behavior
    if (square < 0 || square >= 64) {
        throw std::out_of_range("Square index must be between 0 and 63");
    }

    board |= (1ULL << square);
}

// clears a bit
void Utils::clearBit(uint64_t &board, int square) {
    // reject invalid squares before shifting; an invalid shift count is undefined behavior
    if (square < 0 || square >= 64) {
        throw std::out_of_range("Square index must be between 0 and 63");
    }

    board &= ~(1ULL << square);
}

// converts board coordinates into a square index
int Utils::coordinateToPosition(const std::string &position) {
    // only lowercase board coordinates from a1 through h8 map to valid squares
    if (position.length() != 2 || position[0] < 'a' || position[0] > 'h' || position[1] < '1' || position[1] > '8') {
        throw std::invalid_argument("Invalid chess coordinate");
    }

    int file = position[0] - 'a';
    int rank = position[1] - '1';

    return (rank * 8) + file;
}

