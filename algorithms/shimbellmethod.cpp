#include "shimbellmethod.h"


QVector<QVector<int>> getShortestPathsMatrixBuShimbell(const QVector<QVector<int>>& adjacencyMatrix)
{
    QVector<QVector<int>> shortestPathsMatrix = adjacencyMatrix;

    for (int i = 0; i < adjacencyMatrix.size(); i++)
    {
        specificMatrixProduct(shortestPathsMatrix, adjacencyMatrix);
    }

    return shortestPathsMatrix;
}

void specificMatrixProduct(QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix)
{
    QVector<QVector<int>> matrixProd;

    for (int i = 0; i < firstMatrix.size(); i++)
    {
        QVector<int> sumLine;
        for (int j = 0; j < secondMatrix[0].size(); j++)
        {
            int minElement = 0;
            for (int k = 0; k < firstMatrix[0].size(); k++)
            {
                if (firstMatrix[i][k] * secondMatrix[k][j] != 0)
                {
                    int element = firstMatrix[i][k] + secondMatrix[k][j];
                    if (minElement == 0 || element < minElement) minElement = element;
                }
            }

            sumLine.push_back(minElement);

        }
        matrixProd.push_back(sumLine);
    }

    for (auto i : matrixProd)
    {
        qDebug() << i;
    }
    qDebug() << "";

    firstMatrix = matrixProd;
}
