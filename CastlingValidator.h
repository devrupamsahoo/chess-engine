//
// Created by sahoo on 25-06-2026.
//

#ifndef CHESS_ENGINE_CASTLINGVALIDATOR_H
#define CHESS_ENGINE_CASTLINGVALIDATOR_H

#include "ChessBoard.h"

class CastlingValidator {
public:
    // returns true if kingside castling rights are still available
    static bool hasKingsideRights(ChessBoard &board, bool isWhitePlayer);

    // returns true if queenside castling rights are still available
    static bool hasQueensideRights(ChessBoard &board, bool isWhitePlayer);

    // returns true if all specified squares are safe from enemy attacks
    static bool areSquaresSafeFromAttack(ChessBoard &board, std::vector<int> &squares, bool isWhitePlayer);

    // returns true if all squares between the king and rook are unoccupied
    static bool isPathClear(ChessBoard &board, int startPos, int endPos);

    // returns true if castling is possible on the king side
    static bool canCastleKingside(ChessBoard &board, bool isWhitePlayer);

    // returns true if castling is possible on the queen side
    static bool canCastleQueenside(ChessBoard &board, bool isWhitePlayer);
};


#endif //CHESS_ENGINE_CASTLINGVALIDATOR_H
