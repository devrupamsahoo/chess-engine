//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef CHESS_ENGINE_GAME_PHASE_H
#define CHESS_ENGINE_GAME_PHASE_H

#include <cstdint>
#include "chess/core/ChessBoard.h"

class GamePhase {
public:
    // counts remaining minor and major pieces, knights and bishops one, rooks two, queens four, used to blend opening and endgame values
    static int calculateGamePhase(const ChessBoard &board);
};

#endif
