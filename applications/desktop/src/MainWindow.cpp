// Created by devrupamsahoo on 04-11-2026.

#include "../include/MainWindow.h"

#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLayoutItem>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QStandardPaths>
#include <QStatusBar>
#include <QTextStream>
#include <QVBoxLayout>

#include "../include/ChessBoardWidget.h"
#include "../include/PieceAssets.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // starts the window at a 16:9 size while keeping enough room for both panels
    setWindowTitle("Chess Engine");
    resize(1440, 810);
    setMinimumSize(1120, 630);

    // places both panels inside one resizable horizontal split
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("mainWindowRoot");

    QHBoxLayout *rootLayout = new QHBoxLayout(centralWidget);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(16);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    mainSplitter->setObjectName("mainSplitter");
    mainSplitter->setChildrenCollapsible(false);

    // keeps the information panel on the left and the chessboard on the right
    mainSplitter->addWidget(createInfoPanel());
    mainSplitter->addWidget(createPlayPanel());
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 1);
    mainSplitter->setSizes({700, 700});

    rootLayout->addWidget(mainSplitter);
    setCentralWidget(centralWidget);

    // applies the shared dark theme and distinct styles for the key controls
    setStyleSheet(R"(
        QMainWindow, QWidget#mainWindowRoot {
            background-color: #0b1120;
        }
        QWidget#infoPanel, QWidget#playPanel {
            background-color: #151e2e;
            border: 1px solid #2b3a50;
            border-radius: 16px;
        }
        QLabel {
            color: #dbe5f2;
        }
        QLabel#turnLabel {
            color: #f8fafc;
            font-size: 26px;
            font-weight: 700;
        }
        QLabel#sectionLabel {
            color: #91a4bc;
            font-size: 12px;
            font-weight: 700;
        }
        QLabel#fenLabel {
            background-color: #0f1725;
            border: 1px solid #263449;
            border-radius: 9px;
            color: #bdcadd;
            font-family: "Consolas";
            font-size: 12px;
            padding: 10px;
        }
        QPushButton {
            background-color: #26364d;
            border: 1px solid #3a4d67;
            border-radius: 8px;
            color: #f8fafc;
            font-size: 12px;
            font-weight: 600;
            min-height: 34px;
            padding: 5px 8px;
        }
        QPushButton:hover {
            background-color: #354a65;
            border-color: #5d789c;
        }
        QPushButton:pressed {
            background-color: #1d4ed8;
            border-color: #3b82f6;
        }
        QPushButton#copyFenButton {
            background-color: #176b61;
            border-color: #258b7e;
        }
        QPushButton#copyFenButton:hover {
            background-color: #208477;
        }
        QSplitter::handle {
            background-color: #263449;
            width: 3px;
        }
        QSplitter::handle:hover {
            background-color: #60a5fa;
        }
        QStatusBar {
            background-color: #0b1120;
            color: #cbd5e1;
        }
    )");

    // fills the initial labels and captured-piece rows from the starting position
    updateTurnIndicator();
    updateFenDisplay();
    updateCapturedPieces();
    connectUiActions();
}

// creates the left panel with turn information, captured pieces, FEN, and controls
QWidget *MainWindow::createInfoPanel() {
    QWidget *infoPanel = new QWidget;
    infoPanel->setObjectName("infoPanel");

    QVBoxLayout *infoLayout = new QVBoxLayout(infoPanel);
    infoLayout->setContentsMargins(24, 24, 24, 22);
    infoLayout->setSpacing(14);

    turnLabel = new QLabel(infoPanel);
    turnLabel->setObjectName("turnLabel");
    turnLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // stores future status widgets directly below the turn indicator
    futureToolsLayout = new QVBoxLayout;
    futureToolsLayout->setSpacing(10);

    // keeps turn information at the top and reserves an extensible area for future tools
    infoLayout->addWidget(turnLabel);
    infoLayout->addLayout(futureToolsLayout);
    infoLayout->addStretch(1);

    // groups captured pieces by their original color for quick position review
    QLabel *capturedHeading = new QLabel("CAPTURED PIECES", infoPanel);
    capturedHeading->setObjectName("sectionLabel");

    // creates one horizontal icon row for pieces captured from each side
    QVBoxLayout *capturedRows = new QVBoxLayout;
    capturedRows->setSpacing(8);

    QHBoxLayout *whiteCapturedRow = new QHBoxLayout;
    QLabel *whiteCapturedLabel = new QLabel("White", infoPanel);
    whiteCapturedLabel->setMinimumWidth(58);
    capturedWhiteLayout = new QHBoxLayout;
    whiteCapturedRow->addWidget(whiteCapturedLabel);
    whiteCapturedRow->addLayout(capturedWhiteLayout);
    whiteCapturedRow->addStretch(1);

    QHBoxLayout *blackCapturedRow = new QHBoxLayout;
    QLabel *blackCapturedLabel = new QLabel("Black", infoPanel);
    blackCapturedLabel->setMinimumWidth(58);
    capturedBlackLayout = new QHBoxLayout;
    blackCapturedRow->addWidget(blackCapturedLabel);
    blackCapturedRow->addLayout(capturedBlackLayout);
    blackCapturedRow->addStretch(1);

    capturedRows->addLayout(whiteCapturedRow);
    capturedRows->addLayout(blackCapturedRow);

    // keeps the current position readable while providing a dedicated copy action
    QLabel *fenHeading = new QLabel("CURRENT POSITION · FEN", infoPanel);
    fenHeading->setObjectName("sectionLabel");

    // lets the full FEN wrap in the available space and remain selectable for manual copying
    fenLabel = new QLabel(infoPanel);
    fenLabel->setObjectName("fenLabel");
    fenLabel->setWordWrap(true);
    fenLabel->setTextInteractionFlags(
        Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard
    );

    copyFenButton = new QPushButton("Copy Current FEN", infoPanel);
    copyFenButton->setObjectName("copyFenButton");

    // keeps the FEN text and its dedicated clipboard action together
    QHBoxLayout *fenLayout = new QHBoxLayout;
    fenLayout->setSpacing(8);
    fenLayout->addWidget(fenLabel, 1);
    fenLayout->addWidget(copyFenButton);

    // keeps the main game actions together in one compact row
    undoButton = new QPushButton("Undo", infoPanel);
    redoButton = new QPushButton("Redo", infoPanel);
    resetButton = new QPushButton("Reset", infoPanel);
    loadFenButton = new QPushButton("Load FEN", infoPanel);
    exportFenButton = new QPushButton("Export FEN", infoPanel);

    // gives each game action a place in the same compact horizontal control row
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->setSpacing(6);
    controlsLayout->addWidget(undoButton);
    controlsLayout->addWidget(redoButton);
    controlsLayout->addWidget(resetButton);
    controlsLayout->addWidget(loadFenButton);
    controlsLayout->addWidget(exportFenButton);

    // places game status at the top and position details and controls at the bottom
    infoLayout->addWidget(capturedHeading);
    infoLayout->addLayout(capturedRows);
    infoLayout->addWidget(fenHeading);
    infoLayout->addLayout(fenLayout);
    infoLayout->addLayout(controlsLayout);

    return infoPanel;
}

// creates the right panel and lets the chessboard use its available space
QWidget *MainWindow::createPlayPanel() {
    QWidget *playPanel = new QWidget;
    playPanel->setObjectName("playPanel");

    QVBoxLayout *playLayout = new QVBoxLayout(playPanel);
    playLayout->setContentsMargins(8, 8, 8, 8);
    playLayout->setSpacing(0);

    // gives the board the available panel area so its square can grow with the window
    boardWidget = new ChessBoardWidget(&session, playPanel);
    playLayout->addWidget(boardWidget, 1);

    return playPanel;
}

// connects the board and controls to their session actions
void MainWindow::connectUiActions() {
    // sends selected moves to the chess session and refreshes the display
    connect(boardWidget, &ChessBoardWidget::moveRequested,
            this, [this](int startSquare, int endSquare, bool isPromotion) {
                Piece promotionPiece = noPiece;

                if (isPromotion) {
                    bool accepted = false;
                    const QStringList choices = {"Queen", "Rook", "Bishop", "Knight"};

                    const QString choice = QInputDialog::getItem(
                        this,
                        "Pawn Promotion",
                        "Choose a piece:",
                        choices,
                        0,
                        false,
                        &accepted
                    );

                    if (!accepted) {
                        return;
                    }

                    if (choice == "Queen") {
                        promotionPiece = session.board().isWhiteTurn() ? whiteQueen : blackQueen;
                    } else if (choice == "Rook") {
                        promotionPiece = session.board().isWhiteTurn() ? whiteRook : blackRook;
                    } else if (choice == "Bishop") {
                        promotionPiece = session.board().isWhiteTurn() ? whiteBishop : blackBishop;
                    } else {
                        promotionPiece = session.board().isWhiteTurn() ? whiteKnight : blackKnight;
                    }
                }

                // asks the session to validate and apply the requested square-to-square move
                const bool moveMade = session.makeMove(startSquare, endSquare, promotionPiece);

                // redraws all position details from the updated session state
                boardWidget->refresh();
                updateTurnIndicator();
                updateFenDisplay();
                updateCapturedPieces();

                // statusBar()->showMessage(
                //     moveMade ? "Move accepted" : "Move rejected", 3000
                // );
            });

    // resets the session and refreshes the displayed position
    connect(resetButton, &QPushButton::clicked, this, [this]() {
        // starts a new game and removes any selection from the previous position
        session.reset();
        boardWidget->clearSelection();
        // clears stale highlights and refreshes every display tied to the position
        boardWidget->refresh();
        updateTurnIndicator();
        updateFenDisplay();
        updateCapturedPieces();

        // statusBar()->showMessage("Game reset", 3000);
    });

    // restores the previous position and refreshes the displayed information
    connect(undoButton, &QPushButton::clicked, this, [this]() {
        // stops here when the move history has no earlier position to restore
        if (!session.undo()) {

            // statusBar()->showMessage("Nothing to undo", 3000);
            return;
        }

        boardWidget->clearSelection();
        // undo changes the board and all labels that describe the current position
        boardWidget->refresh();
        updateTurnIndicator();
        updateFenDisplay();
        updateCapturedPieces();

        // statusBar()->showMessage("Move undone", 3000);
    });

    // reapplies the next undone move and refreshes the displayed information
    connect(redoButton, &QPushButton::clicked, this, [this]() {
        // stops here when there is no undone move waiting to be replayed
        if (!session.redo()) {

            // statusBar()->showMessage("Nothing to redo", 3000);
            return;
        }

        boardWidget->clearSelection();
        // redo changes the board and all labels that describe the current position
        boardWidget->refresh();
        updateTurnIndicator();
        updateFenDisplay();
        updateCapturedPieces();

        // statusBar()->showMessage("Move redone", 3000);
    });

    // loads one FEN position and starts a new session from it
    connect(loadFenButton, &QPushButton::clicked, this, [this]() {
        // starts with the current FEN so it can be edited into a new position
        bool accepted = false;
        const QString fen = QInputDialog::getText(
            this,
            "Load FEN Position",
            "Enter a FEN position:",
            QLineEdit::Normal,
            QString::fromStdString(session.currentFen()),
            &accepted
        );

        // cancel closes the dialog without changing the current game
        if (!accepted) {
            return;
        }

        // leaves the current game intact when the supplied FEN is invalid
        if (!session.loadFen(fen.toStdString())) {
            QMessageBox::warning(
                this, "Invalid FEN", "The FEN position could not be loaded."
            );
            return;
        }

        // clears old highlights because loading FEN replaces the active position
        boardWidget->clearSelection();
        boardWidget->refresh();
        updateTurnIndicator();
        updateFenDisplay();
        updateCapturedPieces();

        // statusBar()->showMessage("FEN position loaded", 3000);
    });

    // copies the current position FEN to the system clipboard
    connect(copyFenButton, &QPushButton::clicked, this, [this]() {
        // reads from the session so the copied text always matches the displayed position
        QApplication::clipboard()->setText(
            QString::fromStdString(session.currentFen())
        );

        // statusBar()->showMessage("Current FEN copied", 3000);
    });

    // exports the recorded FEN positions to a text file
    connect(exportFenButton, &QPushButton::clicked, this, [this]() {
        // suggests the documents folder while still letting the user choose another path
        const QString documentsPath =
                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        const QString defaultPath = documentsPath + "/match_fens.txt";
        const QString filePath = QFileDialog::getSaveFileName(
            this,
            "Export FEN History",
            defaultPath,
            "Text files (*.txt);;All files (*)"
        );

        // closing the save dialog without a path cancels the export
        if (filePath.isEmpty()) {
            return;
        }

        // opens the chosen destination and reports a useful error if it cannot be written
        QFile fenFile(filePath);
        if (!fenFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Export Failed", fenFile.errorString());
            return;
        }

        // writes one recorded position per line so the match history is easy to inspect
        QTextStream output(&fenFile);
        for (const std::string &fen: session.fenHistory()) {
            output << QString::fromStdString(fen) << '\n';
        }
        output.flush();

        // checks the stream after writing so a partial export is reported to the user
        if (output.status() != QTextStream::Ok) {
            fenFile.close();
            QMessageBox::warning(
                this, "Export Failed", "Could not write the complete FEN history."
            );
            return;
        }

        fenFile.close();

        // statusBar()->showMessage("FEN history exported", 3000);
    });
}

// updates the turn indicator and highlights the king when it is in check
void MainWindow::updateTurnIndicator() {
    // reads whose turn it is and whether that side's king is currently attacked
    const bool whiteToMove = session.board().isWhiteTurn();
    const bool kingInCheck = session.isKingInCheck(whiteToMove);
    const QString turnText = whiteToMove
                                 ? QStringLiteral("White to move")
                                 : QStringLiteral("Black to move");

    // marks check in both the turn text and board so it is visible in either area
    turnLabel->setText(
        kingInCheck ? turnText + QStringLiteral(" · CHECK") : turnText
    );
    // changes the turn text color only while the side to move is in check
    turnLabel->setStyleSheet(
        kingInCheck
            ? QStringLiteral("color: #ff7777; font-weight: bold;")
            : QString()
    );
    // passes the checked king square to the board, or clears the marker when safe
    boardWidget->setCheckSquare(
        kingInCheck ? session.board().getKingPos(whiteToMove) : -1
    );
}

// updates the label to show the current position as FEN
void MainWindow::updateFenDisplay() {
    // gets FEN from the session so the label always reflects the current board state
    fenLabel->setText(QString::fromStdString(session.currentFen()));
}

// redraws captured pieces grouped by their original color
void MainWindow::updateCapturedPieces() {
    // removes the previous icons before rebuilding the rows from the session history
    const auto clearCapturedRow = [](QHBoxLayout *row) {
        while (QLayoutItem *item = row->takeAt(0)) {
            delete item->widget();
            delete item;
        }
    };

    clearCapturedRow(capturedWhiteLayout);
    clearCapturedRow(capturedBlackLayout);

    // rebuilds both rows from the session's captured-piece list after each position change
    for (Piece piece: session.capturedPieces()) {
        QLabel *pieceIcon = new QLabel;
        pieceIcon->setFixedSize(28, 28);
        pieceIcon->setPixmap(renderPiecePixmap(piece, 24, devicePixelRatioF()));

        if (piece >= whitePawn && piece <= whiteKing) {
            capturedWhiteLayout->addWidget(pieceIcon);
        } else if (piece >= blackPawn && piece <= blackKing) {
            capturedBlackLayout->addWidget(pieceIcon);
        }
    }
}
