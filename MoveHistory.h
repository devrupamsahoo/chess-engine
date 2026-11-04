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

    // vector for storing moves for redo
    std::vector<Move> redoHistory;

public:
    // stores a move and clears redo history when a new move starts a different line of play
    void pushMove(const Move &move, bool clearRedo = true);

    // removes and returns the most recent move
    Move popMove();

    // returns true if the move history is empty
    bool isEmpty() const;

    // returns true if there is a move available to undo
    bool canUndo() const;

    // returns true if there is a move available to redo
    bool canRedo() const;

    // stores a move for redo after it has been undone
    void pushRedoMove(const Move &move);

    // removes and returns the most recent move available to redo
    Move popRedoMove();

    // restores a redone move without clearing the remaining redo history
    void pushRedoneMove(const Move &move);
};


#endif //CHESS_ENGINE_MOVEHISTORY_H
