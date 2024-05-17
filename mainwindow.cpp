#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // создание сцены
    scene = new QGraphicsScene;

    // создание графа
    graph = new Graph(scene);
    ui->graphicsView->setScene(scene);

    // коннект кнопки "очистить"
    connect(ui->clear_button, &QPushButton::clicked, graph, &Graph::clearScene);


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
    updateTimer = new QTimer(this);

    // коннект таймера к слоту для обновления сцены
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateScene);

    // запуск таймера
    updateTimer->start(updateInterval);
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
    graph->addNode();
    qDebug() << "Добавление узла.";

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
    qDebug() << "Удаление узла.";
    graph->needToDelete = true;
}

