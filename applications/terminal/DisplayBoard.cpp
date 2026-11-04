//
// Created by devrupamsahoo on 03-11-2026.
//

#include "DisplayBoard.h"
#include <iostream>
#include "chess/core/Enums.h"
#include "chess/core/Utils.h"
#include "chess/core/ChessBoard.h"

// print the position from black's side, with rank 8 at the top and file a at the left
void DisplayBoard::printBoard(const ChessBoard& board) {

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
                if (Utils::checkBit(board.getPieceBitboard(static_cast<Piece>(p)), square)) {
                    squareChar = pieceToCharMap[p];
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
void DisplayBoard::printBitboard(const ChessBoard& board, Piece pieceType) {
    uint64_t bitboard = board.getPieceBitboard(pieceType);

    std::cout << std::endl;
    std::cout << "         BLACK" << std::endl;
    std::cout << "    a b c d e f g h" << std::endl;
    std::cout << "   -----------------" << std::endl;

    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank+1 << " | ";

        for (int file = 0; file < 8; file++) {
            int square = 8*rank + file;

            if (Utils::checkBit(bitboard, square)) {
                std::cout << pieceToCharMap[pieceType] << " ";
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
