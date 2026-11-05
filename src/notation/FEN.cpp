//
// Created by devrupamsahoo on 03-11-2026.
//

#include "chess/notation/FEN.h"
#include "chess/core/ChessBoard.h"
#include "chess/core/Utils.h"
#include "chess/move/KingValidator.h"

#include <bit>
#include <charconv>
#include <functional>
#include <sstream>
#include <system_error>

// serialize the current board state into the six FEN fields
std::string FEN::serialize(const ChessBoard &board) {
    std::string fen;

    // piece placement, starting from rank 8 and moving down to rank 1
    for (int rank = 7; rank >= 0; --rank) {
        int emptySquares = 0;

        // read each rank from file a to file h
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;

            Piece piece = noPiece;

            // find which piece occupies this square
            for (int p = 0; p < pieceCount; ++p) {
                if (Utils::checkBit(board.board[p], square)) {
                    piece = static_cast<Piece>(p);
                    break;
                }
            }

            // count consecutive empty squares so they can be written as one digit
            if (piece == noPiece) {
                emptySquares++;
            } else {
                // write any empty squares remaining at the end of this rank
                if (emptySquares > 0) {
                    fen += std::to_string(emptySquares);
                    emptySquares = 0;
                }

                fen += pieceToCharMap[piece];
            }
        }

        if (emptySquares > 0) {
            fen += std::to_string(emptySquares);
        }

        if (rank > 0) {
            fen += '/';
        }
    }

    // record which side is to move
    fen += board.whiteTurn ? " w " : " b ";

    // write available castling rights in FEN order, or '-' if none remain
    bool hasCastlingRight = false;

    if (board.castlingRights.wk) {
        fen += 'K';
        hasCastlingRight = true;
    }
    if (board.castlingRights.wq) {
        fen += 'Q';
        hasCastlingRight = true;
    }
    if (board.castlingRights.bk) {
        fen += 'k';
        hasCastlingRight = true;
    }
    if (board.castlingRights.bq) {
        fen += 'q';
        hasCastlingRight = true;
    }

    if (!hasCastlingRight) {
        fen += '-';
    }

    fen += ' ';

    // write the en passant target square, or '-' when no target is available
    if (board.enPassantTarget == -1) {
        fen += '-';
    } else {
        fen += static_cast<char>('a' + board.enPassantTarget % 8);
        fen += static_cast<char>('1' + board.enPassantTarget / 8);
    }

    // write the halfmove clock and fullmove number
    fen += ' ';
    fen += std::to_string(board.halfmoveClock);

    fen += ' ';
    fen += std::to_string(board.fullmoveNumber);

    return fen;
}

// parse and validate a FEN string before replacing the current board state
bool FEN::load(ChessBoard &board, const std::string &fen) {
    std::istringstream fenStream(fen);

    std::string placement;
    std::string activeColor;
    std::string castling;
    std::string enPassant;
    std::string halfmoveText;
    std::string fullmoveText;
    std::string extra;

    // FEN must contain exactly six fields
    if (!(fenStream >> placement >> activeColor >> castling >> enPassant >> halfmoveText >> fullmoveText) || (
            fenStream >> extra)) {
        return false;
    }

    int parsedHalfmoveClock = 0;
    int parsedFullmoveNumber = 0;

    // parse each move counter completely and reject invalid or out-of-range values
    std::function parseNumber = [](const std::string &text, int &value) {
        const char *first = text.data();
        const char *last = first + text.size();
        const std::from_chars_result result = std::from_chars(first, last, value);

        return result.ec == std::errc{} && result.ptr == last;
    };

    // the halfmove clock cannot be negative and the fullmove number must start at 1
    if (!parseNumber(halfmoveText, parsedHalfmoveClock) || !parseNumber(fullmoveText, parsedFullmoveNumber) ||
        parsedHalfmoveClock < 0 || parsedFullmoveNumber < 1) {
        return false;
    }

    // build the position separately so invalid FEN cannot partially change the board
    std::array<uint64_t, pieceCount> parsedBoard{};
    int rank = 7;
    int file = 0;
    bool previousWasDigit = false;

    // read the placement field from rank 8 down to rank 1, tracking the next file on each rank
    for (char symbol: placement) {
        // each rank separator is valid only after exactly eight squares were described
        if (symbol == '/') {
            if (file != 8 || rank == 0) {
                return false;
            }

            --rank;
            file = 0;
            previousWasDigit = false;
            continue;
        }

        // a digit represents that many consecutive empty squares
        if (symbol >= '1' && symbol <= '8') {
            // consecutive digits would split one run of empty squares into an invalid form
            if (previousWasDigit) {
                return false;
            }

            file += symbol - '0';

            if (file > 8) {
                return false;
            }

            previousWasDigit = true;

            continue;
        }

        // convert the FEN piece letter to its piece bitboard index
        std::size_t pieceIndex = pieceToCharMap.find(symbol);

        if (pieceIndex == std::string_view::npos || file >= 8) {
            return false;
        }

        // pawns cannot start on a promotion rank in a loaded position
        if ((pieceIndex == whitePawn || pieceIndex == blackPawn) && (rank == 0 || rank == 7)) {
            return false;
        }

        Utils::setBit(parsedBoard[pieceIndex], rank * 8 + file);
        ++file;

        previousWasDigit = false;
    }

    // require the placement field to describe exactly eight complete ranks
    if (rank != 0 || file != 8) {
        return false;
    }

    bool parsedWhiteTurn;

    // accept only the FEN active-colour values
    if (activeColor == "w") {
        parsedWhiteTurn = true;
    } else if (activeColor == "b") {
        parsedWhiteTurn = false;
    } else {
        return false;
    }

    CastlingRights parsedCastlingRights{false, false, false, false};

    // parse each castling right and reject unknown or repeated rights
    if (castling != "-") {
        for (char right: castling) {
            switch (right) {
                case 'K':
                    if (parsedCastlingRights.wk) return false;
                    parsedCastlingRights.wk = true;
                    break;
                case 'Q':
                    if (parsedCastlingRights.wq) return false;
                    parsedCastlingRights.wq = true;
                    break;
                case 'k':
                    if (parsedCastlingRights.bk) return false;
                    parsedCastlingRights.bk = true;
                    break;
                case 'q':
                    if (parsedCastlingRights.bq) return false;
                    parsedCastlingRights.bq = true;
                    break;
                default:
                    return false;
            }
        }
    }

    // castling rights require the king and rook to remain on their home squares
    if ((parsedCastlingRights.wk &&
         (!Utils::checkBit(parsedBoard[whiteKing], e1)
          || !Utils::checkBit(parsedBoard[whiteRook], h1)))
        || (parsedCastlingRights.wq &&
            (!Utils::checkBit(parsedBoard[whiteKing], e1)
             || !Utils::checkBit(parsedBoard[whiteRook], a1)))
        || (parsedCastlingRights.bk &&
            (!Utils::checkBit(parsedBoard[blackKing], e8)
             || !Utils::checkBit(parsedBoard[blackRook], h8)))
        || (parsedCastlingRights.bq &&
            (!Utils::checkBit(parsedBoard[blackKing], e8)
             || !Utils::checkBit(parsedBoard[blackRook], a8)))) {
        return false;
    }

    int parsedEnPassantTarget = -1;

    // validate the target coordinate and rank for the side to move
    if (enPassant != "-") {
        if (enPassant.size() != 2
            || enPassant[0] < 'a' || enPassant[0] > 'h'
            || enPassant[1] < '1' || enPassant[1] > '8') {
            return false;
        }

        if ((parsedWhiteTurn && enPassant[1] != '6')
            || (!parsedWhiteTurn && enPassant[1] != '3')) {
            return false;
        }

        parsedEnPassantTarget = Utils::coordinateToPosition(enPassant);
    }

    // both kings must appear exactly once
    if (std::popcount(parsedBoard[whiteKing]) != 1
        || std::popcount(parsedBoard[blackKing]) != 1) {
        return false;
    }

    // kings cannot occupy neighboring squares
    const int whiteKingSquare = std::countr_zero(parsedBoard[whiteKing]);
    const int blackKingSquare = std::countr_zero(parsedBoard[blackKing]);

    const int rankDifference =
            whiteKingSquare / 8 - blackKingSquare / 8;
    const int fileDifference =
            whiteKingSquare % 8 - blackKingSquare % 8;

    const int rankDistance =
            rankDifference < 0 ? -rankDifference : rankDifference;
    const int fileDistance =
            fileDifference < 0 ? -fileDifference : fileDifference;

    if (rankDistance <= 1 && fileDistance <= 1) {
        return false;
    }

    // neither side can have more pieces or pawns than it started with
    if (std::popcount(parsedBoard[whitePawn]) > 8
        || std::popcount(parsedBoard[blackPawn]) > 8) {
        return false;
    }

    int whitePieceCount = 0;
    int blackPieceCount = 0;

    for (int piece = whitePawn; piece <= whiteKing; ++piece) {
        whitePieceCount += std::popcount(parsedBoard[piece]);
    }

    for (int piece = blackPawn; piece <= blackKing; ++piece) {
        blackPieceCount += std::popcount(parsedBoard[piece]);
    }

    if (whitePieceCount > 16 || blackPieceCount > 16) {
        return false;
    }

    // estimates the minimum number of promotions needed beyond the starting material
    const auto extraPieces = [](int count, int startingCount) {
        return count > startingCount ? count - startingCount : 0;
    };

    const int whitePawnCount = std::popcount(parsedBoard[whitePawn]);
    const int blackPawnCount = std::popcount(parsedBoard[blackPawn]);

    const int whiteRequiredPromotions =
            extraPieces(std::popcount(parsedBoard[whiteQueen]), 1)
            + extraPieces(std::popcount(parsedBoard[whiteRook]), 2)
            + extraPieces(std::popcount(parsedBoard[whiteBishop]), 2)
            + extraPieces(std::popcount(parsedBoard[whiteKnight]), 2);

    const int blackRequiredPromotions =
            extraPieces(std::popcount(parsedBoard[blackQueen]), 1)
            + extraPieces(std::popcount(parsedBoard[blackRook]), 2)
            + extraPieces(std::popcount(parsedBoard[blackBishop]), 2)
            + extraPieces(std::popcount(parsedBoard[blackKnight]), 2);

    if (whiteRequiredPromotions > 8 - whitePawnCount
        || blackRequiredPromotions > 8 - blackPawnCount) {
        return false;
    }

    // the en passant target must be empty and the pawn that moved must be behind it
    if (parsedEnPassantTarget != -1) {
        for (int p = 0; p < pieceCount; ++p) {
            if (Utils::checkBit(parsedBoard[p], parsedEnPassantTarget)) {
                return false;
            }
        }

        int movedPawnSquare = parsedWhiteTurn
                                  ? parsedEnPassantTarget - 8
                                  : parsedEnPassantTarget + 8;

        Piece movedPawn = parsedWhiteTurn ? blackPawn : whitePawn;

        if (!Utils::checkBit(parsedBoard[movedPawn], movedPawnSquare)) {
            return false;
        }

        // the pawn's starting square must be empty after its double move
        const int pawnStartSquare = parsedWhiteTurn
                                        ? parsedEnPassantTarget + 8
                                        : parsedEnPassantTarget - 8;

        for (int p = 0; p < pieceCount; ++p) {
            if (Utils::checkBit(parsedBoard[p], pawnStartSquare)) {
                return false;
            }
        }

        // a double pawn move resets the halfmove clock
        if (parsedHalfmoveClock != 0) {
            return false;
        }
    }

    // apply the parsed state only after all validation succeeds
    ChessBoard parsedPosition;
    parsedPosition.board = parsedBoard;
    parsedPosition.whiteTurn = parsedWhiteTurn;
    parsedPosition.castlingRights = parsedCastlingRights;
    parsedPosition.enPassantTarget = parsedEnPassantTarget;
    parsedPosition.halfmoveClock = parsedHalfmoveClock;
    parsedPosition.fullmoveNumber = parsedFullmoveNumber;

    parsedPosition.updateOccupancy();
    parsedPosition.updateKingPosition();

    // the side that just moved cannot leave its own king in check
    if (KingValidator::isKingInCheck(parsedPosition, !parsedWhiteTurn)) {
        return false;
    }

    board = parsedPosition;

    return true;
}
