//
// Created by sahoo on 23-06-2026.
//

#include "MoveValidator.h"
#include <iostream>

// ensures player enters valid input position
bool MoveValidator::isValidPosition(const std::string &position) {
    // returns false if input length is not 2
    if (position.length() != 2) {
        std::cout << "Invalid position length!!" << std::endl;
        return false;
    }

    // returns false if first character is something other than a...h
    if (position[0] < 'a' || position[0] > 'h') {
        std::cout << "Invalid position format!!"<< std::endl;
        return false;
    }

    // returns false if first character is something other than 1...8
    if (position[1] < '1' || position[1] > '8') {
        std::cout << "Invalid position format!!"<< std::endl;
        return false;
    }

    return true;
}
