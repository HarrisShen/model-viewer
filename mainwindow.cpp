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

    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::selectFile);

    auto chartView = new QChartView(chart);
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

    auto mainLayout = ui->mainLayout;
    mainLayout->addWidget(chartView);
}

MainWindow::~MainWindow()
{
    delete series;
    delete chart;
    delete ui;
}

void MainWindow::selectFile()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("CSV Files (*.csv)"));
    dataFrame = readCsv<double, int, QString>(fileName);

    series->clear();
    for (qsizetype i = 0; i < dataFrame.size(); i++) {
        series->append(dataFrame.indexAt(i), dataFrame.at(0, i));
    }

    auto axisX = chart->axes().at(0);
    axisX->setRange(0, dataFrame.size());


    auto axisY = chart->axes().at(1);
    axisY->setRange(dataFrame.column(0).min() - 1,
                    dataFrame.column(0).max() + 1);
}
