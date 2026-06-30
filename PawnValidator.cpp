//
// Created by sahoo on 26-06-2026.
//

#include "PawnValidator.h"
#include <iostream>

#include "PieceOwnership.h"
#include "Utils.h"

// returns true if pawn is moving forward one step
bool PawnValidator::isPawnForwardMove(ChessBoard &board, int startPos, int endPos) {
    // pawn cannot move forward into an occupied square
    if (board.getPieceAt(endPos) != noPiece) {
        return false;
    }

    // white pawn forward move
    if (board.isWhiteTurn()) {
        return (startPos + 8) == endPos;
    }

    // black pawn forward move
    return (startPos - 8) == endPos;
}

// returns true if the pawn can move two squares forward from its starting rank
bool PawnValidator::isPawnInitialDoubleMove(ChessBoard &board, int startPos, int endPos) {
    // both intermediate and destination squares must be empty
    int middleSquare = board.isWhiteTurn() ? startPos + 8 : startPos - 8;

    if (Utils::checkBit(board.getAllOccupancy(), middleSquare) || Utils::checkBit(board.getAllOccupancy(), endPos)) {
        return false;
    }

    // white pawn forward double move
    if (board.isWhiteTurn()) {
        return ((startPos >> 3) == 1) && (startPos + 16 == endPos);
    }

    // black pawn forward double move
    return ((startPos >> 3) == 6) && (startPos - 16 == endPos);

}

// returns true if pawn can capture diagonally
bool PawnValidator::isPawnCaptureMove(ChessBoard &board, int startPos, int endPos) {

    Piece targetPiece = board.getPieceAt(endPos);

    // cannot capture empty square
    if (targetPiece == noPiece) {
        return false;
    }

    int startRank = startPos >> 3;
    int endRank = endPos >> 3;

    int startFile = startPos & 7;
    int endFile = endPos & 7;

    int rankDiff = endRank - startRank;
    int fileDiff = abs(endFile - startFile);

    // must move exactly one file diagonally
    if (fileDiff != 1) {
        return false;
    }

    // white pawn captures upward
    if (board.isWhiteTurn()) {
        return rankDiff == 1 && PieceOwnership::isEnemyPiece(board, endPos) && targetPiece != blackKing;
    }

    // black pawn captures downward
    return rankDiff == -1 && PieceOwnership::isEnemyPiece(board, endPos) && targetPiece != whiteKing;
}

// master pawn validator: returns true if the pawn move is legal
bool PawnValidator::isPawnMove(ChessBoard &board, int startPos, int endPos) {
    // diagonal capture
    if (isPawnCaptureMove(board, startPos, endPos)) {
        return true;
    }

    // restricting capturing piece in front of the pawn
    if (board.getPieceAt(endPos) != noPiece) {
        return false;
    }

    // en passant
    if (isEnPassantMove(board, startPos, endPos)) {
        return true;
    }

    // checks if a pawn is moving two steps in first move
    if (isPawnInitialDoubleMove(board, startPos, endPos)) {
        return true;
    }

    // checks if a pawn is moving forward
    if (isPawnForwardMove(board, startPos, endPos)) {
        return true;
    }

    return false;
}

// promotes a pawn that has reached the last rank
void PawnValidator::promotePawn(ChessBoard &board, int endPos, bool isWhitePlayer) {

    int rank = endPos >> 3;

    // white pawn promotes on 8th rank, black pawn promotes on 1st rank
    if ((isWhitePlayer && rank == 7) || (!isWhitePlayer && rank == 0)) {

        std::string promotionChoice;

        while (true) {
            std::cout << "Promote to (Q)ueen, (R)ook, (B)ishop, or (N)ight? Enter a letter: ";
            std::cin >> promotionChoice;

            char promotedPieceChar = toupper(promotionChoice[0]);

            if (promotedPieceChar == 'Q' || promotedPieceChar == 'R'
                || promotedPieceChar == 'B' || promotedPieceChar == 'N') {

                Piece promotedPiece;

                if (isWhitePlayer) {

                    // remove white pawn from promotion square
                    Utils::clearBit(board.getPieceBitboard(whitePawn), endPos);

                    if (promotedPieceChar == 'Q') {
                        promotedPiece = whiteQueen;
                    } else if (promotedPieceChar == 'R') {
                        promotedPiece = whiteRook;
                    } else if (promotedPieceChar == 'B') {
                        promotedPiece = whiteBishop;
                    } else {
                        promotedPiece = whiteKnight;
                    }

                } else {
                    // remove black pawn from promotion square
                    Utils::clearBit(board.getPieceBitboard(blackPawn), endPos);

                    if (promotedPieceChar == 'Q') {
                        promotedPiece = blackQueen;
                    } else if (promotedPieceChar == 'R') {
                        promotedPiece = blackRook;
                    } else if (promotedPieceChar == 'B') {
                        promotedPiece = blackBishop;
                    } else {
                        promotedPiece = blackKnight;
                    }
                }

                // place promoted piece on the same square
                Utils::setBit(board.getPieceBitboard(promotedPiece), endPos);

                // update occupancy after replacing pawn
                board.updateOccupancy();

                break;
            }

            std::cout << "Invalid choice!" << std::endl;
        }
    }
}

// returns true if en passant move is allowed
bool PawnValidator::isEnPassantMove(ChessBoard &board, int startPos, int endPos) {
    int enPassantSquare = board.getEnPassantTarget();

    if (enPassantSquare == -1) {
        return false;
    }

    int startRank = startPos >> 3;
    int endRank = endPos >> 3;

    int startFile = startPos & 7;
    int endFile = endPos & 7;

    int rankDiff = endRank - startRank;
    int fileDiff = std::abs(startFile - endFile);

    // // white pawn en passant
    if (board.isWhiteTurn()) {
        if (rankDiff == 1 && fileDiff == 1) {
            if (endPos == enPassantSquare) {
                return true;
            }
        }
    } else { // for black pawn en passant
        if (rankDiff == -1 && fileDiff == 1) {
            if (endPos == enPassantSquare) {
                return true;
            }
        }
    }

    return false;
}
