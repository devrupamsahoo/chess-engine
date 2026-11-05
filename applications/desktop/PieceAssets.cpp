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

#include "PieceAssets.h"

// returns the embedded SVG resource path for a piece
QString pieceResourcePath(Piece piece) {
    const int index = static_cast<int>(piece);

    if (index < 0 || index >= pieceCount) {
        return {};
    }

    const QChar color = index < blackPawn
        ? QLatin1Char('w')
        : QLatin1Char('b');

    const QChar type = QChar::fromLatin1(
        pieceToCharMap[static_cast<std::size_t>(index)]).toUpper();

    return QStringLiteral(":/pieces/%1%2.svg").arg(color).arg(type);
}

// renders a piece into a pixmap at the requested display size
QPixmap renderPiecePixmap(Piece piece, int logicalSize, qreal devicePixelRatio) {
    if (logicalSize <= 0 || devicePixelRatio <= 0) {
        return {};
    }

    const int pixelSize = qMax(1, qRound(logicalSize * devicePixelRatio));

    QPixmap pixmap(pixelSize, pixelSize);
    pixmap.fill(Qt::transparent);
    pixmap.setDevicePixelRatio(devicePixelRatio);

    QSvgRenderer renderer(pieceResourcePath(piece));

    if (!renderer.isValid()) {
        return {};
    }

    QPainter painter(&pixmap);
    renderer.render(&painter, QRectF(0, 0, logicalSize, logicalSize));

    return pixmap;
}
