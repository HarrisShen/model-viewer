#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include "qdataframe.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void selectModel();

private slots:
    void onModelListItemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    
    QString dirName;
    QDataFrame<double, int, QString> dataFrame;

    QChart *chart;
    QLineSeries *series;

    void _renderData();
};
#endif // MAINWINDOW_H
