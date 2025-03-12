#ifndef KRUASKALMETHOD_H
#define KRUASKALMETHOD_H

#include <QSet>
#include <QVector>

QVector<QVector<int>> getSpanningTreeByKruskal(const QVector<QVector<int>>& adjacencyMatrix);

QVector<QPair<int, int>> getAllOrderedConnection(const QVector<QVector<int>>& adjacencyMatrix);

template <typename T>
int findSetContainingElement(const QVector<T>& vectorOfSets, const typename T::value_type& element);


#endif // KRUASKALMETHOD_H
