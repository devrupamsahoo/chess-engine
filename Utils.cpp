//
// Created by sahoo on 23-06-2026.
//

#include "Utils.h"

// returns true if a bit is set
bool Utils::checkBit(uint64_t board, int square) {
    return (board & (1ULL << square)) != 0;
}

// sets a bit
void Utils::setBit(uint64_t &board, int square) {
    board |= (1ULL << square);
}

// clears a bit
void Utils::clearBit(uint64_t &board, int square) {
    board &= ~(1ULL << square);
}

// converts user input to bitboard square
int Utils::coordinateToPosition(const std::string &position) {
    int file = position[0] - 'a';
    int rank = position[1] - '1';

    return (rank * 8) + file;
}
