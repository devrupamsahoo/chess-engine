#pragma once

#include <QMainWindow>

#include "chess/core/GameSession.h"

class ChessBoardWidget;
class QLabel;
class QHBoxLayout;
class QPushButton;
class QVBoxLayout;
class QWidget;

// coordinates the desktop controls and keeps them synchronized with the game session
class MainWindow : public QMainWindow {
public:
    // creates the window and connects its controls to the chess session
    explicit MainWindow(QWidget* parent = nullptr);

private:
    // owns the current game state used by the board and information panels
    GameSession session;

    // stores the board and labels that are refreshed when the position changes
    ChessBoardWidget* boardWidget = nullptr;
    QLabel* turnLabel = nullptr;
    QLabel* fenLabel = nullptr;

    // holds captured piece icons in separate rows for each piece color
    QHBoxLayout* capturedWhiteLayout = nullptr;
    QHBoxLayout* capturedBlackLayout = nullptr;

    // provides an insertion point for additional information panel tools
    QVBoxLayout* futureToolsLayout = nullptr;

    // buttons for navigating, resetting, loading, exporting, and copying game data
    QPushButton* undoButton = nullptr;
    QPushButton* redoButton = nullptr;
    QPushButton* resetButton = nullptr;
    QPushButton* loadFenButton = nullptr;
    QPushButton* exportFenButton = nullptr;
    QPushButton* copyFenButton = nullptr;

    // builds the two main window panels and connects their user actions
    QWidget* createInfoPanel();
    QWidget* createPlayPanel();
    void connectUiActions();

    // refreshes the turn, FEN, and captured-piece displays from the session
    void updateTurnIndicator();
    void updateFenDisplay();
    void updateCapturedPieces();
};
