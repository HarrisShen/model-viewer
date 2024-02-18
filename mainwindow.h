#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
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
    void selectFile();

private:
    Ui::MainWindow *ui;

    QString fileName;
    QDataFrame<double, int, QString> dataFrame;

    QChart *chart;
    QLineSeries *series;
};
#endif // MAINWINDOW_H
