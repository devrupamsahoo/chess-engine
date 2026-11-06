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

    // sets the bit corresponding to the specified square to 1
    void setBit(uint64_t &board, int square);

    // clears the bit corresponding to the specified square to 0
    void clearBit(uint64_t &board, int square);

    // converts board coordinates into a square index
    int coordinateToPosition(const std::string &position);

}


#endif //CHESS_ENGINE_UTILS_H
