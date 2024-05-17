#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QGraphicsScene>

#include <QTimer>

#include "Node.h"

#include "arrow.h"
#include "link.h"

class Graph : public QObject
{

Q_OBJECT

signals:
    void adjacencyMatrixChanged(const QVector<QVector<int>>& adjacencyMatrix);


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

    // флаг для удаления узла
    bool needToDelete = false;

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
                    // Добавление связи с весом на слой
                    Link* weightLink = new Link(nodes[i], nodes[j], adjacencyMatrix[i][j]);
                    linkLayer->addToGroup(weightLink);

                    // Добавление стрелки на слой
                    Arrow* arrow = new Arrow(nodes[i], nodes[j]);
                    linkLayer->addToGroup(arrow);
               }
           }
       }
       scene->update();
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

    // Метод для удаления узла
    void removeNode(int index)
    {
        // Проверка на корректность индекса
        if (index < 0 || index >= nodes.size())
        {
            qDebug() << "removeNode: Неверный индекс.";
            return;
        }
        // Удаление узла из сцены и вектора узлов
        scene->removeItem(nodes[index]);
        delete nodes[index];
        nodes.remove(index);

        // Обновление индексов узлов
        for (int i = index; i < nodes.size(); ++i)
        {
            nodes[i]->m_index = i;
        }

        // Обновление матрицы смежности
        for (int i = index; i < nodes.size(); ++i)
        {
            for (int j = 0; j < nodes.size(); ++j)
            {
                adjacencyMatrix[i][j] = adjacencyMatrix[i + 1][j];
                adjacencyMatrix[j][i] = adjacencyMatrix[j][i + 1];
            }
        }

        // Удаление последней строки и столбца из матрицы
        adjacencyMatrix.pop_back();
        for (auto& row : adjacencyMatrix)
        {
            row.pop_back();
        }

        // Отправляем сигнал об изменении матрицы смежности
        emit adjacencyMatrixChanged(adjacencyMatrix);
    }


    // очистка всех связей
    void clearLinks()
    {
        QList<QGraphicsItem*> items = linkLayer->childItems();

        foreach (QGraphicsItem *item, items)
        {
            if (item != nullptr)
            linkLayer->removeFromGroup(item);
            delete item;
        }
    }


public slots:
    // обработка сигнала нажатия на узел
    void handleNodePressed(int index)
    {
        // --------------------------Добавление связи-----------------------------------------------------------------
        if (!needToLink && !needToDelete)
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
        else if (needToLink && selectedNodeIndex != -1)
        {
            // Добавляем связь между выделенным узлом и нажатым узлом
//            adjacencyMatrix[selectedNodeIndex][index] = 1;
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
            }
        else if (needToLink)
        {
            // Сохраняем индекс нажатого узла
            selectedNodeIndex = index;
        }

        // ------------------------------Удаление узла-----------------------------------------------------------------

        else if (needToDelete)
        {
            removeNode(index);
            needToDelete = false;
        }

        // -------------------------------------------------------------------------------------------------------------

        // отрисовка связей
        drawLinks();

        // Отправляем сигнал об изменении матрицы смежности
        emit adjacencyMatrixChanged(adjacencyMatrix);
    }

    // очистка сцены
    void clearScene()
    {
        // Удаляем все узлы из вектора nodes
        for (Node* node : nodes)
        {
            scene->removeItem(node); // Удаляем узел из сцены
            delete node; // Освобождаем память, выделенную под узел
        }

        // Очищаем матрицу
        adjacencyMatrix.clear();

        // Очищаем вектор nodes
        nodes.clear();

        // Очистка узлов
        clearLinks();

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
