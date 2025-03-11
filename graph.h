#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>
#include <QQueue>
#include <QGraphicsScene>

#include <QMessageBox>

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

    // матрица для отрисовки
    QVector<QVector<int>> drawingMatrix;

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

    // Алгоритм Флойда
    bool needFloid = false;

    // Обход в глубину
    bool needInDeep = false;

    // Обход в ширину
    bool needInWidth = false;


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

    QVector<Node*> getNodes() {return nodes;}

public:
    void setDrawinMatrix(const QVector<QVector<int>> matrix)
    {
        drawingMatrix = matrix;
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
        drawingMatrix.clear();

        // Очищаем вектор nodes
        nodes.clear();

        // Очистка узлов
        clearLinks();

        // обновление сцены
//        drawNodes();
        drawLinks({});

        // Отправляем сигнал об изменении матрицы смежности
        emit adjacencyMatrixChanged(adjacencyMatrix);
    }

    void turn_off_buttons()
    {
        QVector<bool*> all_buttons = { &needToLink, &needToDelete, &needToDeleteLink, &needToSolveTask,
                                       &needDeixtra, &needInDeep, &needInWidth, &needFloid };
        for (bool* flag : all_buttons)
        {
            *flag = false;
        }
    }


    // отрисовка узлов
    void drawNodes()
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i]->scene() != scene)
            {
                scene->addItem(nodes[i]);
            }
            nodes[i]->update();
        }
    }

    // отрисовка связей
    void drawLinks(const QVector<QVector<int>>& matrix)
    {
       // Удаляем все связи из слоя
       clearLinks();
       drawNodes();

       for (int i = 0; i < matrix.size(); ++i)
       {
           for (int j = 0; j < matrix[i].size(); ++j)
           {
                if (matrix[i][j] > 0)
               {
                    // Добавление связи с весом на слой
                    Link* weightLink = new Link(nodes[i], nodes[j], matrix[i][j]);
                    linkLayer->addToGroup(weightLink);

                    // Добавление стрелки на слой
                    Arrow* arrow = new Arrow(nodes[j], nodes[i]);
                    linkLayer->addToGroup(arrow);
               }
           }
       }

       scene->update();
    }

    Node* inicializationNode(const int& index, const QString& nodeData, const QPointF& position)
    {
        Node* node = new Node(index, position, nodeData, 60);
        node->setPos(position);

        connect(node, &Node::nodePressed, this, &Graph::handleNodePressed);
        connect(node, &Node::updateLinksSignal, this, &Graph::handleUpdateLinksSignal);

        return node;
    }

    Node* inicializationNodeRandPosition(const int& index, const QString& nodeData)
    {
        QPointF position(qrand() % 400, qrand() % 400);
        Node* node = inicializationNode(index, nodeData, position);

        return node;
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

        nodes.append(inicializationNodeRandPosition(nodes.size(), text));

        // Добавляем новую строку и столбец в матрицу смежности
        for (int i = 0; i < nodes.size() - 1; ++i)
        {
            adjacencyMatrix[i].append(0);
        }
        adjacencyMatrix.append(QVector<int>(nodes.size(), 0));

        // Отправляем сигнал об изменении матрицы смежности
        emit adjacencyMatrixChanged(adjacencyMatrix);
    }

    void addNodeFromNewMatrix(QVector<QVector<int>> matrix)
    {
        for (int i=1; i <= matrix.size(); i++)
        {
            nodes.append(inicializationNodeRandPosition(nodes.size(), QString::number(i)));
        }
        adjacencyMatrix=matrix;
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
        nodes.erase(nodes.begin() + index);

        // Обновление индексов узлов
        for (int i = index; i < nodes.size(); ++i)
        {
            nodes[i]->m_index = i;
        }

        // Обновление матрицы смежности
        adjacencyMatrix.erase(adjacencyMatrix.begin() + index); // Удаление строки

        for (auto& row : adjacencyMatrix)
        {
            row.erase(row.begin() + index); // Удаление столбца
        }

        // Отправляем сигнал об изменении матрицы смежности
        emit adjacencyMatrixChanged(adjacencyMatrix);
    }

    // Решение задачи коммивояжера
    QVector<int> solveTravelingSalesmanProblem(int startIndex)
    {
        // Возвращаем пустой вектор, если граф пуст или индекс некорректен
        if (nodes.isEmpty() || startIndex < 0 || startIndex >= nodes.size())
            return QVector<int>();

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

    int calculatePathWeight(const QVector<int>& path)
    {
        int totalWeight = 0;

        for (int i = 0; i < path.size() - 1; ++i)
        {
            int fromNode = path[i];
            int toNode = path[i + 1];
            totalWeight += adjacencyMatrix[fromNode][toNode];
        }

        return totalWeight;
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

        for (int i = 0; i < paths.size(); ++i)
        {
            paths[i].push_front(startIndex);
        }

        return paths;
    }

    // Визуализация обхода
    void highlightPath(const QVector<int>& path)
    {

        for (int i = 0; i < path.size(); ++i)
        {
            QTimer::singleShot( i*1000, [this, path, i]()
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
        QTimer::singleShot(path.size() * 1000 + 500, this, [=]() { clearNodesColor(); });
    }

    // Обход в глубину
    void depthFirstSearch(int startIndex, QVector<bool>& visited, QVector<int>& path)
    {
        if (startIndex < 0 || startIndex >= nodes.size() || visited[startIndex])
            return;

        // Отмечаем узел как посещенный
        visited[startIndex] = true;

        // Добавляем узел в путь
        path.push_back(startIndex);

        // Рекурсивно обходим всех непосещенных соседей
        for (int i = 0; i < nodes.size(); ++i)
        {
            if (adjacencyMatrix[startIndex][i] > 0 && !visited[i])
            {
                depthFirstSearch(i, visited, path);
            }
        }
    }

    // Обход в ширину
    void widthFirstCrawl(int startIndex, QVector<int>& path)
    {
        QVector<bool> visited(nodes.size(), false);
        QQueue<int> queue;

        visited[startIndex] = true;
        queue.enqueue(startIndex);

        while (!queue.isEmpty())
        {
            int currentNode = queue.dequeue();
            path.append(currentNode);

            for (int i = 0; i < nodes.size(); ++i)
            {
                if (adjacencyMatrix[currentNode][i] > 0 && !visited[i])
                {
                    visited[i] = true;
                    queue.enqueue(i);
                }
            }
        }
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

        QPointF center = {600/2, 576/2};
        qreal shift_angle = 360 / size;
        qreal lenght =  30 * size;

        // Создание узлов
        nodes.clear();
        for (int i = 0; i < size; ++i)
        {
            qreal angle = i * shift_angle;
            qreal delta_y = sin(angle / 57.295779513) * lenght;
            qreal delta_x = cos(angle / 57.295779513) * lenght;
            QPointF position(center.x() + delta_x, center.y() + delta_y);
            QString nodeData = QString::number(i + 1);
            Node* node = inicializationNodeRandPosition(i, nodeData);
            node->setPos(position);
            nodes.append(node);
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
        if (!needToLink && !needToDelete && !needToDeleteLink && !needToSolveTask && !needDeixtra && !needInDeep && !needInWidth && !needFloid)
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
                // Предложение пользователю ввести вес связи
                QString text = QInputDialog::getText(nullptr, "Введите вес связи",
                                                     "Вес связи:", QLineEdit::Normal,
                                                     "", &confirm);

                if (confirm && !text.isEmpty())
                {
                    int weight = text.toInt(); // Преобразование строки в целое число
                    adjacencyMatrix[selectedNodeIndex][index] = weight;
                    if (needTwoWayAddition)
                    {
                        adjacencyMatrix[index][selectedNodeIndex] = weight;
                    }
                }
                // Убираем флаг на добавление узла
                needToLink = false;
            }
            else if (needToDeleteLink)
            {
                adjacencyMatrix[selectedNodeIndex][index] = 0;
                if (needTwoWayAddition) adjacencyMatrix[index][selectedNodeIndex] = 0;
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
            needToSolveTask = false;

            // вызов метода решения задачи Комивояжера, принимающий индекс узла в матрице смежности
            qDebug() << "Вы решили задачу коммивояжера!";

            // Здесь можно обработать результат, например, вывести путь или обновить интерфейс
            QVector<int> path = solveTravelingSalesmanProblem(index);

            qDebug() << path;

            // Формирование строки для вывода в statusbar
            QString *path_text = new QString;

            *path_text = "Ответ на задачу коммивояжера: " + QString::number(path[0] + 1);

            for (int i = 1; i < path.size(); ++i)
            {
                *path_text += " -> " +  QString::number(path[i] + 1);
            }

            // Вычисление веса всего пути и запись в statusbar
            *path_text += ". Длина вычисленного пути: " + QString::number(calculatePathWeight(path));

            emit textToStatusBar(*path_text);

            delete path_text;

            // Отрисовка найденного пути
            highlightPath(path);
        }

        // --------------------------------Алгоритм Дейкстры---------------------------------------------------------------

        else if (needDeixtra)
        {
            needDeixtra = false;

            QVector<QVector<int>> paths = shortestPaths(index);

            QString *text = new QString;

            *text = "Кратчайшие пути из узла во все другие.\n";

            for (int i = 0; i < paths.size(); ++i)
            {
                if (paths[i].size() != 1)
                {
                    *text += "\nИз " + QString::number(index + 1) + " в " +  QString::number(i+1) + ": ";

                    *text += QString::number(paths[i][0] + 1);
                    for (int j = 1; j < paths[i].size(); ++j)
                    {
                        *text += " -> " + QString::number(paths[i][j] + 1);
                    }
                    *text += ". Стоимость пути: " + QString::number(calculatePathWeight(paths[i])) + "\n";
                }
            }

            QMessageBox msgBox;
            msgBox.setWindowTitle("Алгоритм Дейкстры");
            msgBox.setInformativeText(*text);
            msgBox.adjustSize();
            msgBox.exec();

            delete text;
        }

        // --------------------------------Алгоритм Флойда---------------------------------------------------------------

        else if (needFloid)
        {
            needFloid = false;
            QStringList all_paths;

            for (int index = 0; index < nodes.size(); index++)
            {
                QVector<QVector<int>> paths = shortestPaths(index);
                for (int i = 0; i < paths.size(); ++i)
                {
                    if (paths[i].size() != 1)
                    {
                        QString text = "\nИз " + QString::number(index + 1) + " в " +  QString::number(i+1) + ": ";

                        text += QString::number(paths[i][0] + 1);
                        for (int j = 1; j < paths[i].size(); ++j)
                        {
                            text += " -> " + QString::number(paths[i][j] + 1);
                        }
                        text += ". Стоимость пути: " + QString::number(calculatePathWeight(paths[i])) + "\n";
                        all_paths.append(text);
                    }
                }
            }

            QMessageBox msgBox;
            msgBox.setWindowTitle("Алгоритм Флойда");

            QString text_for_msg = "";
            for (int i = 0; i < all_paths.size(); i++)
            {
                text_for_msg += all_paths[i];
            }

            msgBox.setInformativeText(text_for_msg);
            msgBox.adjustSize();
            msgBox.exec();

        }

        // ----------------------------------------Обход в глубину---------------------------------------------------

        else if (needInDeep)
        {
            needInDeep = false;

            QVector<bool> visited(nodes.size(), false);
            depthFirstSearch(index, visited, path);

            qDebug() << path;
            highlightPath(path);
            path.clear();
        }

        // ----------------------------------------Обход в ширину---------------------------------------------------

        else if (needInWidth)
        {
            needInWidth = false;

            widthFirstCrawl(index, path);

            qDebug() << path;
            highlightPath(path);
            path.clear();
        }

        // ----------------------------------------------------------------------------------------------------------

        // Отправляем сигнал об изменении матрицы смежности
        emit adjacencyMatrixChanged(adjacencyMatrix);
    }

    // слот для обновления связей
    void handleUpdateLinksSignal()
     {
        drawLinks(drawingMatrix);
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
