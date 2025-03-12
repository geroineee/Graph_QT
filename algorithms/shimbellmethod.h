#ifndef SHIMBELLMETHOD_H
#define SHIMBELLMETHOD_H

#include <QVector>
#include <QDebug>

QVector<QVector<int>> getShortestPathsMatrixBuShimbell(const QVector<QVector<int>>& adjacencyMatrix);

void specificMatrixProduct(QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix);

#endif // SHIMBELLMETHOD_H
