#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QPainter>

#include <QTime>

#include <QDebug>

#include "Node.h"
#include "graph.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

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

private:
    Ui::MainWindow *ui;

    Graph *graph;
    QGraphicsView *scene_view;
    QGraphicsScene *scene;

};
#endif // MAINWINDOW_H
