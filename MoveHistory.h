//
// Created by sahoo on 25-06-2026.
//

#ifndef CHESS_ENGINE_MOVEHISTORY_H
#define CHESS_ENGINE_MOVEHISTORY_H

#include <vector>

#include "Structs.h"
#include "Enums.h"

class MoveHistory {
private:
    // vector for storing the history
    std::vector<Move> history;

public:
    // stores a move in the history
    void pushMove(const Move &move);

    // removes and returns the most recent move
    Move popMove();

    // returns true if the move history is empty
    bool isEmpty() const;
};


#endif //CHESS_ENGINE_MOVEHISTORY_H
