#ifndef SHIMBELLMETHOD_H
#define SHIMBELLMETHOD_H

#include <QVector>
#include <QDebug>

QVector<QVector<int>> getShortestPathsMatrixByShimbell(const QVector<QVector<int>>& adjacencyMatrix, const int& countConnection, const bool& findMin = true);

QVector<QVector<int>> specificMatrixProduct(const QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix, const bool& findMin);

#endif // SHIMBELLMETHOD_H
