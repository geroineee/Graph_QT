#ifndef NODE_H
#define NODE_H


#include <QGraphicsItem>
#include <QPainter>

#include <QDebug>

#include <cmath>

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class Node : public QObject, public QGraphicsItem
{
Q_OBJECT

signals:
    void nodePressed(int index); // Объявление сигнала нажатия на узел
    void updateLinksSignal(); // сигнал на обновление связи

public:
    Node(int index, QPointF position, QString data, int size)
    {
        m_index = index;
        m_position = position;
        m_data = data;
        m_size = size;
        m_brush = Qt::white;
        m_isMovable = true;


        setFlag(ItemIsMovable, true); // Устанавливаем флаг, позволяющий перемещать узлы
    }

    int getSize() {return m_size;}

    void setPosition(const QPointF& position) {m_position = position;}

    void isMovableNode(bool flag)
    {
        m_isMovable = flag;
    }

    QRectF boundingRect() const override
    {
        return QRectF(-m_size/2, -m_size/2, m_size, m_size); // Размеры узла
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)


        QPen pen;
        pen.setWidth(2);
        painter->setPen(pen);

        // основной элемент узла
        painter->setBrush(m_brush);
        painter->drawEllipse(-m_size/2, -m_size/2, m_size, m_size);
        painter->drawText(-10, 10, m_data);

        // прямоугольник с номером узла
        QRectF rect(boundingRect().topLeft(), QSizeF(15, 15));

        painter->setBrush(Qt::white);
        painter->drawRect(rect);
        painter->setFont(QFont("Arial", 9));
        painter->drawText(rect, Qt::AlignCenter, QString::number(m_index+1));
    }

    // перемещение узла по зажатию ЛКМ
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (m_isMovable)
        {
            QGraphicsItem::mouseMoveEvent(event);

            QPointF move = event->scenePos() - event->lastScenePos();
            m_position += move;

            // Обновляем связи при перемещении узла
            emit updateLinksSignal();
        }
    }

    // нажатие ЛКМ
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsItem::mousePressEvent(event);

        // Выделяем узел синим цветом
        QBrush brush(Qt::cyan);
        // Сохраняем кисть для использования в методе paint
        m_brush = brush;
    }

    // отжатие ЛКМ
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsItem::mouseReleaseEvent(event);

        // Отправляем сигнал с индексом нажатого узла
        emit nodePressed(m_index);
    }

    int m_index; // индекс узла (для матрицы смежности)
    QBrush m_brush;
    bool m_isMovable;
private:
    int m_size; // размер окружности узла
    QPointF m_position; // позиция узла
    QString m_data;

};

#endif //NODE_H
