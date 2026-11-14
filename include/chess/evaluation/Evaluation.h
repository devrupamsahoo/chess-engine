//
// Created by devrupamsahoo on 08-11-2026.
//

#ifndef CHESS_ENGINE_EVALUATION_H
#define CHESS_ENGINE_EVALUATION_H

#include "chess/core/ChessBoard.h"

class Evaluation {
public:
    // static score in centipawns from the side to move perspective
    int evaluate(const ChessBoard &board);

    // material-only score in centipawns from White's perspective
    int evaluateMaterial(const ChessBoard &board);

    // returns the total piece-square table score from White's perspective
    int evaluatePieceSquareTables(const ChessBoard &board);
};

#endif
