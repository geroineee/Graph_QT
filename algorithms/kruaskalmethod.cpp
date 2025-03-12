#include "kruaskalmethod.h"

QVector<QVector<int>> getSpanningTreeByKruskal(const QVector<QVector<int>>& adjacencyMatrix)
{
    if (adjacencyMatrix.isEmpty()) return {};

    QVector<QVector<int>> spanningTree(adjacencyMatrix.size(), QVector<int>(adjacencyMatrix.size(), 0));
    QVector<QPair<int, int>> sortedConnectionVector = getAllOrderedConnection(adjacencyMatrix);
    QVector<QSet<int>> vertexSets;

    for (int i = 0; i < adjacencyMatrix.size(); i++) vertexSets.push_back({i});

    for (int i = 0; i < sortedConnectionVector.size(); i++)
    {
        QPair<int, int> currentPair = sortedConnectionVector[i];

        if (vertexSets.size() == 1) break;

        int firstIndex = findSetContainingElement(vertexSets, currentPair.first);
        int secondIndex = findSetContainingElement(vertexSets, currentPair.second);

        if (firstIndex == secondIndex || firstIndex == -1 || secondIndex == -1) continue;

        spanningTree[currentPair.first][currentPair.second] = adjacencyMatrix[currentPair.first][currentPair.second];
        spanningTree[currentPair.second][currentPair.first] = adjacencyMatrix[currentPair.first][currentPair.second];

        vertexSets[firstIndex].unite(vertexSets[secondIndex]);
        vertexSets.removeAt(secondIndex);
    }

    return spanningTree;
}

QVector<QPair<int, int>> getAllOrderedConnection(const QVector<QVector<int>>& adjacencyMatrix)
{
    QVector<QPair<int, int>> sortedConnectionVector;

    for (int i = 0; i < adjacencyMatrix.size(); i++)
    {
        for (int j = i+1; j < adjacencyMatrix.size(); j++)
        {
            if (adjacencyMatrix[i][j] > 0) sortedConnectionVector.push_back({i, j});
        }
    }

    std::sort(sortedConnectionVector.begin(), sortedConnectionVector.end(),
              [&adjacencyMatrix](const QPair<int, int> &firstEl, const QPair<int, int> &secondEl) {
                    return adjacencyMatrix[firstEl.first][firstEl.second] < adjacencyMatrix[secondEl.first][secondEl.second];
              });

    return sortedConnectionVector;
}

template <typename T>
int findSetContainingElement(const QVector<T>& vector, const typename T::value_type& element)
{
    auto it = std::find_if(vector.begin(), vector.end(),
                     [element](const T& set) {
                         return set.contains(element);
                     });
    if (it != vector.end()) return std::distance(vector.begin(), it);
    return -1;
}
