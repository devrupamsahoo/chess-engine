//
// Created by devrupamsahoo on 04-11-2026.
//

// board widget that displays the current position and sends clicked squares to the window

#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QSizePolicy>
#include <QPen>
#include <QSvgRenderer>
#include <QFont>
#include <QString>

#include "ChessBoardWidget.h"
#include "PieceAssets.h"
#include "chess/core/GameSession.h"

ChessBoardWidget::ChessBoardWidget(GameSession *session, QWidget *parent)
    : QWidget(parent), session(session) {
    // create a renderer for each piece and let Qt manage their lifetime
    for (int piece = 0; piece < pieceCount; ++piece) {
        renderers[piece] =
                new QSvgRenderer(pieceResourcePath(static_cast<Piece>(piece)), this);

        if (!renderers[piece]->isValid()) {
            qWarning("Could not load the graphic for piece %d", piece);
        }
    }

    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QSize ChessBoardWidget::sizeHint() const {
    return QSize(640, 640);
}

QSize ChessBoardWidget::minimumSizeHint() const {
    return QSize(360, 360);
}

// enables or disables move selection
void ChessBoardWidget::setInteractionEnabled(bool enabled) {
    interactionEnabled = enabled;

    if (!enabled) {
        clearSelection();
    }

    update();
}

// highlights the king square when it is in check
void ChessBoardWidget::setCheckSquare(int square) {
    checkSquare = (square >= 0 && square < squareCount) ? square : -1;
    update();
}

// clears the currently selected square
void ChessBoardWidget::clearSelection() {
    selectedSquare = -1;
    isDragging = false;
    pendingTargetSquare = -1;
    unsetCursor();
    legalTargetSquares.fill(false);
    captureTargetSquares.fill(false);
    update();
}

// finds the legal destinations for the selected piece
void ChessBoardWidget::updateLegalMoves(int startSquare) {
    legalTargetSquares.fill(false);
    captureTargetSquares.fill(false);

    for (const LegalMoveTarget &target: session->legalMovesFrom(startSquare)) {
        if (target.square < 0 || target.square >= squareCount) {
            continue;
        }

        legalTargetSquares[target.square] = true;
        captureTargetSquares[target.square] = target.isCapture;
    }
}

// redraws the board from the current session position
void ChessBoardWidget::refresh() {
    update();
}

// calculates a centered square board that fits inside the widget
ChessBoardWidget::BoardGeometry ChessBoardWidget::computeGeometry() const {
    const qreal margin = 30.0;
    const qreal availableWidth = width() - margin * 2.0;
    const qreal availableHeight = height() - margin * 2.0;
    const qreal boardSize = qMin(availableWidth, availableHeight);

    if (boardSize <= 0) {
        return {};
    }

    const qreal left = (width() - boardSize) / 2.0;
    const qreal top = (height() - boardSize) / 2.0;

    return {
        QRectF(left, top, boardSize, boardSize),
        boardSize / 8.0,
        margin
    };
}

// returns the display rectangle for a square using a1 = 0 board indexing
QRectF ChessBoardWidget::squareRect(const BoardGeometry &geometry, int square) const {
    const int file = square % 8;
    const int rank = square / 8;
    const int displayRank = 7 - rank;

    return QRectF(
        geometry.board.left() + file * geometry.square,
        geometry.board.top() + displayRank * geometry.square,
        geometry.square,
        geometry.square
    );
}

// returns the square under the mouse, or -1 when outside the board
int ChessBoardWidget::squareAt(const QPointF &position) const {
    const BoardGeometry geometry = computeGeometry();

    if (geometry.square <= 0 || !geometry.board.contains(position)) {
        return -1;
    }

    const int file = static_cast<int>(
        (position.x() - geometry.board.left()) / geometry.square);
    const int displayRank = static_cast<int>(
        (position.y() - geometry.board.top()) / geometry.square);
    const int rank = 7 - displayRank;

    if (file < 0 || file >= 8 || rank < 0 || rank >= 8) {
        return -1;
    }

    return rank * 8 + file;
}

// draws the board squares, selection, legal moves, check highlight, pieces, and coordinates
void ChessBoardWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(35, 35, 35));

    const BoardGeometry geometry = computeGeometry();

    if (geometry.square <= 0) {
        return;
    }

    const QColor lightSquare(240, 217, 181);
    const QColor darkSquare(181, 136, 99);
    const QColor selectionColor(70, 130, 220, 110);
    const ChessBoard &board = session->board();

    for (int square = 0; square < squareCount; ++square) {
        const int file = square % 8;
        const int rank = square / 8;
        const QRectF target = squareRect(geometry, square);

        const QColor squareColor = (file + rank) % 2 == 0
                                       ? darkSquare
                                       : lightSquare;

        painter.fillRect(target, squareColor);

        const Piece piece = board.getPieceAt(square);

        if (piece != noPiece && !(isDragging && square == selectedSquare)) {
            const int pieceIndex = static_cast<int>(piece);

            if (pieceIndex >= 0
                && pieceIndex < pieceCount
                && renderers[pieceIndex]
                && renderers[pieceIndex]->isValid()) {
                renderers[pieceIndex]->render(&painter, target);
            }
        }

        if (square == selectedSquare) {
            painter.fillRect(target, selectionColor);
        }

        if (captureTargetSquares[square]) {
            const qreal inset = geometry.square * 0.08;

            painter.setPen(QPen(QColor(190, 55, 55, 220), 3.0));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(
                target.adjusted(inset, inset, -inset, -inset)
            );
        } else if (legalTargetSquares[square]) {
            const qreal radius = geometry.square * 0.12;

            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(40, 130, 70, 160));
            painter.drawEllipse(target.center(), radius, radius);
        }

        // draws a red ring around the king that is in check
        if (square == checkSquare
            && (piece == whiteKing || piece == blackKing)) {
            const qreal inset = geometry.square * 0.12;
            const QRectF kingMarker =
                    target.adjusted(inset, inset, -inset, -inset);

            painter.setPen(QPen(QColor(255, 0, 0), 5.0));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(kingMarker);
        }
    }

    // draws the dragged piece under the cursor
    if (isDragging && selectedSquare >= 0) {
        const Piece draggedPiece = board.getPieceAt(selectedSquare);
        const int pieceIndex = static_cast<int>(draggedPiece);

        if (pieceIndex >= 0
            && pieceIndex < pieceCount
            && renderers[pieceIndex]
            && renderers[pieceIndex]->isValid()) {
            const QRectF draggedRect(
                dragPosition.x() - geometry.square / 2.0,
                dragPosition.y() - geometry.square / 2.0,
                geometry.square,
                geometry.square
            );

            painter.save();
            painter.setOpacity(0.9);
            renderers[pieceIndex]->render(&painter, draggedRect);
            painter.restore();
        }
    }

    // draws rank and file labels around all four sides of the board
    painter.setPen(QColor(235, 235, 235));

    QFont labelFont = painter.font();
    labelFont.setBold(true);
    labelFont.setPointSize(10);
    painter.setFont(labelFont);

    // rank labels run from 8 at the top to 1 at the bottom on both sides
    for (int displayRank = 0; displayRank < 8; ++displayRank) {
        const qreal rowTop =
                geometry.board.top() + displayRank * geometry.square;

        const QRectF leftLabel(
            geometry.board.left() - geometry.margin,
            rowTop,
            geometry.margin,
            geometry.square
        );

        const QRectF rightLabel(
            geometry.board.right(),
            rowTop,
            geometry.margin,
            geometry.square
        );

        const QString rankLabel = QString::number(8 - displayRank);

        painter.drawText(leftLabel, Qt::AlignCenter, rankLabel);
        painter.drawText(rightLabel, Qt::AlignCenter, rankLabel);
    }

    // file labels run from a at the left to h at the right on both sides
    constexpr char fileLabels[] = "abcdefgh";

    for (int file = 0; file < 8; ++file) {
        const qreal columnLeft =
                geometry.board.left() + file * geometry.square;

        const QRectF topLabel(
            columnLeft,
            geometry.board.top() - geometry.margin,
            geometry.square,
            geometry.margin
        );

        const QRectF bottomLabel(
            columnLeft,
            geometry.board.bottom(),
            geometry.square,
            geometry.margin
        );

        const QString fileLabel =
                QString::fromLatin1(&fileLabels[file], 1);

        painter.drawText(topLabel, Qt::AlignCenter, fileLabel);
        painter.drawText(bottomLabel, Qt::AlignCenter, fileLabel);
    }
}

// selects a piece, starts dragging it, or records a clicked destination
void ChessBoardWidget::mousePressEvent(QMouseEvent *event) {
    if (!interactionEnabled || event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    const int pressedSquare = squareAt(event->position());

    if (pressedSquare < 0) {
        clearSelection();
        return;
    }

    const Piece piece = session->board().getPieceAt(pressedSquare);
    const bool isWhitePiece = piece >= whitePawn && piece <= whiteKing;
    const bool isBlackPiece = piece >= blackPawn && piece <= blackKing;
    const bool belongsToTurn =
            (session->board().isWhiteTurn() && isWhitePiece)
            || (!session->board().isWhiteTurn() && isBlackPiece);

    if (belongsToTurn) {
        selectedSquare = pressedSquare;
        pendingTargetSquare = -1;
        isDragging = true;
        dragPosition = event->position();

        updateLegalMoves(pressedSquare);
        setCursor(Qt::ClosedHandCursor);
        update();
        event->accept();
        return;
    }

    if (selectedSquare >= 0) {
        isDragging = false;
        pendingTargetSquare = pressedSquare;
        event->accept();
        return;
    }

    clearSelection();
}

// completes a drag or a click-selected move when the mouse is released
void ChessBoardWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        QWidget::mouseReleaseEvent(event);
        return;
    }

    const int releasedSquare = squareAt(event->position());

    // completes click-piece-then-click-destination input
    if (pendingTargetSquare >= 0) {
        const int startSquare = selectedSquare;
        const int targetSquare = pendingTargetSquare;
        const bool releasedOnTarget = releasedSquare == targetSquare;

        clearSelection();

        if (releasedOnTarget) {
            const Piece movingPiece = session->board().getPieceAt(startSquare);
            const bool isWhitePawn = movingPiece == whitePawn;
            const bool isBlackPawn = movingPiece == blackPawn;
            const bool isPromotion =
                    (isWhitePawn && targetSquare / 8 == 7)
                    || (isBlackPawn && targetSquare / 8 == 0);

            emit moveRequested(startSquare, targetSquare, isPromotion);
        }

        event->accept();
        return;
    }

    if (!isDragging) {
        QWidget::mouseReleaseEvent(event);
        return;
    }

    const int startSquare = selectedSquare;

    // releasing on the source square leaves it selected for click-to-move
    if (releasedSquare == startSquare) {
        isDragging = false;
        unsetCursor();
        update();
        event->accept();
        return;
    }

    clearSelection();

    if (releasedSquare >= 0) {
        const Piece movingPiece = session->board().getPieceAt(startSquare);
        const bool isWhitePawn = movingPiece == whitePawn;
        const bool isBlackPawn = movingPiece == blackPawn;
        const bool isPromotion =
                (isWhitePawn && releasedSquare / 8 == 7)
                || (isBlackPawn && releasedSquare / 8 == 0);

        emit moveRequested(startSquare, releasedSquare, isPromotion);
    }

    event->accept();
}

// changes the cursor when it is over a piece belonging to the player whose turn it is
void ChessBoardWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!interactionEnabled) {
        unsetCursor();
        return;
    }

    if (isDragging && (event->buttons() & Qt::LeftButton)) {
        dragPosition = event->position();
        update();
        event->accept();
        return;
    }

    const int hoveredSquare = squareAt(event->position());

    if (hoveredSquare < 0) {
        unsetCursor();
        return;
    }

    const Piece piece = session->board().getPieceAt(hoveredSquare);
    const bool isWhitePiece = piece >= whitePawn && piece <= whiteKing;
    const bool isBlackPiece = piece >= blackPawn && piece <= blackKing;
    const bool belongsToTurn =
            (session->board().isWhiteTurn() && isWhitePiece)
            || (!session->board().isWhiteTurn() && isBlackPiece);

    if (belongsToTurn) {
        setCursor(Qt::PointingHandCursor);
    } else {
        unsetCursor();
    }
}
