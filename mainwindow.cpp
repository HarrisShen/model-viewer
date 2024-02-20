#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chart(new QChart)
    , series(new QLineSeries)
{
    ui->setupUi(this);

    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::selectModel);

    QObject::connect(ui->modelList, &QListWidget::itemDoubleClicked, this, &MainWindow::onModelListItemDoubleClicked);

    series->setUseOpenGL(true);
    chart->addSeries(series);

    auto axisX = new QValueAxis;
    axisX->setRange(0, 100);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Episode");

    auto axisY = new QValueAxis;
    axisY->setRange(-100, 100);
    axisY->setTitleText("Return");

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->hide();

    chart->setTitle("Total return of each episodes");

    ui->chartView->setChart(chart);
}

MainWindow::~MainWindow()
{
    delete series;
    delete chart;
    delete ui;
}

void MainWindow::selectModel()
{
    dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "",
                                                QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dirName.isEmpty()) return;
    dataFrame = readCsv<double, int, QString>(QDir(dirName).filePath("train_output.csv"));
    QListWidget *modelList = ui->modelList;
    modelList->addItem(dirName);
    modelList->setCurrentRow(modelList->count() - 1);
    _renderData();
}

void MainWindow::_renderData(){
    series->clear();
    int windowSize = 50, rollingSum = 0;
    for (qsizetype i = 0; i < dataFrame.size(); i++) {
        if (i >= windowSize) rollingSum -= dataFrame.at(0, i - windowSize);
        rollingSum += dataFrame.at(0, i);
        if (i < windowSize) continue;
        series->append(i, rollingSum / windowSize);
    }

    auto axisX = chart->axes().at(0);
    axisX->setRange(0, dataFrame.size());

    auto axisY = chart->axes().at(1);
    axisY->setRange(dataFrame.column(0).min() - 1,
                    dataFrame.column(0).max() + 1);
}

void MainWindow::onModelListItemDoubleClicked(QListWidgetItem *item)
{
    dirName = item->text();
    dataFrame = readCsv<double, int, QString>(QDir(dirName).filePath("train_output.csv"));
    _renderData();
}

