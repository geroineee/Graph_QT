#ifndef NODE_H
#define NODE_H


#include <QGraphicsItem>
#include <QPainter>

#include <QDebug>


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

        setFlag(ItemIsMovable); // Устанавливаем флаг, позволяющий перемещать узлы
    }

    QRectF boundingRect() const override
    {
        return QRectF(-m_size/2, -m_size/2, m_size, m_size); // Размеры узла
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        painter->setBrush(m_brush);
        painter->drawEllipse(-m_size/2, -m_size/2, m_size, m_size);
        painter->drawText(-10, 10, m_data);
    }

    // перемещение узла по зажатию ЛКМ
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsItem::mouseMoveEvent(event);

        QPointF move = event->scenePos() - event->lastScenePos();
        m_position += move;

        // Обновляем связи при перемещении узла
        emit updateLinksSignal();

        update();
    }

    // нажатие ЛКМ
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsItem::mousePressEvent(event);

        // Выделяем узел синим цветом
        QBrush brush(Qt::blue);
        m_brush = brush; // Сохраняем кисть для использования в методе paint

        // Сохраняем индекс узла, на который нажали
        int pressedIndex = m_index;

        // Отправляем сигнал с индексом нажатого узла
        emit nodePressed(pressedIndex); // Используем emit для вызова сигнала

        // Запрашиваем перерисовку элемента
        update();
    }

    // отжатие ЛКМ
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsItem::mouseReleaseEvent(event);
    }

    QBrush m_brush;
private:
    int m_index; // индекс узла (для матрицы смежности)
    int m_size; // размер окружности узла
    QPointF m_position; // позиция узла
    QString m_data;

};

#endif // NODE_H
