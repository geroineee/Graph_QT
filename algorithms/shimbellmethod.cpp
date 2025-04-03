#include "shimbellmethod.h"


QVector<QVector<int>> getShortestPathsMatrixByShimbell(const QVector<QVector<int>>& adjacencyMatrix, const int& countConnection, const bool& findMin)
{
    QVector<QVector<int>> shortestPathsMatrix = adjacencyMatrix;

    for (int i = 0; i < countConnection; i++)
    {
        shortestPathsMatrix = specificMatrixProduct(shortestPathsMatrix, adjacencyMatrix, findMin);
    }

    return shortestPathsMatrix;
}

QVector<QVector<int>> specificMatrixProduct(const QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix, const bool& findMin)
{
    QVector<QVector<int>> matrixProd;

    for (int i = 0; i < firstMatrix.size(); i++)
    {
        QVector<int> sumLine;
        for (int j = 0; j < secondMatrix[0].size(); j++)
        {
            int minOrMaxElement = 0;
            for (int k = 0; k < firstMatrix[0].size(); k++)
            {
                if (firstMatrix[i][k] * secondMatrix[k][j] != 0)
                {
                    int element = firstMatrix[i][k] + secondMatrix[k][j];
                    if (findMin)
                    {
                        if (minOrMaxElement == 0 || element < minOrMaxElement) minOrMaxElement = element;
                    }
                    else
                    {
                        if (minOrMaxElement == 0 || element > minOrMaxElement) minOrMaxElement = element;
                    }
                }
            }

            sumLine.push_back(minOrMaxElement);

        }
        matrixProd.push_back(sumLine);
    }

    return matrixProd;
}
