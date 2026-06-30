//
// Created by sahoo on 25-06-2026.
//

#include "PieceOwnership.h"
#include "Utils.h"

// returns true if the specified square contains an enemy piece
bool PieceOwnership::isEnemyPiece(ChessBoard &board, int square) {

    // white player black piece
    if (board.isWhiteTurn()) {
        if (Utils::checkBit(board.getBlackOccupancy(), square)) {
            return true;
        }
    }

    // black player white piece
    if (!board.isWhiteTurn()) {
        if (Utils::checkBit(board.getWhiteOccupancy(), square)) {
            return true;
        }
    }

    return false;
}

// returns true if the specified square contains the current player's own piece
bool PieceOwnership::isOwnPiece(ChessBoard &board, int square) {

    // black player black piece
    if (!board.isWhiteTurn()) {
        if (Utils::checkBit(board.getBlackOccupancy(), square)) {
            return true;
        }
    }

    // white player white piece
    if (board.isWhiteTurn()) {
        if (Utils::checkBit(board.getWhiteOccupancy(), square)) {
            return true;
        }
    }

    return false;
}

// returns true if the specified square is empty
bool PieceOwnership::isEmptySquare(ChessBoard &board, int square) {
    return !Utils::checkBit(board.getAllOccupancy(), square);
}
