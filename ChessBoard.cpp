//
// Created by sahoo on 23-06-2026.
//

#include "ChessBoard.h"
#include "Utils.h"
#include <iostream>

#include "CastlingValidator.h"
#include "MoveHistory.h"
#include "PieceOwnership.h"

ChessBoard::ChessBoard() {

    // ensures all bits are filled with zero
    board.fill(0);

    // initializes bitboards with pieces
    initBoard();

    // updates occupancy boards
    updateOccupancy();
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

    // king positions
    whiteKingPosition = __builtin_ctzll(board[whiteKing]);
    blackKingPosition = __builtin_ctzll(board[blackKing]);
}

// update occupancy bitboard function implementation
void ChessBoard::updateOccupancy() {
    whiteOccupancy = board[whitePawn] | board[whiteKnight] | board[whiteBishop] |
                     board[whiteRook] | board[whiteQueen] | board[whiteKing];

    blackOccupancy = board[blackPawn] | board[blackKnight] | board[blackBishop] |
                     board[blackRook] | board[blackQueen] | board[blackKing];

    allOccupancy = whiteOccupancy | blackOccupancy;
}

// returns white occupancy
uint64_t ChessBoard::getWhiteOccupancy() const {
    return whiteOccupancy;
}

// returns black occupancy
uint64_t ChessBoard::getBlackOccupancy() const {
    return blackOccupancy;
}

// returns all occupancy
uint64_t ChessBoard::getAllOccupancy() const {
    return allOccupancy;
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
void ChessBoard::printBitboard(Piece pieceType) {
    uint64_t bitboard = board[pieceType];

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

// returns true if it is White's turn
bool ChessBoard::isWhiteTurn() {
    return  whiteTurn;
}

// switches player's turn
void ChessBoard::switchTurn() {
    whiteTurn = !whiteTurn;
}

// returns the piece occupying the specified square
Piece ChessBoard::getPieceAt(int square) {

    for (int i = 0; i < 12; i++) {
        if (Utils::checkBit(board[i], square)) {
            return static_cast<Piece>(i);
        }
    }

    return noPiece;
}

// returns true if the selected coordinate contains a piece belonging to the player whose turn it is
bool ChessBoard::isValidPiece(bool isWhitePlayer, int square) {
    Piece pieceType = getPieceAt(square);

    if (pieceType == noPiece) {
        return false;
    }

    // if currently whites turn
    if (isWhitePlayer) {
        return pieceType <= whiteKing;
    }

    // for black's turn
    return pieceType >= blackPawn && pieceType <= blackKing;
}

// moves a piece from start to end and replaces the origin square with empty space
void ChessBoard::movePiece(int startPos, int endPos) {
    Piece movingPiece = getPieceAt(startPos);
    Piece capturedPiece = getPieceAt(endPos);

    // remove the captured piece from its bitboard
    if (capturedPiece != noPiece) {
        Utils::clearBit(board[capturedPiece] ,endPos);
    }

    // move the piece to its new position
    Utils::clearBit(board[movingPiece], startPos);
    Utils::setBit(board[movingPiece], endPos);

    // keep track of the king's current square
    if (movingPiece == whiteKing || movingPiece == blackKing) {
        Piece pieceType = (isWhiteTurn()) ? whiteKing : blackKing;
        updateKingPosition();
    }

    // update occupancy boards
    updateOccupancy();
}

// returns the bitboard for the specified piece type
uint64_t& ChessBoard::getPieceBitboard(Piece pieceType) {
    return board[pieceType];
}

// returns the read-only bitboard for the specified piece type
const uint64_t& ChessBoard::getPieceBitboard(Piece pieceType) const {
    return board[pieceType];
}

// returns the king position
int ChessBoard::getKingPos(bool isWhiteKing) {

    uint64_t kingBitboard = isWhiteKing
        ? getPieceBitboard(whiteKing)
        : getPieceBitboard(blackKing);

    if (kingBitboard == 0) {
        throw std::runtime_error("King missing from the board!!");
    }

    return __builtin_ctzll(kingBitboard);
}

// updates king position
void ChessBoard::updateKingPosition() {
    if (board[whiteKing] == 0 || board[blackKing] == 0) {
        throw std::runtime_error("King bitboard is empty!");
    }

    whiteKingPosition = __builtin_ctzll(board[whiteKing]);

    blackKingPosition = __builtin_ctzll(board[blackKing]);
}

// makes a move and stores the history
void ChessBoard::makeMove(int startPos, int endPos) {
    Move move;

    move.startSquare = startPos;
    move.endSquare = endPos;

    move.movedPiece = getPieceAt(startPos);
    move.capturedPiece = getPieceAt(endPos);

    // save current castling state
    move.oldCastlingRights = castlingRights;

    // save previous en passant target
    move.oldEnPassantTarget = enPassantTarget;

    // execute en passant capture
    if ((move.movedPiece == whitePawn || move.movedPiece == blackPawn)
         && endPos == enPassantTarget) {

        move.isEnPassantCapture = true;

        int capturedPawnPos = isWhiteTurn() ? endPos - 8 : endPos + 8;
        move.capturedPiece = getPieceAt(capturedPawnPos);

        Utils::clearBit(getPieceBitboard(move.capturedPiece), capturedPawnPos);
    }

    // set new en passant target for initial double move
    if ((move.movedPiece == whitePawn || move.movedPiece == blackPawn)
        && std::abs(startPos - endPos) == 16) {

        enPassantTarget = isWhiteTurn() ? startPos + 8 : startPos - 8;
    } else {
        enPassantTarget = -1;
    }

    // save move for undo
    moveHistory.pushMove(move);

    // updating castling rights
    Piece p = move.movedPiece;

    // king moves - both side lose rights
    if (p == whiteKing) {
        castlingRights.wk = false;
        castlingRights.wq = false;
    } else if (p == blackKing) {
        castlingRights.bk = false;
        castlingRights.bq = false;
    }

    // rook moves lose specific side rights
    if (p == whiteRook) {
        if (startPos == h1) {
            castlingRights.wk = false;
        }
        if (startPos == a1) {
            castlingRights.wq = false;
        }
    }

    if (p == blackRook) {
        if (startPos == h8) {
            castlingRights.bk = false;
        }
        if (startPos == a8) {
            castlingRights.bq = false;
        }
    }

    // rook captured also remove rights
    if (move.capturedPiece == whiteRook) {
        if (endPos == h1) {
            castlingRights.wk = false;
        }
        if (endPos == a1) {
            castlingRights.wq = false;
        }
    }

    if (move.capturedPiece == blackRook) {
        if (endPos == h8) {
            castlingRights.bk = false;
        }
        if (endPos == a8) {
            castlingRights.bq = false;
        }
    }

    // remove moving piece from source
    Utils::clearBit(getPieceBitboard(move.movedPiece), startPos);

    // remove captured piece
    if (move.capturedPiece != noPiece) {
        Utils::clearBit(getPieceBitboard(move.capturedPiece), endPos);
    }

    // place moving piece
    Utils::setBit(getPieceBitboard(move.movedPiece), endPos);

    // update cached king position
    if (move.movedPiece == whiteKing || move.movedPiece == blackKing) {
        updateKingPosition();
    }

    // refresh occupancies
    updateOccupancy();
}

// undo a move and delete its history
void ChessBoard::undoMove() {
    // pops the last move from stack
    Move move = moveHistory.popMove();

    // restore castling rights
    castlingRights = move.oldCastlingRights;

    // restore old en passant target
    enPassantTarget = move.oldEnPassantTarget;

    // remove moved piece from destination
    Utils::clearBit(getPieceBitboard(move.movedPiece), move.endSquare);

    // restore moved piece
    Utils::setBit(getPieceBitboard(move.movedPiece), move.startSquare);

    // restore captured piece
    if (move.isEnPassantCapture) {
        // correctly restore the captured piece, not the moved piece
        int capturedPawnPos = (move.movedPiece == whitePawn) ? move.endSquare - 8 : move.endSquare + 8;
        Utils::setBit(getPieceBitboard(move.capturedPiece), capturedPawnPos);
    } else if (move.capturedPiece != noPiece) {
        Utils::setBit(getPieceBitboard(move.capturedPiece), move.endSquare);
    }

    // undo castling move
    if (move.castlingType == whiteKingside) {
        Utils::clearBit(getPieceBitboard(whiteRook), f1);
        Utils::setBit(getPieceBitboard(whiteRook), h1);
    } else if (move.castlingType == whiteQueenside) {
        Utils::clearBit(getPieceBitboard(whiteRook), d1);
        Utils::setBit(getPieceBitboard(whiteRook), a1);
    } else if (move.castlingType == blackKingside) {
        Utils::clearBit(getPieceBitboard(blackRook), f8);
        Utils::setBit(getPieceBitboard(blackRook), h8);
    } else if (move.castlingType == blackQueenside) {
        Utils::clearBit(getPieceBitboard(blackRook), d8);
        Utils::setBit(getPieceBitboard(blackRook), a8);
    }

    // restore king position
    if (move.movedPiece == whiteKing || move.movedPiece == blackKing) {
        updateKingPosition();
    }

    // refresh occupancies
    updateOccupancy();
}

// returns the current castling rights
const CastlingRights& ChessBoard::getCastlingRights() const {
    return castlingRights;
}

// executes a castling move
void ChessBoard::executeCastle(int startPos, int endPos, bool isWhitePlayer) {
    Move move;

    move.startSquare = startPos;
    move.endSquare = endPos;

    move.movedPiece = getPieceAt(startPos);

    // save current castling state
    move.oldCastlingRights = castlingRights;

    // whiteside
    if (isWhitePlayer) {
        // king start must be e1
        if (startPos != e1) return;

        // kingside
        if (endPos == g1) {
            if (!CastlingValidator::canCastleKingside(*this, true)) {
                return;
            }

            move.capturedPiece = getPieceAt(h1);
            move.castlingType = whiteKingside;

            movePiece(e1, g1);  // king
            movePiece(h1, f1);  // rook
        }

        // queenside
        else if (endPos == c1) {
            if (!CastlingValidator::canCastleQueenside(*this, true)) {
                return;
            }

            move.capturedPiece = getPieceAt(a1);
            move.castlingType = whiteQueenside;

            movePiece(e1, c1);  // king
            movePiece(a1, d1);  // rook
        }
    } else { // black side
        if (startPos != e8) {
            return;
        }

        // kingside
        if (endPos == g8) {
            if (!CastlingValidator::canCastleKingside(*this, false)) {
                return;
            }

            move.capturedPiece = getPieceAt(h8);
            move.castlingType = blackKingside;

            movePiece(e8, g8);
            movePiece(h8, f8);
        }

        // queenside
        else if (endPos == c8) {
            if (!CastlingValidator::canCastleQueenside(*this, false)) {
                return;
            }

            move.capturedPiece = getPieceAt(a8);
            move.castlingType = blackQueenside;

            movePiece(e8, c8);
            movePiece(a8, d8);
        }
    }

    // save move for undo
    moveHistory.pushMove(move);
}

// returns the current en passant target square
int ChessBoard::getEnPassantTarget() const {
    return enPassantTarget;
}

