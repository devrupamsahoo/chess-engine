//
// Created by sahoo on 23-06-2026.
//

#include "chess/core/ChessBoard.h"
#include "chess/core/Utils.h"
#include "chess/move/CastlingValidator.h"
#include "chess/history/MoveHistory.h"
#include "chess/move/PieceOwnership.h"
#include <iostream>
#include <charconv>
#include <functional>
#include <sstream>
#include <system_error>
#include <bit>

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



// returns true if it is White's turn
bool ChessBoard::isWhiteTurn() const {
    return  whiteTurn;
}

// switches player's turn
void ChessBoard::switchTurn() {
    whiteTurn = !whiteTurn;
}

// returns the piece occupying the specified square
Piece ChessBoard::getPieceAt(int square) const {

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
int ChessBoard::getKingPos(bool isWhiteKing) const {

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

// apply a move and record the state needed to undo or redo it
void ChessBoard::makeMove(int startPos, int endPos, Piece promotedPiece, bool isSimulation) {
    Move move;

    move.startSquare = startPos;
    move.endSquare = endPos;

    move.movedPiece = getPieceAt(startPos);
    move.capturedPiece = getPieceAt(endPos);

    // save current castling state
    move.oldCastlingRights = castlingRights;

    // save previous en passant target
    move.oldEnPassantTarget = enPassantTarget;

    move.oldHalfmoveClock = halfmoveClock;
    move.oldFullmoveNumber = fullmoveNumber;

    // reset the halfmove clock after a pawn move or capture; otherwise increment it
    if (move.movedPiece == whitePawn || move.movedPiece == blackPawn || move.capturedPiece != noPiece) {
        halfmoveClock = 0;
    } else {
        ++halfmoveClock;
    }

    // FEN advances the fullmove number after Black's move
    if (!whiteTurn) {
        ++fullmoveNumber;
    }

    // retain the promoted piece so undo can restore the pawn and redo can restore the promotion
    move.promotedPiece = promotedPiece;

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
    Piece pieceToPlace = (promotedPiece == noPiece) ? move.movedPiece : promotedPiece;

    Utils::setBit(getPieceBitboard(pieceToPlace), endPos);

    // update cached king position
    if (move.movedPiece == whiteKing || move.movedPiece == blackKing) {
        updateKingPosition();
    }

    // refresh occupancies
    updateOccupancy();

    // save the resulting rule state so redo can restore it exactly
    move.newCastlingRights = castlingRights;
    move.newEnPassantTarget = enPassantTarget;

    move.newHalfmoveClock = halfmoveClock;
    move.newFullmoveNumber = fullmoveNumber;

    // simulated moves must not clear the redo branch
    moveHistory.pushMove(move, !isSimulation);
}

// restore the previous position and save the move for redo when this is a real undo
void ChessBoard::undoMove(bool isSimulation) {
    // remove the latest move from history so its saved state can be restored
    Move move = moveHistory.popMove();

    // legality checks undo a temporary move without adding it to redo history
    if (!isSimulation) {
        moveHistory.pushRedoMove(move);
    }

    // restore castling rights
    castlingRights = move.oldCastlingRights;

    // restore old en passant target
    enPassantTarget = move.oldEnPassantTarget;

    // restore the FEN move counters to their values before the move
    halfmoveClock = move.oldHalfmoveClock;
    fullmoveNumber = move.oldFullmoveNumber;

    // a promoted pawn ends the move as a different piece than it started.
    Piece pieceAtDestination = move.promotedPiece != noPiece ? move.promotedPiece : move.movedPiece;

    // remove moved piece from destination
    Utils::clearBit(getPieceBitboard(pieceAtDestination), move.endSquare);

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

// report whether the move history contains a move to undo
bool ChessBoard::canUndo() const {
    return moveHistory.canUndo();
}

// report whether the redo history contains a move to replay
bool ChessBoard::canRedo() const {
    return moveHistory.canRedo();
}

// performs redo
void ChessBoard::redoMove() {
    Move move = moveHistory.popRedoMove();

    // replay castling by moving both the king and its rook
    if (move.castlingType == whiteKingside) {
        movePiece(e1, g1);
        movePiece(h1, f1);
    } else if (move.castlingType == whiteQueenside) {
        movePiece(e1, c1);
        movePiece(a1, d1);
    } else if (move.castlingType == blackKingside) {
        movePiece(e8, g8);
        movePiece(h8, f8);
    } else if (move.castlingType == blackQueenside) {
        movePiece(e8, c8);
        movePiece(a8, d8);
    } else {
        // en passant removes the captured pawn from the square behind the destination
        if (move.isEnPassantCapture) {
            int capturedPawnPos = (move.movedPiece == whitePawn) ? move.endSquare - 8 : move.endSquare + 8;

            Utils::clearBit(getPieceBitboard(move.capturedPiece), capturedPawnPos);
        } else if (move.capturedPiece != noPiece) {
            Utils::clearBit(getPieceBitboard(move.capturedPiece), move.endSquare);
        }

        // move the piece and place its promoted type when the saved move was a promotion
        Utils::clearBit(getPieceBitboard(move.movedPiece), move.startSquare);

        Piece pieceToPlace = move.promotedPiece != noPiece ? move.promotedPiece : move.movedPiece;
        Utils::setBit(getPieceBitboard(pieceToPlace), move.endSquare);

        if (move.movedPiece == whiteKing || move.movedPiece == blackKing) {
            updateKingPosition();
        }

        updateOccupancy();
    }

    // restore castling, en passant, and move-counter state from after the original move
    castlingRights = move.newCastlingRights;
    enPassantTarget = move.newEnPassantTarget;

    halfmoveClock = move.newHalfmoveClock;
    fullmoveNumber = move.newFullmoveNumber;

    moveHistory.pushRedoneMove(move);
}

// returns the current castling rights
const CastlingRights& ChessBoard::getCastlingRights() const {
    return castlingRights;
}

// validate and execute castling, then save the resulting position state for undo and redo
bool ChessBoard::executeCastle(int startPos, int endPos, bool isWhitePlayer) {
    Move move;

    move.startSquare = startPos;
    move.endSquare = endPos;

    move.movedPiece = getPieceAt(startPos);
    move.capturedPiece = noPiece;

    // save current castling state
    move.oldCastlingRights = castlingRights;
    move.oldEnPassantTarget = enPassantTarget;

    move.oldHalfmoveClock = halfmoveClock;
    move.oldFullmoveNumber = fullmoveNumber;

    // whiteside
    if (isWhitePlayer) {
        // king start must be e1
        if (startPos != e1 || getPieceAt(startPos) != whiteKing) {
            return false;
        }

        // kingside
        if (endPos == g1) {
            if (!CastlingValidator::canCastleKingside(*this, true)) {
                return false;
            }

            // require the rook to be present before moving it
            if (getPieceAt(h1) != whiteRook) {
                return false;
            }

            move.castlingType = whiteKingside;

            movePiece(e1, g1);  // king
            movePiece(h1, f1);  // rook
        }

        // queenside
        else if (endPos == c1) {
            if (!CastlingValidator::canCastleQueenside(*this, true)) {
                return false;
            }

            if (getPieceAt(a1) != whiteRook) {
                return false;
            }

            move.castlingType = whiteQueenside;

            movePiece(e1, c1);  // king
            movePiece(a1, d1);  // rook
        }else {
            return false;
        }
    } else {
        // black side
        if (startPos != e8 || getPieceAt(startPos) != blackKing) {
            return false;
        }

        // kingside
        if (endPos == g8) {
            if (!CastlingValidator::canCastleKingside(*this, false)) {
                return false;
            }

            if (getPieceAt(h8) != blackRook) {
                return false;
            }

            move.castlingType = blackKingside;

            movePiece(e8, g8);
            movePiece(h8, f8);
        }

        // queenside
        else if (endPos == c8) {
            if (!CastlingValidator::canCastleQueenside(*this, false)) {
                return false;
            }

            if (getPieceAt(a8) != blackRook) {
                return false;
            }

            move.castlingType = blackQueenside;

            movePiece(e8, c8);
            movePiece(a8, d8);
        }else {
            return false;
        }
    }

    // castling uses this turn, so any en passant opportunity expires.
    enPassantTarget = -1;

    // clearing castling rights
    if (isWhitePlayer) {
        castlingRights.wk = false;
        castlingRights.wq = false;
    } else {
        castlingRights.bk = false;
        castlingRights.bq = false;
    }

    move.newCastlingRights = castlingRights;

    // castling is a king move without a capture, so increment the halfmove clock
    ++halfmoveClock;

    if (!isWhitePlayer) {
        ++fullmoveNumber;
    }

    move.newEnPassantTarget = enPassantTarget;

    move.newHalfmoveClock = halfmoveClock;
    move.newFullmoveNumber = fullmoveNumber;

    // save move for undo
    moveHistory.pushMove(move);
    return true;
}

// returns the current en passant target square
int ChessBoard::getEnPassantTarget() const {
    return enPassantTarget;
}
