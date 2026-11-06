//
// Created by devrupamsahoo on 04-11-2026.
//

//
// helpers for locating and rendering the piece graphics compiled in from assets/pieces
//

// include Qt headers first because Enums.h defines direction names such as left and right
#include <QPainter>
#include <QSvgRenderer>
#include <QtMath>

#include <cstddef>

#include "../include/PieceAssets.h"

// returns the embedded SVG resource path for a piece
QString pieceResourcePath(Piece piece) {
    // piece enum values are used as direct indices into the piece lookup tables
    const int index = static_cast<int>(piece);

    if (index < 0 || index >= pieceCount) {
        return {};
    }

    // piece values are grouped by color, allowing the resource prefix to be selected by index
    const QChar color = index < blackPawn
        ? QLatin1Char('w')
        : QLatin1Char('b');

    // the lookup table provides the piece character, which is converted to uppercase for the SVG filename
    const QChar type = QChar::fromLatin1(
        pieceToCharMap[static_cast<std::size_t>(index)]).toUpper();

    return QStringLiteral(":/pieces/%1%2.svg").arg(color).arg(type);
}

// renders a piece into a pixmap at the requested display size
QPixmap renderPiecePixmap(Piece piece, int logicalSize, qreal devicePixelRatio) {
    if (logicalSize <= 0 || devicePixelRatio <= 0) {
        return {};
    }

    // scale the physical pixel dimensions according to the display's pixel density
    const int pixelSize = qMax(1, qRound(logicalSize * devicePixelRatio));

    QPixmap pixmap(pixelSize, pixelSize);
    pixmap.fill(Qt::transparent);

    // tells Qt how many physical pixels represent one logical display pixel
    pixmap.setDevicePixelRatio(devicePixelRatio);

    QSvgRenderer renderer(pieceResourcePath(piece));

    if (!renderer.isValid()) {
        return {};
    }

    QPainter painter(&pixmap);

    // render using logical dimensions so the SVG scales correctly on high-DPI displays
    renderer.render(&painter, QRectF(0, 0, logicalSize, logicalSize));

    return pixmap;
}
