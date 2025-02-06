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

    ui->label_matrixName->setAlignment(Qt::AlignCenter);
    ui->label_matrixName->setText("Матрица смежности");

    // коннект кнопки "очистить"
    connect(ui->clear_button, &QPushButton::clicked, graph, &Graph::clearScene);

    // коннект обновления статусбара из Graph
    connect(graph, &Graph::textToStatusBar, this, &MainWindow::updateStatusBar);

    //  ----------------------------------------------Матрица смежности----------------------------------------------------------

        // модель для матрицы смежности
        matrixModel = new QStandardItemModel(this);
        reachMatrixModel = new QStandardItemModel(this);
        strongMatrixModel = new QStandardItemModel(this);

        // установка модели к обьекту QTableView
        ui->matrixView->setModel(matrixModel);
        ui->tableView_reachMatrix->setModel(reachMatrixModel);
        ui->tableView_strongMatrix->setModel(strongMatrixModel);

        ui->tableView_reachMatrix->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView_strongMatrix->setEditTriggers(QAbstractItemView::NoEditTriggers);

        // минимальный размер колонок и столбцов QTableView
        ui->matrixView->verticalHeader()->setMinimumSectionSize(5);
        ui->matrixView->horizontalHeader()->setMinimumSectionSize(5);

        ui->tableView_reachMatrix->verticalHeader()->setMinimumSectionSize(5);
        ui->tableView_reachMatrix->horizontalHeader()->setMinimumSectionSize(5);

        ui->tableView_strongMatrix->verticalHeader()->setMinimumSectionSize(5);
        ui->tableView_strongMatrix->horizontalHeader()->setMinimumSectionSize(5);

        // коннект для изменения графа при изменении матрицы
        connect(matrixModel, &QStandardItemModel::dataChanged, this, &MainWindow::onMatrixCellChanged);

        // коннект матрицы смежности
        connect(graph, &Graph::adjacencyMatrixChanged, this, &MainWindow::updateAdjacencyMatrix);

        // Отправляем сигнал об изменении матрицы смежности
        updateAdjacencyMatrix(graph->getMatrix());

    // --------------------------------------------------------------------------------------------------------------------------

    // Стиль кнопок
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
    // Выключение других функций
    graph->turn_off_buttons();

    graph->addNode();

    // отрисовка узлов
    updateScene();

}


void MainWindow::on_pushButton_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

    graph->needToLink = true;
}


void MainWindow::on_clear_button_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();
    graph->clearScene();
}

void updateMatrixView(QTableView* tableView, QStandardItemModel* model, const QVector<QVector<int>>& matrix)
{
    // Центрирование полей
    CenterTextDelegate *delegate = new CenterTextDelegate;
    tableView->setItemDelegate(delegate);

    model->clear();
    model->setColumnCount(matrix.size());
    model->setRowCount(matrix.size());

    for (int i = 0; i < matrix.size(); ++i)
    {
        for (int j = 0; j < matrix[i].size(); ++j)
        {
            QStandardItem *item = new QStandardItem(QString::number(matrix[i][j]));
            model->setItem(i, j, item);
        }
    }

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Растягивание все колонки
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Растягивание все строки
}

void MainWindow::updateAdjacencyMatrix(const QVector<QVector<int>>& adjacencyMatrix)
{
    updateMatrixView(ui->matrixView, matrixModel, adjacencyMatrix);
    if (!adjacencyMatrix.isEmpty()){
        updateReachabilityMatrix(adjacencyMatrix);
    } else {
        reachMatrixModel->clear();
        strongMatrixModel->clear();
    }
}

void MainWindow::updateReachabilityMatrix(const QVector<QVector<int>>& adjacencyMatrix)
{
    QVector<QVector<int>> reachabilityMatrix = getReachabilityMatrix(adjacencyMatrix);
    updateMatrixView(ui->tableView_reachMatrix, reachMatrixModel, reachabilityMatrix);
    updateStrongConnectedMatrix(reachabilityMatrix);
}

void MainWindow::updateStrongConnectedMatrix(const QVector<QVector<int>>& reachabilityMatrix)
{
    QVector<QVector<int>> strongConnectedMatrix = getStrongConnectedMatrix(reachabilityMatrix);
    updateMatrixView(ui->tableView_strongMatrix, strongMatrixModel, strongConnectedMatrix);
}

void MainWindow::onMatrixCellChanged(const QModelIndex &index)
{
    int row = index.row();
    int column = index.column();
    int value = matrixModel->data(index).toInt();

    // Обновляем матрицу смежности
    graph->getMatrix()[row][column] = value;

    if (!graph->getMatrix().isEmpty()){
        updateReachabilityMatrix(graph->getMatrix());
    } else {
        reachMatrixModel->clear();
        strongMatrixModel->clear();
    }

    // Перерисовываем граф
    updateScene();
}

void MainWindow::updateScene()
{
    graph->drawLinks();
//    graph->drawNodes();
}

void MainWindow::on_delete_node_button_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

    // переключение
    bool &value = graph->needToDelete;
    value = !value;
}


void MainWindow::on_delete_link_button_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

     // переключение
     bool &value = graph->needToDeleteLink;
     value = !value;

}

void MainWindow::on_pushButton_2_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

    // переключение
    bool &value = graph->needToSolveTask;
    value = !value;

}

void MainWindow::on_checkBox_clicked(bool checked)
{
    graph->needTwoWayAddition = checked;
}


void MainWindow::on_deixtra_button_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

    // переключение
    bool &value = graph->needDeixtra;
    value = !value;
}

void MainWindow::updateStatusBar(QString text)
{
    ui->statusbar->showMessage(text);
}

void MainWindow::on_randomize_button_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

    graph->randomizeAdjacencyMatrix(7);
}


void MainWindow::on_pushButton_3_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

    // переключение
    bool &value = graph->needInDeep;
    value = !value;
}


void MainWindow::on_butti_in_width_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

    // переключение
    bool &value = graph->needInWidth;
    value = !value;
}


void MainWindow::on_button_floid_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

    // переключение
    bool &value = graph->needFloid;
    value = !value;
}


void MainWindow::on_pushButton_readGrafFromFile_clicked()
{
    QFile* file = tryToOpenFile(QDir::currentPath() + "/matrix.txt");
    QVector<QVector<int>> matrix = readMatrixFromFile(file, fileReadingLines);

    if (matrix.isEmpty())
    {
        if (messageBoxYesNoQuestion(this, "?", "Достигнут конец файла,\nначать чтение заново?") == QMessageBox::Yes){
            fileReadingLines = 0;
            on_pushButton_readGrafFromFile_clicked();
        } else {
            return;
        }
    }
    else
    {
        graph->clearScene();
        graph->addNodeFromNewMatrix(matrix);
        updateScene();
    }
}


void MainWindow::on_pushButton_switchMatrix_clicked()
{
    QVector<QString> textToPrint = {"Матрица смежности", "Матрица достижимости", "Матрица сильной связности"};

    int currentIndex = ui->stackedWidget_matrix->currentIndex();
    int index = 0;
    if (currentIndex != ui->stackedWidget_matrix->count()-1) index = currentIndex+1;
    ui->stackedWidget_matrix->setCurrentIndex(index);

    ui->label_matrixName->setText(textToPrint[index]);
    if (index == ui->stackedWidget_matrix->count()-1) index = -1;
    ui->pushButton_switchMatrix->setText(textToPrint[index+1]);
}


void MainWindow::on_pushButton_connectivityComponents_clicked()
{
    if (graph->getMatrix().isEmpty()) return;
    QVector<QVector<int>> strongComponents = getStrongComponentsConnectivity(getStrongConnectedMatrix(getReachabilityMatrix(graph->getMatrix())));
    QVector<QVector<int>> weakComponents = getWeakComponentsConnectivity(getAllPointPathsInDepth(graph->getMatrix().size()), graph->getMatrix().size());
    QString text = "Компоненты сильной связности:\n";
    text += vectorComponentsToString(strongComponents);
    text += "\n\nКомпоненты слабой связности:\n";
    text += vectorComponentsToString(weakComponents);

    QMessageBox::information(this, "Компоненты связности", text);
}

QVector<QVector<int>> MainWindow::getAllPointPathsInDepth(const int& adjacencyMatrixSize)
{
    QVector<QVector<int>> paths;
    for (int i = 0; i < adjacencyMatrixSize; i++)
    {
        QVector<int> path;
        QVector<bool> visited(adjacencyMatrixSize, false);
        graph->depthFirstSearch(i, visited, path);
        paths.push_back(path);
    }

    return paths;
}
