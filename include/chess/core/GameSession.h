//
// Created by devrupamsahoo on 04-11-2026.
//

#ifndef CHESS_ENGINE_GAMESESSION_H
#define CHESS_ENGINE_GAMESESSION_H

#include "chess/core/ChessBoard.h"

#include <cstddef>
#include <string>
#include <vector>

// describes a legal destination and whether the move captures a piece
struct LegalMoveTarget {
    int square;
    bool isCapture;
};

class GameSession {
public:
    // returns the current board position for read-only access
    const ChessBoard& board() const;

    // validates and makes a move, then switches turns and records the new position
    bool makeMove(int startPos, int endPos, Piece promotion = noPiece);

    // restores the previous move and position
    bool undo();

    // replays the next undone move and position
    bool redo();

    // resets the board and match history to the starting position
    void reset();

    // loads a valid FEN position and starts a new match history
    bool loadFen(const std::string& fen);

    // returns the FEN string for the current position
    std::string currentFen() const;

    // returns all recorded FEN positions in the current match history
    const std::vector<std::string>& fenHistory() const;

    // saves the starting position as the first match snapshot
    GameSession();

    // returns true if king is in check
    bool isKingInCheck(bool whitePlayer);

    // returns the legal destinations for the piece on the specified square
    std::vector<LegalMoveTarget> legalMovesFrom(int startSquare);

    // returns the pieces captured in the current position
    const std::vector<Piece>& capturedPieces() const;

private:
    // creates chessboard object
    ChessBoard chessBoard;

    // stores one FEN snapshot for each position reached in the current match
    std::vector<std::string> fenSnapshots;

    // identifies the snapshot currently shown after undo or redo
    std::size_t currentFenIndex = 0;

    // stores captured pieces for each recorded position
    std::vector<std::vector<Piece>> capturedPieceSnapshots;
};

#endif //CHESS_ENGINE_GAMESESSION_H
