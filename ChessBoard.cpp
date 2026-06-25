//
// Created by sahoo on 23-06-2026.
//

#include "ChessBoard.h"
#include "Utils.h"
#include <iostream>

ChessBoard::ChessBoard() {
    // ensures all bits are filled with zero
    board.fill(0);

    initBoard();
}

// initialize board
void ChessBoard::initBoard() {

    // white pieces
    board[whitePawn]   = 0x000000000000FF00ULL;
    board[whiteKnight] = 0x0000000000000042ULL;
    board[whiteBishop] = 0x0000000000000024ULL;
    board[whiteRook]   = 0x0000000000000081ULL;
    board[whiteQueen]  = 0x0000000000000008ULL;
    board[whiteKing]   = 0x0000000000000010ULL;

    // black pieces
    board[blackPawn]   = 0x00FF000000000000ULL;
    board[blackKnight] = 0x4200000000000000ULL;
    board[blackBishop] = 0x2400000000000000ULL;
    board[blackRook]   = 0x8100000000000000ULL;
    board[blackQueen]  = 0x0800000000000000ULL;
    board[blackKing]   = 0x1000000000000000ULL;
}

// prints entire board
void ChessBoard::printBoard() {

    std::cout << std::endl;
    std::cout << "         BLACK" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
    std::cout << "   -----------------" << std::endl;

    for (int rank = 8; rank > 0; rank--) {
        std::cout << rank << " | ";

        for (int file = 8; file > 0; file--) {
            int square = 8*(rank-1) + (8-file);
            // empty by default
            char squareChar = '.';

            // scans 12 bitboards if a piece exists on the square
            for (int p = 0; p < pieceCount; p++) {
                // pass the specific uint64_t bitboard to checkBit
                if (Utils::checkBit(board[p], square)) {
                    squareChar = pieceMap[p];
                    break;
                }
            }
            std::cout << squareChar << " ";
        }
        std::cout << "| " << rank << std::endl;
    }

    std::cout << "   -----------------" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
    std::cout << "         WHITE" << std::endl;
}

// prints one bitboard
void ChessBoard::printBitboard(Piece piece) {
    uint64_t bitboard = board[piece];

    std::cout << std::endl;
    std::cout << "         BLACK" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
    std::cout << "   -----------------" << std::endl;

    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank+1 << " | ";

        for (int file = 0; file < 8; file++) {
            int square = 8*rank + file;

            if (Utils::checkBit(bitboard, square)) {
                std::cout << pieceMap[piece] << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "| " << rank+1 << std::endl;
    }

    std::cout << "   -----------------" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
    std::cout << "         WHITE" << std::endl;
}

// returns true if white's turn
bool ChessBoard::isWhiteTurn() {
    return  whiteTurn;
}

// switches player's turn
void ChessBoard::switchTurn() {
    whiteTurn = !whiteTurn;
}

Piece ChessBoard::getPieceAt(int square) {
    for (int i = 0; i < 12; i++) {
        if (Utils::checkBit(board[i], square)) {
            return static_cast<Piece>(i);
        }
    }
    return noPiece;
}

// returns true if the selected coordinate contains a piece belonging to the player whose turn it is
bool ChessBoard::isValidPiece(bool whitesTurn, int square) {
    Piece pieceType = getPieceAt(square);

    if (pieceType == noPiece) {
        return false;
    }

    // if currently whites turn
    if (whitesTurn) {
        return pieceType <= whiteKing;
    }

    // gor black's turn
    return pieceType >= blackPawn && pieceType <= blackKing;
}