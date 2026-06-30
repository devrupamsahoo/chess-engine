//
// Created by sahoo on 25-06-2026.
//

#include "MoveHistory.h"
#include <stdexcept>

// stores a move in the history
void MoveHistory::pushMove(const Move &move) {
    history.push_back(move);
}

// removes and returns the most recent move
Move MoveHistory::popMove() {
    if (history.empty()) {
        throw std::runtime_error("No moves to undo!");
    }

    Move lastMove = history.back();
    history.pop_back();
    return lastMove;
}

// returns true if the move history is empty
bool MoveHistory::isEmpty() const {
    return history.empty();
}
