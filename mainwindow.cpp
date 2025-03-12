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

        // коннект для изменения графа при изменении матрицы
        connect(matrixModel, &QStandardItemModel::dataChanged, this, &MainWindow::onMatrixCellChanged);

        // коннект матрицы смежности
        connect(graph, &Graph::adjacencyMatrixChanged, this, &MainWindow::updateAdjacencyMatrix);

        ui->comboBox_switchMatrix->addItems(matrixItemsTitles);

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
}

void MainWindow::on_draw_button_clicked()
{
    // Выключение других функций
    graph->turn_off_buttons();

    graph->addNode();

    // отрисовка узлов
    updateScene(graph->getMatrix());
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

void MainWindow::updateMatrixView(QTableView* tableView, QStandardItemModel* model, const QVector<QVector<int>>& matrix)
{
    // Центрирование полей
    CenterTextDelegate *delegate = new CenterTextDelegate;
    tableView->setItemDelegate(delegate);

    model->blockSignals(true); // Блокировка сигнала обновления данных в матрице

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

    model->blockSignals(false);

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Растягивание всех колонок
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Растягивание всех строк
}

void MainWindow::updateAdjacencyMatrix(const QVector<QVector<int>>& adjacencyMatrix)
{
    updateMatrixView(ui->matrixView, matrixModel, adjacencyMatrix);
    graph->setDrawinMatrix(adjacencyMatrix);
    updateScene(adjacencyMatrix);
}

void MainWindow::updateShortestPathsMatrix(const QVector<QVector<int>>& shortestPathsMatrix)
{
    updateMatrixView(ui->matrixView, matrixModel, shortestPathsMatrix);
}

void MainWindow::updateReachabilityMatrix(const QVector<QVector<int>>& adjacencyMatrix)
{
    QVector<QVector<int>> reachabilityMatrix = getReachabilityMatrix(adjacencyMatrix);
    updateMatrixView(ui->matrixView, matrixModel, reachabilityMatrix);
}

void MainWindow::updateStrongConnectedMatrix(const QVector<QVector<int>>& reachabilityMatrix)
{
    if (reachabilityMatrix.empty()) return;

    QVector<QVector<int>> strongConnectedMatrix = getStrongConnectedMatrix(reachabilityMatrix);
    updateMatrixView(ui->matrixView, matrixModel, strongConnectedMatrix);
}

void MainWindow::onMatrixCellChanged(const QModelIndex &index)
{
    int row = index.row();
    int column = index.column();
    int value = matrixModel->data(index).toInt();

    // Обновляем матрицу смежности
    graph->getMatrix()[row][column] = value;
    graph->setDrawinMatrix(graph->getMatrix());

    // Перерисовываем граф
    updateScene(graph->getMatrix());
}

void MainWindow::updateScene(const QVector<QVector<int>>& matrix)
{
    graph->drawLinks(matrix);
}

void MainWindow::on_delete_node_button_clicked()
{
    switchModes(graph->needToDelete);
}


void MainWindow::on_delete_link_button_clicked()
{
    switchModes(graph->needToDeleteLink);

}

void MainWindow::on_pushButton_2_clicked()
{
    switchModes(graph->needToSolveTask);

}

void MainWindow::on_checkBox_clicked(bool checked)
{
    graph->needTwoWayAddition = checked;
}


void MainWindow::on_deixtra_button_clicked()
{
    switchModes(graph->needDeixtra);
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
    switchModes(graph->needInDeep);
}


void MainWindow::on_butti_in_width_clicked()
{
    switchModes(graph->needInWidth);
}


void MainWindow::on_button_floid_clicked()
{
    switchModes(graph->needFloid);
}

void MainWindow::switchModes(bool& mode)
{
    // Выключение других функций
    graph->turn_off_buttons();

    // переключение
    mode = !mode;
}

void MainWindow::disableAllButtons(bool flag)
{
    ui->action_openMatrix->setDisabled(flag);
    QList<QPushButton*> pushButtonChildren = findChildren<QPushButton*>();
    Q_FOREACH (QPushButton* button, pushButtonChildren)
    {
        button->setDisabled(flag);
    }
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
        updateScene(matrix);
    }
}


void MainWindow::on_pushButton_connectivityComponents_clicked()
{
    QVector<QVector<int>> adjacencyMatrix = graph->getMatrix();
    QVector<QVector<int>> pathsFormAllPoints = getAllPointPathsInDepth(adjacencyMatrix.size());

    if (adjacencyMatrix.isEmpty())
        return;

    QVector<QVector<int>> strongComponents = getStrongComponentsConnectivity(adjacencyMatrix);
    QVector<QVector<int>> weakComponents = getWeakComponentsConnectivity(pathsFormAllPoints, adjacencyMatrix.size());

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


void MainWindow::on_action_saveMatrix_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Сохранить текущий граф."), "", tr("Ini Files (*.ini);"));

    if (!filePath.isEmpty())
    {
        QSettings matrixFileSettings(filePath, QSettings::IniFormat);
        saveAdjacencyMatrix(matrixFileSettings, graph->getMatrix());
        saveNodeCoordinates(matrixFileSettings, graph->getNodes());
    }
}


void MainWindow::on_action_openMatrix_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Открыть сохраненный граф."), "", tr("Ini Files (*.ini);"));

    if (!filePath.isEmpty())
    {
        graph->clearScene();
        QSettings matrixFileSettings(filePath, QSettings::IniFormat);
        QVector<QVector<int>> adjacencyMatrix = loadAdjacencyMatrix(matrixFileSettings);
        graph->addNodeFromNewMatrix(adjacencyMatrix);
        loadNodeCoordinates(matrixFileSettings, graph->getNodes());
    }
}

void MainWindow::on_comboBox_switchMatrix_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        updateMatrixViewCommon(QAbstractItemView::AllEditTriggers, graph->getMatrix(), &MainWindow::updateAdjacencyMatrix, false);
        break;
    case 1:
        updateMatrixViewCommon(QAbstractItemView::NoEditTriggers, getShortestPathsMatrixBuShimbell(graph->getMatrix()), &MainWindow::updateShortestPathsMatrix, false);
        break;
    case 2:
        updateMatrixViewCommon(QAbstractItemView::NoEditTriggers, graph->getMatrix(), &MainWindow::updateReachabilityMatrix, true);
        break;
    case 3:
        updateMatrixViewCommon(QAbstractItemView::NoEditTriggers, getReachabilityMatrix(graph->getMatrix()), &MainWindow::updateStrongConnectedMatrix, true);
        break;
    case 4:
        updateMatrixViewCommon(QAbstractItemView::NoEditTriggers, getSpanningTreeByPrima(graph->getMatrix()), &MainWindow::updateAdjacencyMatrix, true);
        break;
    case 5:
        updateMatrixViewCommon(QAbstractItemView::NoEditTriggers, getSpanningTreeByKruskal(graph->getMatrix()), &MainWindow::updateAdjacencyMatrix, true);
        break;
    default:
        break;
    }
}

void MainWindow::updateMatrixViewCommon(QAbstractItemView::EditTriggers editTrigger, const QVector<QVector<int>>& matrix,
                                        void (MainWindow::*updateFunction)(const QVector<QVector<int>>&), bool needToDisableButtons)
{
    ui->matrixView->setEditTriggers(editTrigger);
    (this->*updateFunction)(matrix);
    disableAllButtons(needToDisableButtons);
}

