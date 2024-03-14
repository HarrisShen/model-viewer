#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QLabel>
#include <QJsonDocument>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dataloader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , dataUpdateThread_(new QThread())
    , chart_(new QChart())
    , series_(new QLineSeries())
{
    ui_->setupUi(this);

    QObject::connect(ui_->actionOpen, &QAction::triggered, this, &MainWindow::selectModel);

    QObject::connect(ui_->modelList, &QListWidget::itemDoubleClicked, this, &MainWindow::onModelListItemDoubleClicked);

    fsWatcher_ = new QFileSystemWatcher(this);
    QObject::connect(fsWatcher_, &QFileSystemWatcher::fileChanged, this, &MainWindow::onDataFileChanged);

    DataLoader *dataLoader = new DataLoader();
    dataLoader->moveToThread(dataUpdateThread_);
    QObject::connect(dataUpdateThread_, &QThread::finished, dataLoader, &QObject::deleteLater);
    QObject::connect(this, &MainWindow::fetchUpdate, dataLoader, &DataLoader::loadData);
    QObject::connect(dataLoader, &DataLoader::resultReady, this, &MainWindow::updateData);
    dataUpdateThread_->start();

    series_->setUseOpenGL(true);
    chart_->addSeries(series_);

    auto axisX = new QValueAxis;
    axisX->setRange(0, 100);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Episode");

    auto axisY = new QValueAxis;
    axisY->setRange(-100, 100);
    axisY->setTitleText("Return");

    chart_->addAxis(axisX, Qt::AlignBottom);
    series_->attachAxis(axisX);
    chart_->addAxis(axisY, Qt::AlignLeft);
    series_->attachAxis(axisY);
    chart_->legend()->hide();

    chart_->setTitle("Total return of each episodes");

    ui_->chartView->setChart(chart_);

    timer_ = new QTimer(this);
    QObject::connect(timer_, &QTimer::timeout, this, [this](){ this->loadData(); });
}

MainWindow::~MainWindow()
{
    delete series_;
    delete chart_;
    delete ui_;
}

void MainWindow::loadData(bool force)
{
    const QString dataPath = QDir(dirName_).filePath("train_output.csv");

    fsWatcher_->addPath(dataPath);
    if (fileUpdatedMap_.find(dataPath) == fileUpdatedMap_.end())
        fileUpdatedMap_[dataPath] = true;

    if (!force && !fileUpdatedMap_[dataPath])
        return;

    emit fetchUpdate(dataPath, dataFrame_.size());
}

void MainWindow::updateData(QList<QList<double>> data, QList<int> index, QList<QString> header)
{
    dataframe_t updateDataFrame { data, index, header };
    dataFrame_.concatenate(updateDataFrame);

    const QString dataPath = QDir(dirName_).filePath("train_output.csv");
    fileUpdatedMap_[dataPath] = false;

    renderData();
}

void MainWindow::selectModel()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "",
                                                        QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
    if (dirPath.isEmpty()) return;
    // TODO: other integrity checks for model files

    QListWidget *modelList = ui_->modelList;
    auto existingItems = modelList->findItems(dirPath, Qt::MatchExactly);
    if (existingItems.empty())
    {
        modelList->addItem(dirPath);
        modelList->setCurrentRow(modelList->count() - 1);
    }
    else
        modelList->setCurrentItem(existingItems[0]);

    setDataDir(dirPath);
}

void MainWindow::setDataDir(QString dataDir)
{
    if (dirName_ == dataDir) return;
    dirName_ = dataDir;
    dataFrame_ = dataframe_t{};
    series_->clear();
    timer_->stop();

    QFile modelSettings(QDir(dirName_).filePath("model_settings.json"));
    if (modelSettings.open(QIODevice::ReadOnly))
    {
        QJsonDocument modelJson(QJsonDocument::fromJson(modelSettings.readAll()));

        ui_->label->setText(QString("State: %1").arg(modelJson["state_dim"].toInt()));
    } else
    {
        qWarning("Cannot open model settings.");
    }

    loadData(true);

    timer_->start(1000);
}

void MainWindow::renderData()
{
    // series_->clear();
    int windowSize = 50;
    double rollingSum = 0.0;
    QList<double> movingAverage;
    for (qsizetype i = 0; i < dataFrame_.size(); i++) {
        if (i >= windowSize) rollingSum -= dataFrame_.at(0, i - windowSize);
        rollingSum += dataFrame_.at(0, i);
        if (i < windowSize) continue;
        movingAverage.push_back(rollingSum / windowSize);
    }

    if (movingAverage.empty()) return;

    auto axisX = chart_->axes().at(0);
    axisX->setRange(0, dataFrame_.size());

    auto axisY = chart_->axes().at(1);
    axisY->setRange(*std::min_element(movingAverage.begin(), movingAverage.end()) - 1,
                    *std::max_element(movingAverage.begin(), movingAverage.end()) + 1);

    int maxX {0};
    if (series_->count() > 0)
        maxX = (int) series_->at(series_->count() - 1).x();
    for (qsizetype i = std::max(windowSize, maxX); i < dataFrame_.size(); i++)
        series_->append(i, movingAverage[i - windowSize]);
}

void MainWindow::onDataFileChanged(const QString &path)
{
    fileUpdatedMap_[path] = true;
}

void MainWindow::onModelListItemDoubleClicked(QListWidgetItem *item)
{
    setDataDir(item->text());
}

