#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QGraphicsScene>

#include <QTimer>

#include "Node.h"

class Graph : public QObject
{

Q_OBJECT

public:
    Graph(QGraphicsScene *new_scene)
    {
        scene = new_scene;
    };

    int getSelectedNodeIndex() const
    {
        return selectedNodeIndex;
    }

private:
    // Создание матрицы смежности
    QVector<QVector<int>> adjacencyMatrix;

    // вектор всех узлов
    QVector<Node*> nodes;

    // индекс выделенного узла
    int selectedNodeIndex = -1;

    QGraphicsScene *scene;

public:
    bool needToLink = false;


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
                        qDebug() << sourcePos << "\n"<<  destPos;
                        QTimer::singleShot(0, this, SLOT(clearScene()));
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

        connect(node, &Node::nodePressed, this, &Graph::handleNodePressed);

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
        if (!needToLink)
        {
            // Очистка цвета узлов
            for (Node *node : nodes)
            {
                 QBrush brush(Qt::white);
                node->m_brush = brush;
                node->update();
            }
            updateLinks();
            return;
        }

        // Проверяем, есть ли уже выделенный узел
        if (selectedNodeIndex != -1)
        {
            // Добавляем связь между выделенным узлом и нажатым узлом
            adjacencyMatrix[selectedNodeIndex][index] = 1;
            adjacencyMatrix[index][selectedNodeIndex] = 1;

            // Очистка цвета узлов

            for (Node *node : nodes)
            {
                QBrush brush(Qt::white);
                node->m_brush = brush;
                node->update();
            }

            // Обновляем состояние выделенного узла
            selectedNodeIndex = -1;

            // Убираем флаг на добавление узла
            needToLink = false;

            // отрисовка связей
            updateLinks();
        }
        else
        {
            // Сохраняем индекс нажатого узла
            selectedNodeIndex = index;
        }
    }

    // очистка сцены
    void clearScene()
    {
        // Удаляем все узлы из вектора nodes
        for (Node* node : nodes)
        {
            QGraphicsScene* this_scene = node->scene();
            this_scene->removeItem(node); // Удаляем узел из сцены
            delete node; // Освобождаем память, выделенную под узел
        }
        nodes.clear(); // Очищаем вектор nodes

        // Очищаем сцену
//        scene->clear();
    }

    // обновление связей
    void updateLinks()
    {
        drawLinks(scene);
    }
};

#endif // GRAPH_H
