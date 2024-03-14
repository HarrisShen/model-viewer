#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>

class DataLoader : public QObject
{
    Q_OBJECT
public:
    explicit DataLoader(QObject *parent = nullptr);

public slots:
    void loadData(QString filePath, qsizetype skipRows);

signals:
    void resultReady(QList<QList<double>> data, QList<int> index, QList<QString> header);
};

#endif // DATALOADER_H
