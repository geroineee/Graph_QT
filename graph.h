#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QGraphicsScene>

#include <QTimer>

#include "Node.h"

#include "arrow.h"

class Graph : public QObject
{

Q_OBJECT

signals:
    void adjacencyMatrixChanged(const QVector<QVector<int>>& adjacencyMatrix);

public:
    Graph(QGraphicsScene *new_scene)
    {
        scene = new_scene;

        // Создаем новый слой для связей
        linkLayer = new QGraphicsItemGroup();

        // Добавляем его на сцену
        scene->addItem(linkLayer);
    }

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

    // сцена для отрисовки
    QGraphicsScene *scene;

    // Новый слой для связей
    QGraphicsItemGroup* linkLayer;

public:
    // флаг для добавления новых связей между узлами
    bool needToLink = false;

    QVector<QVector<int>>& getMatrix() {return adjacencyMatrix;}


public:
    // отрисовка узлов
    void drawNodes()
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i]->scene() != scene)
            {
                scene->addItem(nodes[i]);
            }
        }
    }

    // отрисовка связей
    void drawLinks()
       {
           // Удаляем все связи из слоя
           clearLinks();

           for (int i = 0; i < adjacencyMatrix.size(); ++i)
           {
               for (int j = 0; j < adjacencyMatrix[i].size(); ++j)
               {
                   if (adjacencyMatrix[i][j] > 0)
                   {
                       QPointF sourcePos = nodes[i]->scenePos();
                       QPointF destPos = nodes[j]->scenePos();

                       // Добавление новой связи на слой
                       QGraphicsLineItem* link = scene->addLine(sourcePos.x(), sourcePos.y(), destPos.x(), destPos.y());
                       linkLayer->addToGroup(link);

                       // Добавление стрелки на слой
//                       Arrow* arrow = new Arrow(nodes[i], nodes[j]);
//                       linkLayer->addToGroup(arrow);
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

        // нажатие на узел
        connect(node, &Node::nodePressed, this, &Graph::handleNodePressed);

        // перемещение узла
        connect(node, &Node::updateLinksSignal, this, &Graph::handleUpdateLinksSignal);

        // Добавляем новую строку и столбец в матрицу смежности
        for (int i = 0; i < nodes.size() - 1; ++i)
        {
            adjacencyMatrix[i].append(0);
        }
        QVector<int> newRow(nodes.size(), 0);
        adjacencyMatrix.append(newRow);

        // Отправляем сигнал об изменении матрицы смежности
        emit adjacencyMatrixChanged(adjacencyMatrix);
    }

    // очистка всех связей
    void clearLinks()
    {
        QList<QGraphicsItem*> items = linkLayer->childItems();

        foreach (QGraphicsItem *item, items)
        {
            linkLayer->removeFromGroup(item);
            delete item;
        }
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
            drawLinks();
            }
            else
            {
                // Сохраняем индекс нажатого узла
                selectedNodeIndex = index;
            }

        // Отправляем сигнал об изменении матрицы смежности
        emit adjacencyMatrixChanged(adjacencyMatrix);
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

        // Очищаем матрицу
        adjacencyMatrix.clear();

        // Очищаем вектор nodes
        nodes.clear();

        // Очистка узлов
        clearLinks();

        // Очистка сцены
//        scene->clear();

        // Отправляем сигнал об изменении матрицы смежности
        emit adjacencyMatrixChanged(adjacencyMatrix);
    }


    // слот для обновления связей
    void handleUpdateLinksSignal()
     {
        drawLinks();
     }
};

#endif // GRAPH_H
