//
// Created by sahoo on 25-06-2026.
//

#include "chess/history/MoveHistory.h"
#include <stdexcept>

// stores a move and clears redo history when a new move starts a different line of play
void MoveHistory::pushMove(const Move &move, bool clearRedo) {
    history.push_back(move);
    if (clearRedo) {
        redoHistory.clear();
    }
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

// returns true if there is a move available to undo
bool MoveHistory::canUndo() const {
    return !history.empty();
}

// returns true if there is a move available to redo
bool MoveHistory::canRedo() const {
    return !redoHistory.empty();
}

// stores a move for redo after it has been undone
void MoveHistory::pushRedoMove(const Move &move) {
    redoHistory.push_back(move);
}

// removes and returns the most recent move available to redo
Move MoveHistory::popRedoMove() {
    if (redoHistory.empty()) {
        throw std::runtime_error("No moves to redo!");
    }

    Move nextMove = redoHistory.back();
    redoHistory.pop_back();
    return nextMove;
}

// restores a redone move without clearing the remaining redo history
void MoveHistory::pushRedoneMove(const Move &move) {
    history.push_back(move);
}
