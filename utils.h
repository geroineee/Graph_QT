#ifndef UTILS_H
#define UTILS_H

#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSet>

QFile* tryToOpenFile(const QString& file_path, bool isTranc = false);

QVector<int> vectorStringToInt(const QVector<QString>& vect);

QVector<QVector<int>> readMatrixFromFile(QFile* file, int& nextLineNum);

QVector<QVector<int>> getMatrixProduct(const QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix);

QVector<QVector<int>> getReachabilityMatrix(const QVector<QVector<int>>& adjacencyMatrix);

QVector<QVector<int>> getUnitMatrix(const QVector<QVector<int>>& matrix);

QVector<QVector<int>> getMatrixSum(const QVector<QVector<int>>& firstMatrix, const QVector<QVector<int>>& secondMatrix);

QVector<QVector<int>> getStrongConnectedMatrix(const QVector<QVector<int>>& reachabilityMatrix);

QVector<QVector<int>> getStrongComponentsConnectivity(const QVector<QVector<int>>& strongConnectedMatrix);

QVector<QVector<int>> getWeakComponentsConnectivity(const QVector<QVector<int>>& pathsFromAllPoint, const int& adjacencyMatrixSize);

void mergeVector(QVector<int>& firstVector, const QVector<int>& secondVector);

QString vectorComponentsToString(const QVector<QVector<int>>& vectorComponents);

int messageBoxYesNoQuestion(QWidget* parent, const QString& header, const QString& text);

#endif // UTILS_H
