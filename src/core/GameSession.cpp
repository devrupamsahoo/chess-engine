//
// Created by devrupamsahoo on 04-11-2026.
//

#include "chess/core/GameSession.h"
#include "chess/move/KingValidator.h"
#include "chess/move/MoveValidator.h"
#include "chess/move/PawnValidator.h"
#include "chess/notation/FEN.h"

// saves the starting position as the first match snapshot
GameSession::GameSession() {
    // save the starting position as the first session snapshot
    fenSnapshots.push_back(FEN::serialize(chessBoard));
    capturedPieceSnapshots.emplace_back();
}

// returns the current board position for read-only access
const ChessBoard& GameSession::board() const {
    return chessBoard;
}

// validates and makes a move, then switches turns and records the new position
bool GameSession::makeMove(int startPos, int endPos, Piece promotion) {
    // validate before reading the source square
    if (!MoveValidator::isValidMove(chessBoard, startPos, endPos)) {
        return false;
    }

    const bool whiteToMove = chessBoard.isWhiteTurn();

    const Piece movingPiece = chessBoard.getPieceAt(startPos);

    Piece capturedPiece = chessBoard.getPieceAt(endPos);

    // records the pawn captured away from the destination during en passant
    if (capturedPiece == noPiece
        && (movingPiece == whitePawn || movingPiece == blackPawn)
        && endPos == chessBoard.getEnPassantTarget()) {
        capturedPiece = whiteToMove ? blackPawn : whitePawn;
        }

    const bool isPawn = movingPiece == whitePawn || movingPiece == blackPawn;

    const bool isPromotion = isPawn && PawnValidator::isPawnPromotion(endPos, whiteToMove);

    // validate the promotion piece for the moving side
    if (isPromotion) {
        if (promotion == noPiece) {
            promotion = whiteToMove ? whiteQueen : blackQueen;
        }

        const bool validWhitePromotion =
            promotion == whiteQueen || promotion == whiteRook ||
            promotion == whiteBishop || promotion == whiteKnight;

        const bool validBlackPromotion =
            promotion == blackQueen || promotion == blackRook ||
            promotion == blackBishop || promotion == blackKnight;

        if ((whiteToMove && !validWhitePromotion) ||
            (!whiteToMove && !validBlackPromotion)) {
            return false;
        }
    } else if (promotion != noPiece) {
        // a promotion piece is only valid when a pawn reaches its last rank
        return false;
    }

    // recognize castling only when the king moves between its exact home and castling squares
    const bool isCastling =
        (movingPiece == whiteKing && startPos == e1 &&
            (endPos == g1 || endPos == c1)) ||
        (movingPiece == blackKing && startPos == e8 &&
            (endPos == g8 || endPos == c8));

    if (isCastling) {
        if (!chessBoard.executeCastle(startPos, endPos, whiteToMove)) {
            return false;
        }
    } else {
        chessBoard.makeMove(startPos, endPos, promotion);
    }

    // record the position after the move and turn change
    chessBoard.switchTurn();

    // discard any redo branch and record the new position and captured pieces
    std::vector<Piece> nextCapturedPieces =
        capturedPieceSnapshots[currentFenIndex];

    if (capturedPiece != noPiece) {
        nextCapturedPieces.push_back(capturedPiece);
    }

    fenSnapshots.resize(currentFenIndex + 1);
    capturedPieceSnapshots.resize(currentFenIndex + 1);

    fenSnapshots.push_back(FEN::serialize(chessBoard));
    capturedPieceSnapshots.push_back(nextCapturedPieces);
    currentFenIndex = fenSnapshots.size() - 1;

    return true;
}

// returns the legal destinations for the piece on the specified square
std::vector<LegalMoveTarget> GameSession::legalMovesFrom(int startSquare) {
    std::vector<LegalMoveTarget> targets;

    if (startSquare < 0 || startSquare >= squareCount) {
        return targets;
    }

    const Piece movingPiece = chessBoard.getPieceAt(startSquare);

    if (movingPiece == noPiece ||
        !chessBoard.isValidPiece(chessBoard.isWhiteTurn(), startSquare)) {
        return targets;
        }

    const bool movingPieceIsPawn =
        movingPiece == whitePawn || movingPiece == blackPawn;

    for (int endSquare = 0; endSquare < squareCount; ++endSquare) {
        if (!MoveValidator::isValidMove(chessBoard, startSquare, endSquare)) {
            continue;
        }

        const Piece targetPiece = chessBoard.getPieceAt(endSquare);
        const bool isEnPassantCapture =
            movingPieceIsPawn &&
            endSquare == chessBoard.getEnPassantTarget() &&
            targetPiece == noPiece;

        targets.push_back({
            endSquare,
            targetPiece != noPiece || isEnPassantCapture
        });
    }

    return targets;
}

// restore the previous position and move the current snapshot back
bool GameSession::undo() {
    if (!chessBoard.canUndo() || currentFenIndex == 0) {
        return false;
    }

    chessBoard.undoMove();
    chessBoard.switchTurn();
    --currentFenIndex;

    return true;
}

// replay the next move and move the current snapshot forward
bool GameSession::redo() {
    if (!chessBoard.canRedo() || currentFenIndex + 1 >= fenSnapshots.size()) {
        return false;
    }

    chessBoard.redoMove();
    chessBoard.switchTurn();
    ++currentFenIndex;

    return true;
}

// discard the old match snapshots and start history from the initial position
void GameSession::reset() {
    chessBoard = ChessBoard{};
    fenSnapshots.clear();
    fenSnapshots.push_back(FEN::serialize(chessBoard));
    capturedPieceSnapshots.clear();
    capturedPieceSnapshots.emplace_back();
    currentFenIndex = 0;
}

// load a position from FEN and start a new match history from it
bool GameSession::loadFen(const std::string& fen) {
    if (!FEN::load(chessBoard, fen)) {
        return false;
    }

    // a loaded position starts a new session history
    fenSnapshots.clear();
    fenSnapshots.push_back(FEN::serialize(chessBoard));
    capturedPieceSnapshots.clear();
    capturedPieceSnapshots.emplace_back();
    currentFenIndex = 0;

    return true;
}

// returns the FEN string for the current position
std::string GameSession::currentFen() const {
    return fenSnapshots[currentFenIndex];
}

// returns all recorded FEN positions in the current match history
const std::vector<std::string>& GameSession::fenHistory() const {
    return fenSnapshots;
}

// returns the pieces captured in the current position
const std::vector<Piece>& GameSession::capturedPieces() const {
    return capturedPieceSnapshots[currentFenIndex];
}

// returns true if king is in check
bool GameSession::isKingInCheck(bool whitePlayer) {
    return KingValidator::isKingInCheck(chessBoard, whitePlayer);
}
