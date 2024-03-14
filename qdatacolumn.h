#ifndef QDATACOLUMN_H
#define QDATACOLUMN_H

#include <QList>
#include <QVariant>

template <typename T, typename U, typename V>
class QDataColumn
{
public:
    QDataColumn(QList<T> &data, QList<U> *index, V &header):
        data_(data), index_(index), header_(header)
    {
        if (data_.size() != index_->size())
            throw std::runtime_error("inconsistent length of data and index");
    }
    inline qsizetype size() const { return data_.size(); }
    inline V header() const { return header_; }
    inline QList<T> data() const { return data_; }
    inline QList<U> index() const { return *index_; }
    inline T at(const qsizetype &i) const { return data_.at(i); }
    inline T min() const { return *std::min_element(data_.begin(), data_.end()); }
    inline T max() const { return *std::max_element(data_.begin(), data_.end()); }

    void concatenate(const QDataColumn<T, U, V> &other)
    {
        if (header_ != other.header())
            throw std::runtime_error("Inconsistent header for data columns");
        data_.append(other.data());
    }

private:
    QList<T> data_;
    QList<U> *index_;
    V header_;
};

#endif // QDATACOLUMN_H
