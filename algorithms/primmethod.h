#ifndef PRIMMETHOD_H
#define PRIMMETHOD_H

#include <QVector>

QVector<QVector<int>> getSpanningTreeByPrima(const QVector<QVector<int>>& adjacencyMatrix);

QPair<int, int> getMinConnectionFromHandledNodes(const QVector<QVector<int>>& adjacencyMatrix, const QVector<bool>& handledNode);

#endif // PRIMMETHOD_H
