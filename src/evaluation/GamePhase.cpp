//
// Created by devrupamsahoo on 10-11-2026.
//

#include "chess/evaluation/GamePhase.h"
#include <algorithm>

// counts remaining minor and major pieces, knights and bishops one, rooks two, queens four, used to blend opening and endgame values
int GamePhase::calculateGamePhase(const ChessBoard &board) {
    int phase = 0;

    for (int square = 0; square < squareCount; ++square) {
        const Piece piece = board.getPieceAt(square);

        // weight each non pawn piece by how much it marks the middlegame
        switch (piece) {
            case whiteKnight:
            case blackKnight:
            case whiteBishop:
            case blackBishop:
                phase += 1;
                break;

            case whiteRook:
            case blackRook:
                phase += 2;
                break;

            case whiteQueen:
            case blackQueen:
                phase += 4;
                break;

            default:
                break;
        }
    }

    return std::clamp(phase, 0, 24);
}
