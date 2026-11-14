//
// Created by devrupamsahoo on 10-11-2026.
//

#ifndef CHESS_ENGINE_KING_ATTACKERS_H
#define CHESS_ENGINE_KING_ATTACKERS_H

#include <cstdint>
#include "chess/core/ChessBoard.h"


class KingAttackers {
private:
    // checks whether one specific attacker on fromSquare attacks targetSquare
    static bool pieceAttacksSquare(
        const ChessBoard& board,
        int fromSquare,
        int targetSquare,
        Piece piece
    );

    // counts distinct enemy pieces attacking squares next to the king, not used by the main evaluation at the moment
    static int countUniqueKingAttackers(const ChessBoard &board, int kingSquare, bool attackerIsWhite);

public:
    // counts pieces of one type that attack a square, pawns and knights use tables, sliders walk rays
    static int countAttackers(const ChessBoard &board, int square, bool attackerIsWhite, Piece piece);

    // weighted pressure from distinct attackers hitting the king zone plus a bonus by attacker count
    static int evaluateUniqueKingAttackers(const ChessBoard &board, int kingSquare, bool attackerIsWhite);
};

#endif
