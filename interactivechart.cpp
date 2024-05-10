#include "interactivechart.h"

QPair<qsizetype, qsizetype> InteractiveChart::getRangeAxisX()
{
    return { std::max(minX_, maxX_ - rangeX_), maxX_ };
}

void InteractiveChart::setRangeAxisX(qsizetype availMin, qsizetype availMax)
{
    auto axisX = axes().at(0);
    minX_ = availMin, maxX_ = availMax;
    auto [axisMin, axisMax] = getRangeAxisX();
    axisX->setRange(axisMin, axisMax);
}

void InteractiveChart::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    rangeX_ += event->delta();
    if (rangeX_ > maxX_ - minX_) rangeX_ = maxX_ - minX_;
    if (rangeX_ < 100) rangeX_ = 100;
    qDebug() << rangeX_;
    emit chartUpdated();
}
