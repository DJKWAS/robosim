#include "signaltoplot.h"

SignalToPlot::SignalToPlot()
{
    ptrToGraph=NULL;

    max=-999999;
    min=999999;
}

SignalToPlot::SignalToPlot(QString Name)
{
    plotName=Name;
    ptrToGraph=NULL;

    max=-999999;
    min=999999;
}

void SignalToPlot::addData(QPointF newData)
{
    data.append(newData);
    if (newData.y() < min)
        min = newData.y();
    if (newData.y() > max)
        max = newData.y();
}
