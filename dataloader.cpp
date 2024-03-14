#include "dataloader.h"
#include "qdataframe.h"

DataLoader::DataLoader(QObject *parent)
    : QObject{parent}
{}

void DataLoader::loadData(QString filePath, qsizetype skipRows)
{
    auto dataFrame = readCsv<double, int, QString>(filePath, 0, 0, skipRows);
    emit resultReady(dataFrame.data(), dataFrame.index(), dataFrame.columns());
}
