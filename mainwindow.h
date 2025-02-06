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

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QDir>

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

    // отрисовка матрицы
    void updateAdjacencyMatrix(const QVector<QVector<int>>& adjacencyMatrix);
    void updateReachabilityMatrix(const QVector<QVector<int>>& adjacencyMatrix);
    void updateStrongConnectedMatrix(const QVector<QVector<int>>& reachabilityMatrix);

    // отрисовка изменений в матрице
    void onMatrixCellChanged(const QModelIndex &index);

    void on_delete_node_button_clicked();

    void on_delete_link_button_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_clicked(bool checked);

    void on_deixtra_button_clicked();

    void updateStatusBar(QString text);

    void on_randomize_button_clicked();

    void updateScene();


    void on_pushButton_3_clicked();

    void on_butti_in_width_clicked();

    void on_button_floid_clicked();

    void on_pushButton_readGrafFromFile_clicked();

    void on_pushButton_switchMatrix_clicked();

    void on_pushButton_connectivityComponents_clicked();

private:
    Ui::MainWindow *ui;

    Graph *graph;

    // Сцена для отрисовки
    QGraphicsScene *scene;


    // Модель для отображения матрицы смежности
    QStandardItemModel *matrixModel;
    QStandardItemModel *reachMatrixModel;
    QStandardItemModel *strongMatrixModel;

    QTimer *updateTimer;
    int updateInterval = 1000; // интервал обновления в миллисекундах

    int fileReadingLines = 0;

    QVector<QVector<int>> getAllPointPathsInDepth(const int& adjacencyMatrixSize); // Обход в глубину для всех вершин графа
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
