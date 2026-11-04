//
// Created by sahoo on 23-06-2026.
//

#include <iostream>

#include "chess/move/MoveValidator.h"
#include "chess/move/CastlingValidator.h"
#include "chess/move/PieceOwnership.h"
#include "chess/move/MovementPatterns.h"
#include "chess/move/PawnValidator.h"
#include "chess/move/KingValidator.h"
#include "chess/move/PathValidator.h"

// returns true if the input position is valid
bool MoveValidator::isValidPosition(const std::string &position) {

    // validates standard 4-character moves eg. e2e4
    if (position.length() == 4) {
        if (position[0] >= 'a' && position[0] <= 'h' &&
            position[2] >= 'a' && position[2] <= 'h') {

            if (position[1] >= '1' && position[1] <= '8' &&
                position[3] >= '1' && position[3] <= '8') {
                return true;
                }
            }
    }

    // validates 5-character promotion moves eg. e7e8q
    if (position.length() == 5) {
        if (position[0] >= 'a' && position[0] <= 'h' && position[2] >= 'a' && position[2] <= 'h') {

            if (position[1] >= '1' && position[1] <= '8' && position[3] >= '1' && position[3] <= '8') {

                char p = static_cast<char>(std::tolower(static_cast<unsigned char>(position[4])));

                if (p == 'q' || p == 'r' || p == 'n' || p == 'b') {
                    return true;
                }
            }
        }
    }

    std::cout << "Not a valid position format!!" << std::endl;
    return false;
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
    if (startPos < 0 || startPos >= squareCount || endPos < 0 || endPos >= squareCount) {
        return false;
    }

    // enters same position twice
    if (startPos == endPos) {
        std::cout << "Same position!!" << std::endl;
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
        bool isWhitePlayer = movingPiece == whiteKing;

        // recognize castling only for the king's exact home-square moves
        bool isKingsideCastle = (isWhitePlayer && startPos == e1 && endPos == g1) || (!isWhitePlayer && startPos == e8 && endPos == g8);

        bool isQueensideCastle = (isWhitePlayer && startPos == e1 && endPos == c1) || (!isWhitePlayer && startPos == e8 && endPos == c8);

        if (isKingsideCastle || isQueensideCastle) {
            bool canCastle = isKingsideCastle ? CastlingValidator::canCastleKingside(board, isWhitePlayer) : CastlingValidator::canCastleQueenside(board, isWhitePlayer);

            if (canCastle) {
                return true;
            }

            std::cout << "Illegal castling attempt!!" << std::endl;
            return false;
        }

        // non-castling king moves must move to an adjacent square
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
