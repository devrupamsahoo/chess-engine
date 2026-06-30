//
// Created by sahoo on 25-06-2026.
//

#ifndef CHESS_ENGINE_KINGVALIDATOR_H
#define CHESS_ENGINE_KINGVALIDATOR_H

#include <cmath>
#include "ChessBoard.h"

class KingValidator {
public:
    // returns true if a king move is valid
    static bool isKingMove(int startPos, int endPos);

    // returns true if the specified player's king is currently in check
    static bool isKingInCheck(ChessBoard& board, bool isWhitePlayer);

    // returns true if the move leaves the current player's king in check
    static bool leavesKingInCheck(ChessBoard &board, int startPos, int endPos);
};


#endif //CHESS_ENGINE_KINGVALIDATOR_H
