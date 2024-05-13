#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QPainter>

class Node : public QGraphicsItem
{
public:
    Node(int index, QPointF position, QString data)
    {
        m_index = index;
        m_position = position;
        m_data = data;
    }

    QRectF boundingRect() const override
    {
        return QRectF(-20, -20, 40, 40); // Размеры узла
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setBrush(Qt::white);
        painter->drawEllipse(-20, -20, 40, 40);
        painter->drawText(-10, 10, m_data);
    }

private:
    int m_index;
    QPointF m_position;
    QString m_data;
};

#endif // NODE_H
