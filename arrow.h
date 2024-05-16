#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsItem>
#include <QPainter>

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

        QLineF line = calculateLine();
        painter->drawLine(line);

        QPointF arrowP2 = line.p2();
        QPointF arrowP1 = line.p1() + (line.p2() - line.p1()) * 0.5;

//        qreal angle = arrowP2 - (arrowP1-arrowP2);

        QLineF arrowLine1(arrowP1, arrowP2 + QPointF(10, -10));
        QLineF arrowLine2(arrowP1, arrowP2 + QPointF(10, 10));
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
