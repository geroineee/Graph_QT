#ifndef GRAPH_H
#define GRAPH_H

#include <QVector>

class Graph
{
public:
    Graph();

public:
    // Создание матрицы смежности
    QVector<QVector<int>> adjacencyMatrix;

};

#endif // GRAPH_H
