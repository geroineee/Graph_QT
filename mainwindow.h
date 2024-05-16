#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QItemDelegate>


#include <QPainter>

#include <QTime>

#include <QDebug>

#include "Node.h"
#include "graph.h"

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

private:
    Ui::MainWindow *ui;

    Graph *graph;

    // Сцена для отрисовки
    QGraphicsScene *scene;

    QStandardItemModel *matrixModel; // Модель для отображения матрицы смежности

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
