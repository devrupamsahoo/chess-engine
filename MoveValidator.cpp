//
// Created by sahoo on 23-06-2026.
//

#include "MoveValidator.h"
#include <iostream>

#include "CastlingValidator.h"
#include "PieceOwnership.h"
#include "MovementPatterns.h"
#include "PawnValidator.h"
#include "kingValidator.h"
#include "PathValidator.h"

// returns true if the input position is valid
bool MoveValidator::isValidPosition(const std::string &position) {
    // returns false if input length is not 2
    if (position.length() != 2) {
        std::cout << "Invalid position length!!" << std::endl;
        return false;
    }

    // returns false if first character is something other than a...h
    if (position[0] < 'a' || position[0] > 'h') {
        std::cout << "Invalid position format!!"<< std::endl;
        return false;
    }

    // returns false if first character is something other than 1...8
    if (position[1] < '1' || position[1] > '8') {
        std::cout << "Invalid position format!!"<< std::endl;
        return false;
    }

    return true;
}

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
    // enters same position twice
    if (startPos == endPos) {
        std::cout << "Same position!!" << std::endl;
        return false;
    }

    Piece movingPiece(board.getPieceAt(startPos));
    Piece capturedPiece(board.getPieceAt(endPos));

    // cannot capture the opponent's king
    if (capturedPiece == whiteKing || capturedPiece == blackKing) {
        std::cout << "Can,t capture king!!" << std::endl;
        return false;
    }

    // cannot capture your own piece
    if (PieceOwnership::isOwnPiece(board, endPos)) {
        std::cout << "Can't capture own piece!!" << std::endl;
        return false;
    }

    // checks move validity for pawns
    if (movingPiece == whitePawn || movingPiece == blackPawn) {
        if (!PawnValidator::isPawnMove(board, startPos, endPos)) {
            std::cout << "Not a valid pawn move!!" << std::endl;
            return false;
        }
    }

    // checks move validity for rooks
    if (movingPiece == whiteRook || movingPiece == blackRook) {
        if (!MovementPatterns::isHorizontalMove(startPos, endPos) &&
            !MovementPatterns::isVerticalMove(startPos, endPos)) {

            std::cout << "Not a valid rook move!!" << std::endl;
            return false;
        }

        if (PathValidator::isPathBlocked(board, startPos, endPos)) {
            std::cout << "Path is blocked!!" << std::endl;
            return false;
        }
    }

    // checks move validity for bishops
    if (movingPiece == whiteBishop || movingPiece == blackBishop) {
        if (!MovementPatterns::isDiagonalMove(startPos, endPos)) {
            std::cout << "Not a valid bishop move!!" << std::endl;
            return false;
        }

        if (PathValidator::isPathBlocked(board, startPos, endPos)) {
            std::cout << "Path is blocked!!" << std::endl;
            return false;
        }
    }

    // checks move validity for queens
    if (movingPiece == whiteQueen || movingPiece == blackQueen) {
        if (!MovementPatterns::isDiagonalMove(startPos, endPos) &&
            !MovementPatterns::isHorizontalMove(startPos, endPos) &&
            !MovementPatterns::isVerticalMove(startPos, endPos)) {

            std::cout << "Not a valid queen move!!" << std::endl;
            return false;
        }

        if (PathValidator::isPathBlocked(board, startPos, endPos)) {
            std::cout << "Path is blocked!!" << std::endl;
            return false;
        }
    }

    // checks move validity for knights
    if (movingPiece == whiteKnight || movingPiece == blackKnight) {
        if (!isKnightMove(startPos, endPos)) {
            std::cout << "Not a valid knight move!!" << std::endl;
            return false;
        }
    }

    // checks move validity for kings
    if (movingPiece == whiteKing || movingPiece == blackKing) {
        int startFile = startPos & 7;
        int endFile = endPos & 7;

        if (std::abs(startFile - endFile) == 2) {
            bool isWhitePlayer = (movingPiece == whiteKing);

            if (endFile > startFile) { // kingside
                if (CastlingValidator::canCastleKingside(board,isWhitePlayer)) {
                    return true;
                }
            } else { // queenside
                if (CastlingValidator::canCastleQueenside(board,isWhitePlayer)) {
                    return true;
                }
            }

            std::cout << "Illegal castling attempt!!" << std::endl;
            return false;
        }

        if (!KingValidator::isKingMove(startPos, endPos)) {
            std::cout << "Not a valid king move!!" << std::endl;
            return false;
        }
    }

    // checks if move leaves king in check
    if (KingValidator::leavesKingInCheck(board, startPos, endPos)) {
        std::cout << "Leaves king in check!! Try another move: ";
        return false;
    }

    return true;
}
