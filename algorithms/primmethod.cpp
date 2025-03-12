#include "primmethod.h"

QVector<QVector<int>> getSpanningTreeByPrima(const QVector<QVector<int>>& adjacencyMatrix)
{
    if (adjacencyMatrix.isEmpty()) return {};

    QVector<QVector<int>> spanningTree(adjacencyMatrix.size(), QVector<int>(adjacencyMatrix.size(), 0));
    QVector<bool> handledNode(adjacencyMatrix.size(), false);
    int lastHandedNode = 0;

    for (int i = 0; i < adjacencyMatrix.size()-1; i++)
    {
        handledNode[lastHandedNode] = true;

        QPair<int, int> minConnection = getMinConnectionFromHandledNodes(adjacencyMatrix, handledNode);

        spanningTree[minConnection.first][minConnection.second] = adjacencyMatrix[minConnection.first][minConnection.second];
        spanningTree[minConnection.second][minConnection.first] = adjacencyMatrix[minConnection.first][minConnection.second];

        lastHandedNode = minConnection.second;
    }

    return spanningTree;
}

QPair<int, int> getMinConnectionFromHandledNodes(const QVector<QVector<int>>& adjacencyMatrix, const QVector<bool> &handledNode)
{
    QPair<int, int> minConnection = {0, 0};

    for (int i = 0; i < adjacencyMatrix.size(); i++)
    {
        if (!handledNode[i]) continue;

        for (int j = 0; j < adjacencyMatrix.size(); j++)
        {
            if (handledNode[j] || adjacencyMatrix[i][j] == 0) continue;

            if (minConnection == QPair<int,int>{0, 0} ||
                    adjacencyMatrix[i][j] < adjacencyMatrix[minConnection.first][minConnection.second])
            {
                minConnection = {i, j};
            }
        }
    }

    return minConnection;
}
