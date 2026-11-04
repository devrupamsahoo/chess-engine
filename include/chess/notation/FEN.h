//
// Created by devrupamsahoo on 03-11-2026.
//

#ifndef CHESS_ENGINE_FEN_H
#define CHESS_ENGINE_FEN_H

#include <string>

class ChessBoard;

class FEN {
public:
    // converts a board position to a FEN string
    static std::string serialize(const ChessBoard& board);

    // loads a valid FEN position into the board
    static bool load(ChessBoard& board, const std::string& fen);
};

#endif // CHESS_ENGINE_FEN_H
