#include "utils.h"

QFile* tryToOpenFile(const QString& file_path, bool isTranc)
{
    QFile *file = new QFile;
    file->setFileName(file_path);
    isTranc ? file->open(QIODevice::WriteOnly | QIODevice::Truncate) : file->open(QIODevice::ReadWrite);
    if (!file->isOpen())
    {
        delete file;
        QMessageBox::critical(nullptr, "Ошибка", "Ошибка при открытии файла по пути:\n" + file_path);
        return nullptr;
    }
    return file;
}

QVector<int> vectorStringToInt(const QVector<QString>& vect)
{
    QVector<int> intVect;
    bool ok;
    for (int i = 0; i < vect.size(); i++)
    {
        int num = vect[i].toInt(&ok);
        if (!ok) return {};
        intVect.push_back(num);
    }
    return intVect;
}

QVector<QVector<int>> readMatrixFromFile(QFile* file, int& nextLineNum)
{
    if (file == nullptr)
        return QVector<QVector<int>> {};

    QVector<QVector<int>> matrix;
    bool findSeparatorSymbol = false;
    int countReadingLine = 0;

    while(!findSeparatorSymbol)
    {
        QByteArray data = file->readLine();
        countReadingLine++;
        if (countReadingLine > nextLineNum)
        {
            nextLineNum++;
            QString text = QString::fromUtf8(data);
            if (text[0] == '=' || text.isEmpty())
            {
                findSeparatorSymbol = true;
            }
            else
            {
                QVector<QString> line = text.split(' ').toVector();
                matrix.push_back(vectorStringToInt(line));
            }
        }
    }
    file->close();

    return matrix;
}

QVector<QVector<int>> getMatrixProduct(const QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix)
{
    QVector<QVector<int>> matrixProduct;
    if (firstMatrix[0].size() != secondMatrix.size())
        return {};

    for (int i = 0; i < firstMatrix.size(); i++)
    {
        QVector<int> productRow;
        for (int j = 0; j < secondMatrix[0].size(); j++)
        {
            int productElement = 0;
            for (int k = 0; k < firstMatrix[0].size(); k++)
            {
                productElement += firstMatrix[i][k] * secondMatrix[k][j];
            }
            productRow.push_back(productElement);
        }
        matrixProduct.push_back(productRow);
    }

    return matrixProduct;
}

QVector<QVector<int>> getReachabilityMatrix(const QVector<QVector<int>>& adjacencyMatrix)
{
    QVector<QVector<int>> reachabilityMatrix = getMatrixSum(getUnitMatrix(adjacencyMatrix), adjacencyMatrix);
    QVector<QVector<int>> poweredAdjacencyMatrix = adjacencyMatrix;

    for (int i = 0; i < adjacencyMatrix.size()-1; i++)
    {
        poweredAdjacencyMatrix = getMatrixProduct(poweredAdjacencyMatrix, adjacencyMatrix);
        reachabilityMatrix = getMatrixSum(reachabilityMatrix, poweredAdjacencyMatrix);
    }

    return reachabilityMatrix;
}

QVector<QVector<int>> getUnitMatrix(const QVector<QVector<int>>& matrix)
{
    if(matrix.isEmpty() || matrix.size() != matrix[0].size()) return {};

    QVector<QVector<int>> unitMatrix(matrix.size(), QVector<int>(matrix[0].size(), 0));

    for (int i = 0; i < matrix.size(); i++)
    {
        unitMatrix[i][i] = 1;
    }

    return unitMatrix;
}

QVector<QVector<int>> getMatrixSum(const QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix)
{
    QVector<QVector<int>> matrixSum;

    for (int i = 0; i < firstMatrix.size(); i++)
    {
        QVector<int> sumLine;
        for (int j = 0; j < secondMatrix[0].size(); j++)
        {
            sumLine.push_back(firstMatrix[i][j] || secondMatrix[i][j]);
        }
        matrixSum.push_back(sumLine);
    }

    return matrixSum;
}

QVector<QVector<int>> getStrongConnectedMatrix(const QVector<QVector<int>>& reachabilityMatrix)
{
    QVector<QVector<int>> strongConnectedMatrix(reachabilityMatrix.size(), QVector<int>(reachabilityMatrix[0].size(), 0));
    for (int i = 0; i < reachabilityMatrix.size(); i++)
    {
        for (int j = i; j < reachabilityMatrix[0].size(); j++)
        {
            if (reachabilityMatrix[i][j] & reachabilityMatrix[j][i])
            {
                strongConnectedMatrix[i][j] = 1;
                strongConnectedMatrix[j][i] = 1;
            }
        }
    }
    return strongConnectedMatrix;
}

QVector<QVector<int>> getStrongComponentsConnectivity(const QVector<QVector<int>>& adjacencyMatrix)
{
    QVector<QVector<int>> reachabilityMatrix = getReachabilityMatrix(adjacencyMatrix);
    QVector<QVector<int>> strongConnectedMatrix = getStrongConnectedMatrix(reachabilityMatrix);

    QVector<QVector<int>> strongComponentsConnectivity;
    QVector<bool> visitedPoints(strongConnectedMatrix.size(), false);

    for (int i = 0; i < strongConnectedMatrix.size(); i++)
    {
        if (visitedPoints[i])
            continue;
        QVector<int> currentComponent;
        for (int j = 0; j < strongConnectedMatrix[0].size(); j++)
        {
            if (strongConnectedMatrix[i][j] == 1 && visitedPoints[j] == false)
            {
                currentComponent.push_back(j);
                visitedPoints[j] = true;
            }
        }
        if (!currentComponent.isEmpty())
            strongComponentsConnectivity.push_back(currentComponent);
    }
    return strongComponentsConnectivity;
}

int messageBoxYesNoQuestion(QWidget* parent, const QString& header, const QString& text)
{
    QMessageBox messageBox(QMessageBox::Question, header, text, QMessageBox::Yes | QMessageBox::No, parent);
    messageBox.setButtonText(QMessageBox::Yes, "Да");
    messageBox.setButtonText(QMessageBox::No, "Нет");

    return messageBox.exec();
}


QVector<QVector<int>> getWeakComponentsConnectivity(const QVector<QVector<int>>& pathsFromAllPoint, const int& adjacencyMatrixSize)
{
    QVector<QVector<int>> weakComponentsConnectivity;
    QVector<bool> findedPoint(adjacencyMatrixSize, false);

    for (int i = 0; i < adjacencyMatrixSize; i++)
    {
        if (!findedPoint[i])
        {
            QVector<int> componentConnectivity = {};
            for (int j = 0; j < pathsFromAllPoint.size(); j++)
            {
                if (pathsFromAllPoint[j].contains(i))
                {
                    mergeVector(componentConnectivity, pathsFromAllPoint[j]);
                }
            }
            if (componentConnectivity.size() > 0)
            {
                std::sort(componentConnectivity.begin(), componentConnectivity.end());
                weakComponentsConnectivity.push_back(componentConnectivity);
                for (int k = 0; k < componentConnectivity.size(); k++)
                {
                    findedPoint[componentConnectivity[k]] = true;
                }
            }
        }
    }

    return weakComponentsConnectivity;
}

void mergeVector(QVector<int>& firstVector, const QVector<int>& secondVector)
{
    firstVector.append(secondVector);
    QSet<int> set = QSet<int>::fromList(QList<int>::fromVector(firstVector));
    firstVector = QVector<int>::fromList(set.toList());
}

QString vectorComponentsToString(const QVector<QVector<int>>& vectorComponents)
{
    QString text;
    for (int i = 0; i < vectorComponents.size(); i++)
    {
        if (vectorComponents[0].size() > 0)
            text += QString::number(i+1) + ") " + QString::number(vectorComponents[i][0]+1);
        for (int j = 1; j< vectorComponents[i].size(); j++)
        {
            text += ", " + QString::number(vectorComponents[i][j]+1);
        }
        text += "\n";
    }

    return text;
}

void saveAdjacencyMatrix(QSettings& settings, const QVector<QVector<int>>& adjacencyMatrix)
{
    settings.beginGroup("adjacencyMatrix");
    settings.setValue("matrixDimension", adjacencyMatrix.size());

    for (int i = 0; i < adjacencyMatrix.size(); i++)
    {
        for (int j = 0; j < adjacencyMatrix[i].size(); j++)
        {
            settings.setValue(QString("matrixElement_%1_%2").arg(i).arg(j), adjacencyMatrix[i][j]);
        }
    }
    settings.endGroup();
}

void saveNodeCoordinates(QSettings& settings, const QVector<Node*>& nodes)
{
    settings.beginGroup("vertexCoordinates");
    for (int i = 0; i < nodes.size(); i++)
    {
        QPointF position = nodes[i]->pos();
        settings.setValue(QString("node_%1_X").arg(i), position.x());
        settings.setValue(QString("node_%1_Y").arg(i), position.y());
    }
    settings.endGroup();
}

QVector<QVector<int>> loadAdjacencyMatrix(QSettings& settings)
{
    QVector<QVector<int>> adjacencyMatrix;

    settings.beginGroup("adjacencyMatrix");
    int matrixSize = settings.value("matrixDimension").toInt();
    adjacencyMatrix.resize(matrixSize);

    for (int i = 0; i < matrixSize; i++)
    {
        adjacencyMatrix[i].resize(matrixSize);
        for (int j = 0; j < matrixSize; j++)
        {
            adjacencyMatrix[i][j] = settings.value(QString("matrixElement_%1_%2").arg(i).arg(j)).toInt();
        }
    }
    settings.endGroup();

    return adjacencyMatrix;
}

void loadNodeCoordinates(QSettings& settings, const QVector<Node*>& nodes)
{
    settings.beginGroup("vertexCoordinates");
    for (int i = 0; i < nodes.size(); i++)
    {
        QPointF position;
        position.setX(settings.value(QString("node_%1_X").arg(i)).toReal());
        position.setY(settings.value(QString("node_%1_Y").arg(i)).toReal());
        nodes[i]->setPos(position);
    }
    settings.endGroup();
}
