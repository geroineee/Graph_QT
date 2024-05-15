#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QGraphicsScene>

#include "Node.h"

class Graph : public QObject
{
public:
    Graph();

private:
    // Создание матрицы смежности
    QVector<QVector<int>> adjacencyMatrix;

    // вектор всех узлов
    QVector<Node*> nodes;

    // индекс выделенного узла
    int selectedNodeIndex = -1;

public:
    // отрисовка узлов
    void drawNodes(QGraphicsScene *scene)
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            scene->addItem(nodes[i]);
        }
    }

    // отрисовка связей
    void drawLinks(QGraphicsScene *scene)
    {
        for (int i = 0; i < adjacencyMatrix.size(); ++i)
        {
            for (int j = 0; j < adjacencyMatrix[i].size(); ++j)
            {
                if (adjacencyMatrix[i][j] > 0)
                {
                    QPointF sourcePos = nodes[i]->scenePos();
                    QPointF destPos = nodes[j]->scenePos();
                    scene->addLine(sourcePos.x(), sourcePos.y(), destPos.x(), destPos.y());
                }
            }
        }
    }

    // добавление узла (рандомно)
    void addNode()
    {
        // создание узла и рандомизация его расположения
        QPointF position(qrand() % 400, qrand() % 400);
        Node* node = new Node(nodes.size(), position, QString::number(qrand() % 400), 60);
        node->setPos(position);
        nodes.append(node);

        //connect(node, &Node::nodePressed, this, &Graph::handleNodePressed);

        qDebug()  << "Начальная позиция: " << position;

        // Добавляем новую строку и столбец в матрицу смежности
        for (int i = 0; i < nodes.size() - 1; ++i)
        {
            adjacencyMatrix[i].append(0);
        }
        QVector<int> newRow(nodes.size(), 0);
        adjacencyMatrix.append(newRow);
    }

public slots:
    // обработка сигнала нажатия на узел
    void handleNodePressed(int index)
    {
        // Проверяем, есть ли уже выделенный узел
        if (selectedNodeIndex != -1)
        {
            // Добавляем связь между выделенным узлом и нажатым узлом
            adjacencyMatrix[selectedNodeIndex][index] = 1;
            adjacencyMatrix[index][selectedNodeIndex] = 1;

            // Обновляем состояние выделенного узла
            selectedNodeIndex = -1;
        }
        else
        {
            // Сохраняем индекс нажатого узла
            selectedNodeIndex = index;
        }
    }
};

#endif // GRAPH_H
