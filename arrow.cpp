#include "arrow.h"

QArrow::QArrow(const QPointF &start, const QPointF &end, QGraphicsItem *parent)
    : QGraphicsItem(parent), startPoint(start), endPoint(end)
{
}

QRectF QArrow::boundingRect() const
{
    qreal penWidth = 1.0;
    return QRectF(startPoint.x() - penWidth, startPoint.y() - penWidth,
                  endPoint.x() - startPoint.x() + penWidth * 2,
                  endPoint.y() - startPoint.y() + penWidth * 2);
}

void QArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::black);

    // Рисуем линию от startPoint к endPoint
    painter->drawLine(startPoint, endPoint);

    // Вычисляем вектор направления и его нормированную версию
    QPointF direction = endPoint - startPoint;
    qreal length = direction.manhattanLength();
    if (length > 0) {
        direction.setX(direction.x() / length);
        direction.setY(direction.y() / length);
    }

    // Вычисляем точки для стрелки
    QPointF arrowP1 = endPoint - QPointF(direction.x() + 50, -direction.y()+ 50 );
    QPointF arrowP2 = endPoint - QPointF(-direction.x() + 50, direction.y() + 50);

    // Рисуем стрелку
    QPolygonF arrowHead;
    arrowHead << endPoint << arrowP1 << arrowP2;
    painter->drawPolygon(arrowHead);
}

void QArrow::setStartPoint(const QPointF &start)
{
    prepareGeometryChange();
    startPoint = start;
}

void QArrow::setEndPoint(const QPointF &end)
{
    prepareGeometryChange();
    endPoint = end;
}
