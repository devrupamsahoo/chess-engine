//
// Created by sahoo on 23-06-2026.
//

#ifndef CHESS_ENGINE_MOVEVALIDATOR_H
#define CHESS_ENGINE_MOVEVALIDATOR_H

#include <string>

class MoveValidator {
public:
    // ensures player enters valid input position
    static bool isValidPosition(const std::string &position);
};


#endif //CHESS_ENGINE_MOVEVALIDATOR_H
