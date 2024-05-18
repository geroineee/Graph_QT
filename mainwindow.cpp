#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // создание сцены
    QGraphicsScene *scene = new QGraphicsScene(this);

    // создание графа
    graph = new Graph(scene);
    ui->graphicsView->setScene(scene);

    // коннект кнопки "очистить"
    connect(ui->clear_button, &QPushButton::clicked, graph, &Graph::clearScene);

    // коннект обновления статусбара из Graph
    connect(graph, &Graph::textToStatusBar, this, &MainWindow::updateStatusBar);


    //  ----------------------------------------------Матрица смежности----------------------------------------------------------

        // модель для матрицы смежности
        matrixModel = new QStandardItemModel(this);

        // установка модели к обьекту QTableView
        ui->matrixView->setModel(matrixModel);

        // минимальный размер колонок и столбцов QTableView
        ui->matrixView->verticalHeader()->setMinimumSectionSize(5);
        ui->matrixView->horizontalHeader()->setMinimumSectionSize(5);

        // коннеут для изменения графа при изменении матрицы
        connect(matrixModel, &QStandardItemModel::dataChanged, this, &MainWindow::onMatrixCellChanged);

        // коннект матрицы смежности
        connect(graph, &Graph::adjacencyMatrixChanged, this, &MainWindow::updateAdjacencyMatrix);

        // Отправляем сигнал об изменении матрицы смежности
        updateAdjacencyMatrix(graph->getMatrix());

    // --------------------------------------------------------------------------------------------------------------------------

    // инициализация таймера
//    updateTimer = new QTimer(this);

    // коннект таймера к слоту для обновления сцены
//    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateScene);

    // запуск таймера
//    updateTimer->start(updateInterval);

    // кнопки

    ui->delete_link_button->setStyleSheet("QPushButton::pressed { background-color: lightgray; border: 1px solid black; }");
    ui->delete_node_button->setStyleSheet("QPushButton::pressed { background-color: lightgray; border: 1px solid black; }");
    ui->draw_button->setStyleSheet("QPushButton::pressed { background-color: lightgray; border: 1px solid black; }");
    ui->pushButton->setStyleSheet("QPushButton::pressed { background-color: lightgray; border: 1px solid black; }");
    ui->pushButton_2->setStyleSheet("QPushButton::pressed { background-color: lightgray; border: 1px solid black; }");
    ui->clear_button->setStyleSheet("QPushButton::pressed { background-color: lightgray; border: 1px solid black; }");
}

MainWindow::~MainWindow()
{
    delete ui;

    // очистка таймера
    updateTimer->stop();
    delete updateTimer;
}

void MainWindow::on_draw_button_clicked()
{
    qDebug() << "Добавление узла.";
    graph->addNode();

    // отрисовка узлов
    graph->drawNodes();
}

void MainWindow::on_pushButton_clicked()
{
    graph->needToLink = true;
}


void MainWindow::on_clear_button_clicked()
{
    qDebug() << "Очистка.";
}

void MainWindow::updateAdjacencyMatrix(const QVector<QVector<int>>& adjacencyMatrix)
{
    // Центрирование полей
    CenterTextDelegate *delegate = new CenterTextDelegate;
    ui->matrixView->setItemDelegate(delegate);

    // Очистка
    matrixModel->clear();
    matrixModel->setColumnCount(adjacencyMatrix.size());
    matrixModel->setRowCount(adjacencyMatrix.size());

    for (int i = 0; i < adjacencyMatrix.size(); ++i)
    {
        for (int j = 0; j < adjacencyMatrix[i].size(); ++j)
        {
            QStandardItem *item = new QStandardItem(QString::number(adjacencyMatrix[i][j]));
            matrixModel->setItem(i, j, item);
        }
    }

    // Установка ширины колонок и высоты строк на всю ширину и высоту окна QTableView
    ui->matrixView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Растягиваем все колонки
    ui->matrixView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Растягиваем все строки
}

void MainWindow::onMatrixCellChanged(const QModelIndex &index)
{
    int row = index.row();
    int column = index.column();
    int value = matrixModel->data(index).toInt();

    // Обновляем матрицу смежности
    graph->getMatrix()[row][column] = value;

    // Перерисовываем граф
    graph->drawLinks();
}

void MainWindow::updateScene()
{
    graph->drawNodes();
    graph->drawLinks();
}

void MainWindow::on_delete_node_button_clicked()
{
    // переключение
    bool &value = graph->needToDelete;
    if (value)
    {
        value = false;
        qDebug() << "Удаление узла. off";
    }

    else
    {
        value = true;
        qDebug() << "Удаление узла. on";
    }

    // Выключение других функций
    graph->needToDeleteLink = false;
    graph->needToLink = false;
    graph->needToSolveTask = false;
    graph->needDeixtra = false;
}


void MainWindow::on_delete_link_button_clicked()
{
     // переключение
     bool &value = graph->needToDeleteLink;
     if (value)
     {
         value = false;
         qDebug() << "Удаление связи. off";
     }
     else
     {
         value = true;
         qDebug() << "Удаление связи. on";
     }

     // Выключение других функций
     graph->needToLink = false;
     graph->needToSolveTask = false;
     graph->needToDelete = false;
     graph->needDeixtra = false;
}

void MainWindow::on_pushButton_2_clicked()
{
    // переключение
    bool &value = graph->needToSolveTask;
    if (value)
    {
        value = false;
        qDebug() << "Коммивояжер. off";
    }
    else
    {
        value = true;
        qDebug() << "Коммивояжер. on";
    }

    // Выключение других функций
    graph->needToDeleteLink = false;
    graph->needToLink = false;
    graph->needToDelete = false;
    graph->needDeixtra = false;
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    graph->needTwoWayAddition = checked;
}


void MainWindow::on_deixtra_button_clicked()
{
    // переключение
    bool &value = graph->needDeixtra;
    if (value)
    {
        value = false;
        qDebug() << "Дейкстра. off";
    }
    else
    {
        value = true;
        qDebug() << "Дейкстра. on";
    }

    // Выключение других функций
    graph->needToDeleteLink = false;
    graph->needToLink = false;
    graph->needToSolveTask = false;
    graph->needToDelete = false;
}

void MainWindow::updateStatusBar(QString text)
{
    ui->statusbar->showMessage(text);
}

void MainWindow::on_randomize_button_clicked()
{
    qDebug() << "Рандомизация.";
    graph->randomizeAdjacencyMatrix(10);
}

