#ifndef UTILS_H
#define UTILS_H

#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSet>
#include <QSettings>

#include "graph.h"

QFile* tryToOpenFile(const QString& file_path, bool isTranc = false);

QVector<int> vectorStringToInt(const QVector<QString>& vect);

QVector<QVector<int>> readMatrixFromFile(QFile* file, int& nextLineNum);

QVector<QVector<int>> getMatrixProduct(const QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix);

QVector<QVector<int>> getReachabilityMatrix(const QVector<QVector<int>>& adjacencyMatrix);

QVector<QVector<int>> getUnitMatrix(const QVector<QVector<int>>& matrix);

QVector<QVector<int>> getMatrixSum(const QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix);

QVector<QVector<int>> getStrongConnectedMatrix(const QVector<QVector<int>>& reachabilityMatrix);

QVector<QVector<int>> getStrongComponentsConnectivity(const QVector<QVector<int>>& adjacencyMatrix);

QVector<QVector<int>> getWeakComponentsConnectivity(const QVector<QVector<int>>& pathsFromAllPoint, const int& adjacencyMatrixSize);

void mergeVector(QVector<int>& firstVector, const QVector<int>& secondVector);

QString vectorComponentsToString(const QVector<QVector<int>>& vectorComponents);

int messageBoxYesNoQuestion(QWidget* parent, const QString& header, const QString& text);

void saveAdjacencyMatrix(QSettings& settings, const QVector<QVector<int>>& adjacencyMatrix);

void saveNodeCoordinates(QSettings& settings, const QVector<Node*>& nodes);

QVector<QVector<int>> loadAdjacencyMatrix(QSettings& settings);

void loadNodeCoordinates(QSettings& settings, const QVector<Node*>& nodes);

QVector<QVector<int>> getSpanningTreeByPrima(const QVector<QVector<int>>& adjacencyMatrix);

QPair<int, int> getMinConnectionFromHandledNodes(const QVector<QVector<int>>& adjacencyMatrix, const QVector<bool>& handledNode);

QVector<QVector<int>> getSpanningTreeByKruskal(const QVector<QVector<int>>& adjacencyMatrix);

QVector<QPair<int, int>> getAllOrderedConnection(const QVector<QVector<int>>& adjacencyMatrix);

template <typename T>
int findSetContainingElement(const QVector<T>& vectorOfSets, const typename T::value_type& element);

#endif // UTILS_H
