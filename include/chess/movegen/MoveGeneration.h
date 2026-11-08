//
// Created by devrupamsahoo on 05-11-2026.
//

#ifndef CHESS_ENGINE_MOVEGENERATION_H
#define CHESS_ENGINE_MOVEGENERATION_H


#include "chess/core/ChessBoard.h"

#include <vector>

// one move produced by the generator, it may still leave the mover's own king in check
struct PseudoLegalMove {
    int startSquare;
    int endSquare;
    Piece promotion = noPiece;
    bool isEnPassant = false;
    CastlingType castling = noCastle;
};

class MoveGeneration {
public:
    // generates moves for the side to move, without checking legality
    static std::vector<PseudoLegalMove> generatePseudoLegalMoves(const ChessBoard& board);

    // discards all illegal moves
    static std::vector<PseudoLegalMove> filterLegalMoves(const ChessBoard& board, const std::vector<PseudoLegalMove>& candidates);

    static std::vector<PseudoLegalMove> generateLegalMoves(const ChessBoard& board);
};


#endif //CHESS_ENGINE_MOVEGENERATION_H
