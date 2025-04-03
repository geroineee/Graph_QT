#include "clique.h"
#include <QSet>
#include <algorithm>
#include <QDebug>

// Функция для удаления поглощаемых items
QVector<QSet<int>> removeAbsorbedItems(const QVector<QSet<int>>& items) {
    QVector<QSet<int>> result;
    QVector<QSet<int>> sortedItems = items;

    // Сортируем по размеру (от меньшего к большему)
    std::sort(sortedItems.begin(), sortedItems.end(),
        [](const QSet<int>& a, const QSet<int>& b) {
            return a.size() < b.size();
        });

    for (int i = 0; i < sortedItems.size(); ++i) {
        bool isAbsorbed = false;
        // Проверяем только большие множества
        for (int j = i + 1; j < sortedItems.size(); ++j) {
            if (sortedItems[j].contains(sortedItems[i])) {
                isAbsorbed = true;
                break;
            }
        }
        if (!isAbsorbed) {
            result.append(sortedItems[i]);
        }
    }
    return result;
}

// Функция для раскрытия скобок (КНФ → ДНФ)
QVector<QSet<int>> expandDisjunctions(const QVector<QPair<int, int>>& disjunctions) {
    QVector<QSet<int>> result;
    if (disjunctions.empty()) return result;

    // Используем QSet для хранения уникальных элементов
    QSet<QSet<int>> uniqueItems;

    // Начальные элементы
    uniqueItems.insert({disjunctions[0].first});
    uniqueItems.insert({disjunctions[0].second});

    for (int i = 1; i < disjunctions.size(); ++i) {
        QSet<QSet<int>> newUniqueItems;
        const int a = disjunctions[i].first;
        const int b = disjunctions[i].second;

        for (const QSet<int>& item : qAsConst(uniqueItems)) {
            QSet<int> newItem1 = item;
            newItem1.insert(a);
            newUniqueItems.insert(newItem1);

            QSet<int> newItem2 = item;
            newItem2.insert(b);
            newUniqueItems.insert(newItem2);
        }

        uniqueItems = newUniqueItems;
    }

        result.reserve(uniqueItems.size());
        foreach (const QSet<int>& item, uniqueItems)
        {
            result.append(item);
        }

        return result;
}

// Находим дополнение item — вершины, которые НЕ входят в него
QSet<int> findCliqueFromItem(const QSet<int>& item, int totalVertices) {
    QSet<int> clique;
    for (int i = 0; i < totalVertices; ++i) {
        if (!item.contains(i)) {
            clique.insert(i);
        }
    }
    return clique;
}

QVector<QVector<int>> findAllMaxCliques(const QVector<QVector<int>>& adjacencyMatrix) {
    const int n = adjacencyMatrix.size();
    QVector<QVector<int>> maxCliques;
    if (n == 0) return maxCliques;

    // 1. Находим все дизъюнкции для отсутствующих рёбер
    QVector<QPair<int, int>> disjunctions;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (adjacencyMatrix[i][j] == 0) {
                disjunctions.append({i, j});
            }
        }
    }

    // 2. Раскрываем скобки (КНФ → ДНФ)
    QVector<QSet<int>> dnfItems = expandDisjunctions(disjunctions);

    // 3. Находим все клики как дополнения items
    QVector<QSet<int>> allCliques;
    for (const QSet<int>& item : qAsConst(dnfItems)) {
        allCliques.append(findCliqueFromItem(item, n));
    }

    // 4. Находим максимальный размер клики
    int maxSize = 0;
    for (const QSet<int>& clique : allCliques) {
        if (clique.size() > maxSize) {
            maxSize = clique.size();
        }
    }

    // 5. Собираем все клики максимального размера
    for (const QSet<int>& clique : allCliques) {
        if (clique.size() == maxSize) {

            QVector<int> cliqueVector;

            cliqueVector.reserve(clique.size());
            for (int vertex : clique)
            {
                cliqueVector.append(vertex);
            }

            std::sort(cliqueVector.begin(), cliqueVector.end());
            maxCliques.append(cliqueVector);
        }
    }

    return maxCliques;
}
