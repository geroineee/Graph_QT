#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QItemDelegate>
#include <QPainter>
#include <QTime>
#include <QDebug>

#include "Node.h"
#include "graph.h"
#include "zoomablegraphicsview.h"
#include "utils.h"
#include "algorithms/kruaskalmethod.h"
#include "algorithms/primmethod.h"
#include "algorithms/shimbellmethod.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QDir>
#include <QFileDialog>
#include <QSettings>

#include <QStandardItemModel>
#include <QTableView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_draw_button_clicked();

    void on_pushButton_clicked();

    void on_clear_button_clicked();

    void updateAdjacencyMatrix(const QVector<QVector<int>>& adjacencyMatrix);

    // отрисовка изменений в матрице
    void onMatrixCellChanged(const QModelIndex &index);

    void on_delete_node_button_clicked();

    void on_delete_link_button_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_clicked(bool checked);

    void on_deixtra_button_clicked();

    void updateStatusBar(QString text);

    void on_randomize_button_clicked();

    void updateScene(const QVector<QVector<int>>& matrix);


    void on_pushButton_3_clicked();

    void on_butti_in_width_clicked();

    void on_button_floid_clicked();

    void on_pushButton_readGrafFromFile_clicked();

    void on_pushButton_connectivityComponents_clicked();

    void on_action_saveMatrix_triggered();

    void on_action_openMatrix_triggered();

    void on_comboBox_switchMatrix_currentIndexChanged(int index);

    void on_comboBox_countConnectionForShimbell_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    Graph *graph;

    // Сцена для отрисовки
    QGraphicsScene *scene;


    // Модель для отображения матрицы смежности
    QStandardItemModel *matrixModel;

    int fileReadingLines = 0;

    QStringList matrixItemsTitles = {"Матрица смежности", "Матрица минимальных путей (Алгоритм Шимбелла)" , "Матрица максимальных путей (Алгоритм Шимбелла)",
                                     "Матрица достижимости", "Матрица сильной связности", "Остовное дерево (Метод Прима)",
                                     "Остовное дерево (Метод Краскала)"};

    QVector<QVector<int>> getAllPointPathsInDepth(const int& adjacencyMatrixSize); // Обход в глубину для всех вершин графа
    void switchModes(bool& mode);
    void disableAllButtons(bool);
    void updateMatrixViewCommon(const QVector<QVector<int>>& matrix, void (MainWindow::*updateFunction)(const QVector<QVector<int>>&),
                                QAbstractItemView::EditTriggers editTrigger, bool needToDisableButtons);

    // отрисовка матрицы
    void updateMatrixView(QTableView* tableView, QStandardItemModel* model, const QVector<QVector<int>>& matrix);
    void updateReachabilityMatrix(const QVector<QVector<int>>& adjacencyMatrix);
    void updateStrongConnectedMatrix(const QVector<QVector<int>>& reachabilityMatrix);
    void updateShortestPathsMatrix(const QVector<QVector<int>>& shortestPathsMatrix);
};

class CenterTextDelegate : public QItemDelegate
{
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem opt = option;

        opt.displayAlignment = Qt::AlignCenter;

        QItemDelegate::paint(painter, opt, index);
    }
};
#endif // MAINWINDOW_H
