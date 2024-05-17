#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsItem>
#include <QPainter>

#include <math.h>

class Arrow : public QGraphicsItem
{

public:
    Arrow(QGraphicsItem* sourceNode, QGraphicsItem* destNode)
    {
        this->sourceNode = sourceNode;
        this->destNode = destNode;
    }

    QRectF boundingRect() const override
    {
        return QRectF(QPointF(0, 0), QSizeF(100, 20)).normalized();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        // получение начальных координат
        QLineF line = calculateLine();

        double angle = atan2(line.dy(), line.dx());
        double arrowSize = 10;

        // начало и конец стрелки
        QPointF arrowP1 = line.pointAt(0.1);
        QPointF arrowP2 = line.pointAt(0.15);

        // расчет координат для стрелки
        QLineF arrowLine1(arrowP1, arrowP2 + QPointF(arrowSize * 1 * cos(angle + M_PI_4), arrowSize * 1 *  sin(angle + M_PI_4)));
        QLineF arrowLine2(arrowP1, arrowP2 + QPointF(arrowSize * 1 * cos(angle - M_PI_4), arrowSize * 1 * sin(angle - M_PI_4)));

        painter->drawLine(arrowLine1);
        painter->drawLine(arrowLine2);
    }


private:
    QGraphicsItem* sourceNode;
    QGraphicsItem* destNode;


    QLineF calculateLine() const
    {
        QPointF sourcePos = sourceNode->mapToScene(sourceNode->boundingRect().center());
        QPointF destPos = destNode->mapToScene(destNode->boundingRect().center());
        return QLineF(sourcePos, destPos);
    }
};

#endif // ARROW_H
