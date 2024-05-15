#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QPainter>

#include <QDebug>

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class Node : public QGraphicsItem
{

signals:
    void nodePressed(int index); // Объявление сигнала


public:
    Node(int index, QPointF position, QString data, int size)
    {
        m_index = index;
        m_position = position;
        m_data = data;
        m_size = size;
        setFlag(ItemIsMovable); // Устанавливаем флаг, позволяющий перемещать узлы
    }

    QRectF boundingRect() const override
    {
        return QRectF(-m_size/2, -m_size/2, m_size, m_size); // Размеры узла
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setBrush(Qt::white);
        painter->drawEllipse(-m_size/2, -m_size/2, m_size, m_size);
        painter->drawText(-10, 10, m_data);
    }

    // перемещение узла по зажатию ЛКМ
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    // нажатие ЛКМ
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


    // отжатие ЛКМ
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int m_index; // индекс узла
    int m_size; // размер окружности узла
    QPointF m_position; // позиция узла
    QString m_data;
};

#endif // NODE_H
