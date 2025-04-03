#include "clique.h"


QVector<int> findMaxClique(const QVector<QVector<int>>& adjacencyMatrix)
{
    QVector<int> maxClique = {};

    for (int i = 0; i < adjacencyMatrix.size(); i++)
    {
        QVector<int> vertexNotExpandClique = getVertexNotExpandClique(adjacencyMatrix, {i});
        QVector<int> currentClique = findCliqueRec(adjacencyMatrix, {i}, vertexNotExpandClique);
        if (currentClique.size() > maxClique.size()) maxClique = currentClique;
    }

    return maxClique;
}


QVector<int> findCliqueRec(const QVector<QVector<int>>& adjacencyMatrix, QVector<int> clique, QVector<int> vertexNotExpandClique)
{
    if (clique.size() + vertexNotExpandClique.size() == adjacencyMatrix.size()) return clique;
}

QVector<int> getVertexNotExpandClique(const QVector<QVector<int>>& adjacencyMatrix, const QVector<int> &clique)
{
    QVector<int> vertexNotExpandClique = {};

    for (int i = 0; i < adjacencyMatrix.size(); i++)
    {
        for (int j = 1; j < clique.size(); j++)
        {
            if ((adjacencyMatrix[clique[0]][i] == 0 && clique[0] != i) ||
                    (adjacencyMatrix[clique[0]][i] != adjacencyMatrix[clique[j]][i] && i != j))
            {
                vertexNotExpandClique.push_back(i);
                break;
            }
        }
    }

    return vertexNotExpandClique;
}
