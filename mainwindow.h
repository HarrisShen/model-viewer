#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QListWidgetItem>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QThread>
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

    using dataframe_t = QDataFrame<double, int, QString>;

signals:
    void fetchUpdate(QString path, int skipRows);

public slots:
    void loadData(bool force = false);
    void updateData(QList<QList<double>> data, QList<int> index, QList<QString> header);
    void selectModel();

private slots:
    void onDataFileChanged(const QString &path);
    void onModelListItemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui_;

    QTimer *timer_;

    QFileSystemWatcher *fsWatcher_;
    QMap<QString, bool> fileUpdatedMap_;
    
    QString dirName_;
    dataframe_t dataFrame_;

    QThread *dataUpdateThread_;

    QChart *chart_;
    QLineSeries *series_;

    void setDataDir(QString dataDir);
    void renderData();
};
#endif // MAINWINDOW_H
