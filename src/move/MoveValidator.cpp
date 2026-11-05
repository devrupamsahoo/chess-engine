//
// Created by sahoo on 23-06-2026.
//

#include "chess/move/MoveValidator.h"
#include "chess/move/CastlingValidator.h"
#include "chess/move/PieceOwnership.h"
#include "chess/move/MovementPatterns.h"
#include "chess/move/PawnValidator.h"
#include "chess/move/KingValidator.h"
#include "chess/move/PathValidator.h"

// returns true if the move is a valid knight move
bool MoveValidator::isKnightMove(int startPos, int endPos) {
    int startRank = startPos >> 3;
    int startFile = startPos & 7;

    int endRank = endPos >> 3;
    int endFile = endPos & 7;

    int rankDiff = abs(startRank - endRank);
    int fileDiff = abs(startFile - endFile);

    return (rankDiff == 1 && fileDiff == 2) || (rankDiff == 2 && fileDiff == 1);
}

// master move validator function: returns true if the move is legal for the selected piece
bool MoveValidator::isValidMove(ChessBoard &board, int startPos, int endPos) {
    if (startPos < 0 || startPos >= squareCount || endPos < 0 || endPos >= squareCount) {
        return false;
    }

    // enters same position twice
    if (startPos == endPos) {
        return false;
    }

    Piece movingPiece = board.getPieceAt(startPos);

    // only allow a piece belonging to the side to move to start a move
    if (movingPiece == noPiece || !board.isValidPiece(board.isWhiteTurn(), startPos)) {
        return false;
    }

    Piece capturedPiece = board.getPieceAt(endPos);

    // cannot capture the opponent's king
    if (capturedPiece == whiteKing || capturedPiece == blackKing) {
        return false;
    }

    // cannot capture your own piece
    if (PieceOwnership::isOwnPiece(board, endPos)) {
        return false;
    }

    // checks move validity for pawns
    if (movingPiece == whitePawn || movingPiece == blackPawn) {
        if (!PawnValidator::isPawnMove(board, startPos, endPos)) {
            return false;
        }
    }

    // checks move validity for rooks
    if (movingPiece == whiteRook || movingPiece == blackRook) {
        if (!MovementPatterns::isHorizontalMove(startPos, endPos) &&
            !MovementPatterns::isVerticalMove(startPos, endPos)) {

            return false;
        }

        if (PathValidator::isPathBlocked(board, startPos, endPos)) {
            return false;
        }
    }

    // checks move validity for bishops
    if (movingPiece == whiteBishop || movingPiece == blackBishop) {
        if (!MovementPatterns::isDiagonalMove(startPos, endPos)) {
            return false;
        }

        if (PathValidator::isPathBlocked(board, startPos, endPos)) {
            return false;
        }
    }

    // checks move validity for queens
    if (movingPiece == whiteQueen || movingPiece == blackQueen) {
        if (!MovementPatterns::isDiagonalMove(startPos, endPos) &&
            !MovementPatterns::isHorizontalMove(startPos, endPos) &&
            !MovementPatterns::isVerticalMove(startPos, endPos)) {

            return false;
        }

        if (PathValidator::isPathBlocked(board, startPos, endPos)) {
            return false;
        }
    }

    // checks move validity for knights
    if (movingPiece == whiteKnight || movingPiece == blackKnight) {
        if (!isKnightMove(startPos, endPos)) {
            return false;
        }
    }

    // checks move validity for kings
    if (movingPiece == whiteKing || movingPiece == blackKing) {
        bool isWhitePlayer = movingPiece == whiteKing;

        // recognize castling only for the king's exact home-square moves
        bool isKingsideCastle = (isWhitePlayer && startPos == e1 && endPos == g1) || (!isWhitePlayer && startPos == e8 && endPos == g8);

        bool isQueensideCastle = (isWhitePlayer && startPos == e1 && endPos == c1) || (!isWhitePlayer && startPos == e8 && endPos == c8);

        if (isKingsideCastle || isQueensideCastle) {
            bool canCastle = isKingsideCastle ? CastlingValidator::canCastleKingside(board, isWhitePlayer) : CastlingValidator::canCastleQueenside(board, isWhitePlayer);

            if (canCastle) {
                return true;
            }

            return false;
        }

        // non-castling king moves must move to an adjacent square
        if (!KingValidator::isKingMove(startPos, endPos)) {
            return false;
        }
    }

    // checks if move leaves king in check
    if (KingValidator::leavesKingInCheck(board, startPos, endPos)) {
        return false;
    }

    return true;
}
