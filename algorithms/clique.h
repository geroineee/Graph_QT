#ifndef CLIQUE_H
#define CLIQUE_H

#include <QVector>

QVector<int> findMaxClique(const QVector<QVector<int>>& adjacencyMatrix);

QVector<int> findCliqueRec(const QVector<QVector<int>>& adjacencyMatrix, QVector<int> clique, QVector<int> vertexNotExpandClique);

QVector<int> getVertexNotExpandClique(const QVector<QVector<int>>& adjacencyMatrix, const QVector<int>& clique);

#endif // CLIQUE_H
