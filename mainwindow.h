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

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

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

    // отрисовка изменений в матрице
    void onMatrixCellChanged(const QModelIndex &index);

    void updateScene();

    void on_delete_node_button_clicked();

    void on_delete_link_button_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_clicked(bool checked);

    void on_deixtra_button_clicked();

    void updateStatusBar(QString text);

    void on_randomize_button_clicked();

private:
    Ui::MainWindow *ui;

    Graph *graph;

    // Сцена для отрисовки
    QGraphicsScene *scene;


    // Модель для отображения матрицы смежности
    QStandardItemModel *matrixModel;

    QTimer *updateTimer;
    int updateInterval = 1000; // интервал обновления в миллисекундах

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
