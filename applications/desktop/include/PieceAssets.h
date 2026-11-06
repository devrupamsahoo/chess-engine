//
// Created by devrupamsahoo on 04-11-2026.
//

//
// helpers for locating and rendering the piece graphics compiled in from assets/pieces
//

#ifndef CHESS_ENGINE_PIECEASSETS_H
#define CHESS_ENGINE_PIECEASSETS_H

#include <QPixmap>
#include <QString>

#include "chess/core/Enums.h"

// returns the embedded SVG resource path for a piece
QString pieceResourcePath(Piece piece);

// renders a piece into a pixmap at the requested display size
QPixmap renderPiecePixmap(Piece piece, int logicalSize, qreal devicePixelRatio);

#endif //CHESS_ENGINE_PIECEASSETS_H
