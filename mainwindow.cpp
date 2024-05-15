#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // создание графа
    graph = new Graph(scene);
    // создание сцены
    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_draw_button_clicked()
{
    graph->addNode();
    qDebug() << "Нажал.";

    graph->drawNodes(scene);
    graph->drawLinks(scene);
}


void MainWindow::on_pushButton_clicked()
{
    graph->needToLink = true;
}

