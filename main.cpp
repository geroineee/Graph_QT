#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPainter>
#include <QVector>
#include <QTime>

#include <QDebug>

#include "Node.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Инициализируем генератор случайных чисел
    qsrand(QTime::currentTime().msec());

    // Создание сцены и виджета QGraphicsView
    QGraphicsScene scene;
    QGraphicsView view(&scene);

    view.setRenderHint(QPainter::Antialiasing);

    view.setGeometry(700, 300, 600, 600);

    view.show();

    // Создание матрицы смежности (пример)
    QVector<QVector<int>> adjacencyMatrix =
    {
        {0, 0, 0, 1},
        {0, 0, 1, 0},
        {0, 1, 0, 0},
        {1, 0, 0, 0}
    };

    // Создание узлов и их добавление на сцену
    QVector<Node*> nodes;
    for (int i = 0; i < adjacencyMatrix.size(); ++i)
    {
        QPointF position(qrand() % 400, qrand() % 400); // Произвольное расположение узла
        Node* node = new Node(i, position, QString::number(i));
        node->setPos(position);
        qDebug() << node->pos();
        scene.addItem(node);
        nodes.append(node);
    }

    //Отрисовка рёбер между узлами
    for (int i = 0; i < adjacencyMatrix.size(); ++i)
    {
        for (int j = 0; j < adjacencyMatrix[i].size(); ++j)
        {
            if (adjacencyMatrix[i][j] > 0)
            {
                QPointF sourcePos = nodes[i]->scenePos();
                QPointF destPos = nodes[j]->scenePos();
                scene.addLine(sourcePos.x(), sourcePos.y(), destPos.x(), destPos.y());
            }
        }
    }

    return a.exec();
}
