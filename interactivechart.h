#ifndef INTERACTIVECHART_H
#define INTERACTIVECHART_H

#include <QChart>
#include <QGraphicsSceneWheelEvent>

class InteractiveChart : public QChart
{
    Q_OBJECT
signals:
    void chartUpdated();
public:
    InteractiveChart() {}

    QPair<qsizetype, qsizetype> getRangeAxisX();
    void setRangeAxisX(qsizetype availMin, qsizetype availMax);
private:
    qsizetype minX_, maxX_;
    qsizetype rangeX_ = 1000;

    void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif // INTERACTIVECHART_H
