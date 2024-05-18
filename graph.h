#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QGraphicsScene>

#include <QTimer>
#include <QThread>
#include <QInputDialog>
#include <QString>

#include <QDebug>

#include "Node.h"

#include "arrow.h"
#include "link.h"

class Graph : public QObject
{

Q_OBJECT

signals:
    void adjacencyMatrixChanged(const QVector<QVector<int>>& adjacencyMatrix);

    void textToStatusBar(QString);


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

    // диалоговое окно
    QInputDialog inputDialog;

    // Ответ на задачу коммивояжера
     QVector<int> path;

     // результат алгоритмов обхода
     QVector<QVector<int>> paths;

public:
    // флаг для добавления новых связей между узлами
    bool needToLink = false;

    // флаг для удаления узла
    bool needToDelete = false;

    // удаление связи
    bool needToDeleteLink = false;

    // комивояжер
    bool needToSolveTask = false;

    // Двусвязное добавление
    bool needTwoWayAddition = false;

    // Алгоритм дейкстры
    bool needDeixtra = false;


public:
    Graph(QGraphicsScene *new_scene)
    {
        scene = new_scene;

        // Создаем новый слой для связей
        linkLayer = new QGraphicsItemGroup();

        // Добавляем его на сцену
        scene->addItem(linkLayer);
    }

    QVector<int> getPath(){return path;}

    QVector<QVector<int>> getPaths(){return paths;}

    int getSelectedNodeIndex() const {return selectedNodeIndex;}

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
        // Ввод данных
        bool confirm;

        QString text = inputDialog.getText(nullptr, "Введите данные в узел",
                                            "Данные:", QLineEdit::Normal,
                                            "", &confirm);

        // При отмене \ пустой строке \ не число
        if ( !confirm /*|| text == "" || !text.toInt() */)
        {
            return;
        }

        // создание узла и рандомизация его расположения
        QPointF position(qrand() % 400, qrand() % 400);
        Node* node = new Node(nodes.size(), position, text, 60);
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

    // Решение задачи коммивояжера
    QVector<int> solveTravelingSalesmanProblem(int startIndex)
    {
        if (nodes.isEmpty() || startIndex < 0 || startIndex >= nodes.size())
            return QVector<int>(); // Возвращаем пустой вектор, если граф пуст или индекс некорректен

        QVector<bool> visited(nodes.size(), false);
        QVector<int> path;
        path.reserve(nodes.size());

        // Начинаем с указанного узла
        int currentNode = startIndex;
        visited[currentNode] = true;
        path.append(currentNode);

        // Пока не посетим все узлы
        while (path.size() < nodes.size())
        {
            int nextNode = -1;
            int minDistance = INT_MAX;

            // Ищем ближайший непосещенный узел
            for (int i = 0; i < nodes.size(); ++i)
            {
                if (!visited[i] && adjacencyMatrix[currentNode][i] > 0 && adjacencyMatrix[currentNode][i] < minDistance)
                {
                    minDistance = adjacencyMatrix[currentNode][i];
                    nextNode = i;
                }
            }

            if (nextNode == -1)
                break; // Если не удалось найти следующий узел, выходим из цикла

            // Добавляем следующий узел в путь и отмечаем его как посещенный
            path.append(nextNode);
            visited[nextNode] = true;
            currentNode = nextNode;
        }

        // Возвращаемся в начальный узел
        path.append(startIndex);

        return path;
    }

    QVector<QVector<int>> shortestPaths(int startIndex)
    {
        int size = nodes.size();
        QVector<QVector<int>> paths(size, QVector<int>());
        QVector<int> distances(size, std::numeric_limits<int>::max());
        QVector<bool> visited(size, false);

        distances[startIndex] = 0;

        for (int count = 0; count < size - 1; ++count)
        {
            int minDistance = std::numeric_limits<int>::max();
            int minIndex = -1;

            // Выбираем узел с минимальным расстоянием
            for (int i = 0; i < size; ++i)
            {
                if (!visited[i] && distances[i] <= minDistance)
                {
                    minDistance = distances[i];
                    minIndex = i;
                }
            }

            // узел с минимальным расстоянием среди непосещенных узлов.
            int u = minIndex;
            visited[u] = true;

            // Обновляем расстояния до соседей выбранного узла
            for (int v = 0; v < size; ++v)
            {
                if (!visited[v] && adjacencyMatrix[u][v] && distances[u] != std::numeric_limits<int>::max() &&
                    distances[u] + adjacencyMatrix[u][v] < distances[v])
                {
                    distances[v] = distances[u] + adjacencyMatrix[u][v];
                    paths[v] = paths[u]; // Копируем путь до u
                    paths[v].append(v); // Добавляем узел v в путь
                }
            }
        }

        // Устанавливаем пустой путь для начального узла
        paths[startIndex].clear();
        return paths;
    }

    // Визуализация обхода
    void highlightPath(const QVector<int>& path)
    {

        for (int i = 0; i < path.size(); ++i)
        {
            QTimer::singleShot( i*800, [this, path, i]()
            {
                int prev_index = -1;
                if ((i - 1) >= 0)
                {
                    prev_index = path[i-1];
                }

                int index = path[i];

                // Перекрашивание узла
                nodes[index]->m_brush = Qt::green;

                // Обновление узла на сцене
                nodes[index]->update();

                // закрашивание предыдущий серым
                if (prev_index != -1)
                {
                    nodes[prev_index]->m_brush = Qt::gray;
                    nodes[prev_index]->update();
                }
            });
        }
        // очистка цвета узлов
        QTimer::singleShot(path.size() * 800 + 500, this, [=]() { clearNodesColor(); });

    }

    // Рандомизация матрицы и создания узлов
    void randomizeAdjacencyMatrix(int size)
    {
        // Очистка текущих узлов и матрицы смежности
        clearScene();

        // Создание новой матрицы смежности
        adjacencyMatrix.clear();
        for (int i = 0; i < size; ++i)
        {
            QVector<int> row(size, 0);
            adjacencyMatrix.append(row);
        }

        // Заполнение матрицы смежности случайными значениями
        for (int i = 0; i < size; ++i)
        {
            for (int j = i + 1; j < size; ++j)
            {
                // Вес от 1 до 50
                int weight = qrand() % 50 + 1;
                adjacencyMatrix[i][j] = weight;
                adjacencyMatrix[j][i] = weight;
            }
        }

        // Создание узлов
        nodes.clear();
        for (int i = 0; i < size; ++i)
        {
            QPointF position(qrand() % 400, qrand() % 400); // Случайная позиция на сцене
            QString data = QString::number(i + 1); // Данные узла
            Node* node = new Node(i, position, data, 60); // Создание узла
            node->setPos(position);
            nodes.append(node);

            // Подключение сигналов узла
            connect(node, &Node::nodePressed, this, &Graph::handleNodePressed);
            connect(node, &Node::updateLinksSignal, this, &Graph::handleUpdateLinksSignal);
        }

        // Отрисовка узлов
        drawNodes();

        // Отрисовка связей
        drawLinks();

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
        if (!needToLink && !needToDelete && !needToDeleteLink && !needToSolveTask && !needDeixtra)
        {
            // Очистка цвета узлов
            clearNodesColor();
            return;
        }

        // Проверяем, есть ли уже выделенный узел
        if ((needToLink || needToDeleteLink) && selectedNodeIndex != -1)
        {
            bool confirm = true;
            if (needToLink)
            {
                qDebug() << "номер нажатого:" << index + 1;

                // Предложение пользователю ввести вес связи
                QString text = QInputDialog::getText(nullptr, "Введите вес связи",
                                                     "Вес связи:", QLineEdit::Normal,
                                                     "", &confirm);

                if (confirm && !text.isEmpty())
                {
                    int weight = text.toInt(); // Преобразование строки в целое число
                    adjacencyMatrix[index][selectedNodeIndex] = weight;
                    if (needTwoWayAddition) adjacencyMatrix[selectedNodeIndex][index] = weight;
                }
                // Убираем флаг на добавление узла
                needToLink = false;
            }
            else if (needToDeleteLink)
            {
                adjacencyMatrix[index][selectedNodeIndex] = 0;
                if (needTwoWayAddition) adjacencyMatrix[selectedNodeIndex][index] = 0;
                needToDeleteLink = false;
            }

            // Очистка цвета узлов
            clearNodesColor();

            // Обновляем состояние выделенного узла
            selectedNodeIndex = -1;
        }
        else if (needToLink || needToDeleteLink)
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

        // --------------------------------Задача Коммивояжера------------------------------------------------------------------

        else if (needToSolveTask)
        {
            // вызов метода решения задачи Комивояжера, принимающий индекс узла в матрице смежности
            qDebug() << "Вы решили задачу коммивояжера!";

            // Здесь можно обработать результат, например, вывести путь или обновить интерфейс
            QVector<int> path = solveTravelingSalesmanProblem(index);

            qDebug() << path;

            QString path_text = "Ответ на задачу коммивояжера: " + QString::number(path[0] + 1);

            for (int i = 1; i < path.size(); ++i)
            {
                path_text += " -> " +  QString::number(path[i] + 1);
            }

            emit textToStatusBar(path_text);

            // Отрисовка найденного пути
            highlightPath(path);

            needToSolveTask = false;
        }

        // --------------------------------Алгоритм Дейкстры---------------------------------------------------------------

        else if (needDeixtra)
        {
            qDebug() << "Алгоритм Дейкстры!";

            QVector<QVector<int>> paths = shortestPaths(index);

            for (int i = 0; i < paths.size(); ++i)
            {
                qDebug() << paths[i];
            }

            needDeixtra = false;
        }

        // ----------------------------------------------------------------------------------------------------------------

        nodes[index]->m_isMovable = true;

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

    // Очистка цвета узлов
void clearNodesColor()
{
    for (Node *node : nodes)
    {
        QBrush brush(Qt::white);
        node->m_brush = brush;
        node->update();
    }
}

};

#endif // GRAPH_H
