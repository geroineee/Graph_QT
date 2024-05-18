#ifndef LINK_H
#define LINK_H

#include <QGraphicsItem>
#include <QPainter>
#include <QFont>

#include "Node.h"

class Link : public QGraphicsItem
{
public:
    Link(Node* sourceNode, Node* destNode, int weight)
    {
        this->sourceNode = sourceNode;
        this->destNode = destNode;
        this->weight = weight;
    }

    QRectF boundingRect() const override
    {
        return QRectF(QPointF(0, 0), QSizeF(100, 20)).normalized();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        QPen pen;
        pen.setWidth(2);
        painter->setPen(pen);

        // Получение начальных координат
        QLineF line = calculateLine();

        // Отрисовка линии
        painter->drawLine(line);

        // Отрисовка прямоугольника с весом связи
        QPointF center = line.pointAt(0.5);
        QRectF rect(center.x() - 20, center.y() - 10, 40, 20);
        painter->setBrush(Qt::white);
        painter->drawRect(rect);
        painter->setPen(Qt::black);
        painter->setFont(QFont("Arial", 10));
        painter->drawText(rect, Qt::AlignCenter, QString::number(weight));
    }

    void setWeight(int newWeight) {
        weight = newWeight;
        update(); // Обновляем отображение веса
    }

    int getWeight() const {
        return weight;
    }

public:
    Node* sourceNode;
    Node* destNode;
    int weight;

    QLineF calculateLine() const
    {
        QPointF sourcePos = sourceNode->mapToScene(sourceNode->boundingRect().center());
        QPointF destPos = destNode->mapToScene(destNode->boundingRect().center());
        return QLineF(sourcePos, destPos);
    }
};

#endif // LINK_H
