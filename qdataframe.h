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
        _index(index), _header(header)
    {
        if (data.size() != header.size())
            throw std::runtime_error("inconsistent number of columns");
        for (qsizetype i = 0; i < data.size(); i++) {
            if (data[i].size() != _index.size())
                throw std::runtime_error("inconsistent number of rows");
            _columns.emplace_back(data[i], &_index, header[i]);
            _col_map[header[i]] = i;
        }
    }
    inline qsizetype size() const { return _columns[0].size(); }
    inline QPair<qsizetype, qsizetype> shape() const { return qMakePair(size(), _header.size()); }
    inline QList<V> columns() const { return _header; }
    inline QList<U> index() const { return _index; }
    inline U indexAt(const qsizetype &i) const { return _index.at(i); }
    inline const column_t& column(const qsizetype &i) const { return _columns.at(i); }
    inline T at(const qsizetype &i, const qsizetype &j) const { return column(i).at(j); }
private:
    QList<column_t> _columns;
    QList<U> _index;
    QList<V> _header;
    QMap<V, qsizetype> _col_map;
};

template<typename T, typename U, typename V>
QDataFrame<T, U, V> readCsv(const QString &fileName, int index_col = 0, int header_row = 0);

template<typename T>
T string_cast(const QString &s);

template<typename T, typename U, typename V>
QDataFrame<T, U, V> readCsv(const QString &fileName, int index_col, int header_row)
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
        auto lineValues = line.split(",");
        if (!data.size())
            data = QList<QList<T>>(lineValues.size() - 1, QList<T>());
        if (l == header_row) {
            for (qsizetype i = 0; i < lineValues.size(); i++) {
                if (i == index_col) continue;
                header.append(string_cast<V>(lineValues[i]));
            }
            continue;
        }
        for (qsizetype i = 0; i < lineValues.size(); i++) {
            if (i == index_col)
                index.append(string_cast<U>(lineValues[i]));
            else if (i < index_col)
                data[i].append(string_cast<T>(lineValues[i]));
            else
                data[i - 1].append(string_cast<T>(lineValues[i]));
        }
    }
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
