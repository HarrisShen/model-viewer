#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

template <typename T>
void readCsv(const QString& fileName, T& out, bool header = true)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        auto lineValues = line.split(",");
        if (header) {
            qDebug() << "Header: " << lineValues.join(", ") << '\n';
            header = false;
        }
    }
}
