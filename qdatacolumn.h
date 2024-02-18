#ifndef QDATACOLUMN_H
#define QDATACOLUMN_H

#include <QList>
#include <QVariant>

template <typename T, typename U, typename V>
class QDataColumn
{
public:
    QDataColumn(QList<T> &data, QList<U> *index, V &header):
        _data(data), _index(index), _header(header)
    {
        if (_data.size() != _index->size())
            throw std::runtime_error("inconsistent length of data and index");
    }
    inline qsizetype size() const { return _data.size(); }
    inline V header() const { return _header; }
    inline T at(const qsizetype &i) const { return _data.at(i); }
    inline T min() const { return *std::min_element(_data.begin(), _data.end()); }
    inline T max() const { return *std::max_element(_data.begin(), _data.end()); }
private:
    QList<T> _data;
    QList<U> *_index;
    V _header;
};

#endif // QDATACOLUMN_H
