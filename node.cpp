//#include "Node.h"


//void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsItem::mouseReleaseEvent(event);
//}

//// нажатие ЛКМ
//void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsItem::mousePressEvent(event);

//    // Выделяем узел синим цветом
//    QBrush brush(Qt::blue);

//    QPainter painter;
//    painter.setBrush(brush);

//    // Сохраняем индекс узла, на который нажали
//    int pressedIndex = m_index;

//    // Отправляем сигнал с индексом нажатого узла
//    emit nodePressed(pressedIndex); // Используем emit для вызова сигнала
//}


//// перемещение узла по зажатию ЛКМ
//void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsItem::mouseMoveEvent(event);

//    QPointF move = event->scenePos() - event->lastScenePos(); // Получаем вектор перемещения мыши

//    m_position += move; // Смещаем центр узла на вектор перемещения
//}
