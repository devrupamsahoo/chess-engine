//
// Created by devrupamsahoo on 04-11-2026.
//

#include "chess/core/GameSession.h"
#include "chess/move/KingValidator.h"
#include "chess/move/PawnValidator.h"
#include "chess/notation/FEN.h"
#include "chess/movegen/MoveGeneration.h"
#include <algorithm>
#include <sstream>

// saves the starting position as the first match snapshot
GameSession::GameSession() {
    // save the starting position as the first session snapshot
    fenSnapshots.push_back(FEN::serialize(chessBoard));
    capturedPieceSnapshots.emplace_back();

    refreshLegalMoves();
}

// returns the current board position for read-only access
const ChessBoard &GameSession::board() const {
    return chessBoard;
}

// validates and makes a move, then switches turns and records the new position
bool GameSession::makeMove(int startPos, int endPos, Piece promotion) {
    // validate before reading the source square
    const auto &legalMoves = currentLegalMoves;

    const auto legalMove = std::find_if(
        legalMoves.begin(),
        legalMoves.end(),
        [&](const PseudoLegalMove &move) {
            if (move.startSquare != startPos ||
                move.endSquare != endPos) {
                return false;
            }

            // if a promotion piece was supplied, it must match the generated promotion candidate.
            if (promotion != noPiece &&
                move.promotion != promotion) {
                return false;
            }

            return true;
        });

    if (legalMove == legalMoves.end()) {
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

    refreshLegalMoves();

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
std::vector<LegalMoveTarget>
GameSession::legalMovesFrom(int startSquare) {
    std::vector<LegalMoveTarget> targets;

    if (startSquare < 0 || startSquare >= squareCount) {
        return targets;
    }

    const auto &legalMoves = this->legalMoves();

    for (const PseudoLegalMove &move: legalMoves) {
        if (move.startSquare != startSquare) {
            continue;
        }

        const Piece targetPiece =
                chessBoard.getPieceAt(move.endSquare);

        const bool isCapture =
                targetPiece != noPiece ||
                move.isEnPassant;

        const bool alreadyAdded =
                std::any_of(
                    targets.begin(),
                    targets.end(),
                    [&](const LegalMoveTarget &target) {
                        return target.square == move.endSquare;
                    });

        if (!alreadyAdded) {
            targets.push_back({
                move.endSquare,
                isCapture
            });
        }
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

    refreshLegalMoves();

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

    refreshLegalMoves();

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
    refreshLegalMoves();
}

// load a position from FEN and start a new match history from it
bool GameSession::loadFen(const std::string &fen) {
    if (!FEN::load(chessBoard, fen)) {
        return false;
    }

    // a loaded position starts a new session history
    fenSnapshots.clear();
    fenSnapshots.push_back(FEN::serialize(chessBoard));
    capturedPieceSnapshots.clear();
    capturedPieceSnapshots.emplace_back();
    currentFenIndex = 0;
    refreshLegalMoves();

    return true;
}

// returns the FEN string for the current position
std::string GameSession::currentFen() const {
    return fenSnapshots[currentFenIndex];
}

// returns all recorded FEN positions in the current match history
const std::vector<std::string> &GameSession::fenHistory() const {
    return fenSnapshots;
}

// returns the pieces captured in the current position
const std::vector<Piece> &GameSession::capturedPieces() const {
    return capturedPieceSnapshots[currentFenIndex];
}

// returns true if king is in check
bool GameSession::isKingInCheck(bool whitePlayer) const {
    return KingValidator::isKingInCheck(chessBoard, whitePlayer);
}

void GameSession::refreshLegalMoves() {
    currentLegalMoves =
            MoveGeneration::generateLegalMoves(chessBoard);
}

const std::vector<PseudoLegalMove> &GameSession::legalMoves() const {
    return currentLegalMoves;
}

GameStatus GameSession::gameStatus() const {
    const bool sideToMoveIsWhite =
            chessBoard.isWhiteTurn();

    const bool sideToMoveIsInCheck =
            KingValidator::isKingInCheck(
                chessBoard,
                sideToMoveIsWhite);

    // no legal moves means either checkmate or stalemate
    if (currentLegalMoves.empty()) {
        if (sideToMoveIsInCheck) {
            return GameStatus::checkmate;
        }

        return GameStatus::stalemate;
    }

    // one hundred halfmoves equals fifty moves without a pawn move or capture
    if (chessBoard.getHalfmoveClock() >= 100 ||
        isThreefoldRepetition() ||
        isInsufficientMaterial()) {
        return GameStatus::draw;
    }

    // legal moves remain, but the current king is attacked
    if (sideToMoveIsInCheck) {
        return GameStatus::check;
    }

    return GameStatus::ongoing;
}

// returns the part of a FEN that identifies the chess position
static std::string positionKey(const std::string &fen) {
    std::istringstream stream(fen);

    std::string placement;
    std::string activeColor;
    std::string castlingRights;
    std::string enPassantTarget;

    stream >>
            placement >>
            activeColor >>
            castlingRights >>
            enPassantTarget;

    return placement + " " +
           activeColor + " " +
           castlingRights + " " +
           enPassantTarget;
}

// checks whether the current position has appeared at least three times
bool GameSession::isThreefoldRepetition() const {
    const std::string currentPosition =
            positionKey(currentFen());

    int occurrenceCount = 0;

    for (const std::string &fen: fenSnapshots) {
        if (positionKey(fen) == currentPosition) {
            ++occurrenceCount;
        }
    }

    return occurrenceCount >= 3;
}

// checks whether neither side has enough material to force checkmate
bool GameSession::isInsufficientMaterial() const {
    int minorPieceCount = 0;

    bool hasKnight = false;
    bool bishopsOnSameColour = true;
    int firstBishopSquareColour = -1;

    for (int square = 0; square < squareCount; ++square) {
        const Piece piece = chessBoard.getPieceAt(square);

        // a pawn, rook, or queen means checkmate material still exists
        if (piece == whitePawn ||
            piece == blackPawn ||
            piece == whiteRook ||
            piece == blackRook ||
            piece == whiteQueen ||
            piece == blackQueen) {
            return false;
            }

        // remember whether every bishop stands on the same square colour
        if (piece == whiteBishop || piece == blackBishop) {
            ++minorPieceCount;

            const int file = square % 8;
            const int rank = square / 8;
            const int bishopSquareColour = (file + rank) % 2;

            if (firstBishopSquareColour == -1) {
                firstBishopSquareColour = bishopSquareColour;
            } else if (bishopSquareColour != firstBishopSquareColour) {
                bishopsOnSameColour = false;
            }
        }

        // knights prevent the same-colour-bishops rule from applying
        if (piece == whiteKnight || piece == blackKnight) {
            ++minorPieceCount;
            hasKnight = true;
        }
    }

    // king versus king, king and one minor piece versus king
    if (minorPieceCount <= 1) {
        return true;
    }

    // bishops restricted to one square colour can never create checkmate
    return !hasKnight && bishopsOnSameColour;
}
