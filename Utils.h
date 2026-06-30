//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_UTILS_H
#define CHESS_ENGINE_UTILS_H

#include <cstdint>
#include <string>

namespace Utils {
    // returns true if the specified square's bit is set
    bool checkBit(uint64_t board, int square);

    // sets a bit
    void setBit(uint64_t &board, int square);

    // clears a bit
    void clearBit(uint64_t &board, int square);

    // converts board coordinates into a square index
    int coordinateToPosition(const std::string &position);
}


#endif //CHESS_ENGINE_UTILS_H
