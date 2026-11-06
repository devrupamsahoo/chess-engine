//
// board widget that displays the current position and sends clicked squares to the window
//

#ifndef CHESS_ENGINE_CHESSBOARDWIDGET_H
#define CHESS_ENGINE_CHESSBOARDWIDGET_H

#include <array>

#include <QRectF>
#include <QWidget>

#include "chess/core/Enums.h"

class GameSession;
class QMouseEvent;
class QPaintEvent;
class QSvgRenderer;

class ChessBoardWidget : public QWidget {
    Q_OBJECT

public:
    // reads the board from the session, which must outlive this widget
    explicit ChessBoardWidget(GameSession* session, QWidget* parent = nullptr);

    // provides the preferred and minimum dimensions for the board widget
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    // enables or disables move selection
    void setInteractionEnabled(bool enabled);

    // highlights the king square when it is in check
    void setCheckSquare(int square);

public slots:
    // clears the currently selected square
    void clearSelection();

    // redraws the board from the current session position
    void refresh();


signals:
    // sends a requested move to the main window
    void moveRequested(int from, int to, bool isPromotion);

protected:
    // handles board rendering and mouse input for selecting and dragging pieces
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    // stores the calculated board area, individual square size, and surrounding margin
    struct BoardGeometry {
        QRectF board;
        qreal square = 0;
        qreal margin = 0;
    };

    // calculates board dimensions and converts board squares into screen rectangles
    BoardGeometry computeGeometry() const;
    QRectF squareRect(const BoardGeometry& geometry, int square) const;

    // returns the square under the mouse, or -1 when outside the board
    int squareAt(const QPointF& position) const;

    // provides access to the current game position and renders the individual piece icons
    GameSession* session;
    std::array<QSvgRenderer*, pieceCount> renderers{};

    // tracks the selected piece, drag state, and current mouse position during interaction
    int selectedSquare = -1;
    bool isDragging = false;
    int pendingTargetSquare = -1;
    QPointF dragPosition;
    int checkSquare = -1;
    bool interactionEnabled = true;

    // stores legal destinations and identifies which ones capture a piece
    std::array<bool, squareCount> legalTargetSquares{};
    std::array<bool, squareCount> captureTargetSquares{};

    // finds the legal destinations for the selected piece
    void updateLegalMoves(int startSquare);
};

#endif //CHESS_ENGINE_CHESSBOARDWIDGET_H
