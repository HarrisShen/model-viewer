#ifndef QDATAFRAME_H
#define QDATAFRAME_H

#include "qdatacolumn.h"
#include <QFile>
#include <QList>
#include <QMap>
#include <QVariant>

template <typename T, typename U, typename V>
class QDataFrame
{
    using column_t = QDataColumn<T, U, V>;
public:
    QDataFrame() = default;
    QDataFrame(QList<QList<T>> &data, QList<U> &index, QList<V> &header) :
        index_(index), header_(header)
    {
        if (data.size() != header.size())
            throw std::runtime_error("inconsistent number of columns");
        for (qsizetype i = 0; i < data.size(); i++) {
            if (data[i].size() != index_.size())
                throw std::runtime_error("inconsistent number of rows");
            columns_.emplace_back(data[i], &index_, header[i]);
            col_map_[header[i]] = i;
        }
    }
    inline qsizetype size() const { return columns_.size() == 0 ? 0 : columns_[0].size(); }
    inline QList<V> columns() const { return header_; }
    inline QPair<qsizetype, qsizetype> shape() const { return qMakePair(size(), header_.size()); }

    inline QList<U> index() const { return index_; }
    inline U indexAt(const qsizetype &i) const { return index_.at(i); }
    inline column_t column(const qsizetype &i) const { return columns_.at(i); }
    inline T at(const qsizetype &i, const qsizetype &j) const { return column(i).at(j); }
    inline QList<QList<T>> data() const
    {
        QList<QList<T>> d(columns_.size());
        for (qsizetype i = 0; i < columns_.size(); i++)
            d[i] = columns_[i].data();
        return d;
    }

    void appendRow(QList<T> row, U &ind);
    void concatenate(const QDataFrame<T, U, V> &other)
    {
        if (size() == 0)
        {
            *this = other;
            return;
        }
        for(qsizetype i = 0; i < columns_.size(); i++) {
            columns_[i].concatenate(other.column(i));
            index_.append(other.index());
        }
    }
private:
    QList<column_t> columns_;
    QList<U> index_;
    QList<V> header_;
    QMap<V, qsizetype> col_map_;
};

template<typename T, typename U, typename V>
QDataFrame<T, U, V> readCsv(const QString &fileName, qsizetype indexCol = 0, qsizetype headerRow = 0, qsizetype skipRows = 0);

template<typename T>
T string_cast(const QString &s);

template<typename T, typename U, typename V>
QDataFrame<T, U, V> readCsv(const QString &fileName, qsizetype indexCol, qsizetype headerRow, qsizetype skipRows)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QDataFrame<T, U, V>();
    QList<QList<T>> data {};
    QList<U> index {};
    QList<V> header {};
    QTextStream in(&file);
    for (qsizetype l = 0; !in.atEnd(); l++) {
        QString line = in.readLine();
        if (l < skipRows + (int) (l > headerRow) && l != headerRow)
            continue;
        auto lineValues = line.split(",");
        if (!data.size())
            data = QList<QList<T>>(lineValues.size() - 1, QList<T>());
        if (l == headerRow) {
            for (qsizetype i = 0; i < lineValues.size(); i++) {
                if (i == indexCol) continue;
                header.append(string_cast<V>(lineValues[i]));
            }
            continue;
        }
        for (qsizetype i = 0; i < lineValues.size(); i++) {
            if (i == indexCol)
                index.append(string_cast<U>(lineValues[i]));
            else if (i < indexCol)
                data[i].append(string_cast<T>(lineValues[i]));
            else
                data[i - 1].append(string_cast<T>(lineValues[i]));
        }
    }
    file.close();
    return QDataFrame<T, U, V>(data, index, header);
}

template<typename T>
T string_cast(const QString &s)
{
    QVariant qv(s);
    if (qv.canConvert<T>())
        return qv.value<T>();
    return T();
}

#endif // QDATAFRAME_H
