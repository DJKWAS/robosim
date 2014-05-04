#ifndef SIGNALTOPLOT_H
#define SIGNALTOPLOT_H

#include <QString>
#include <QVector>
#include <QPointF>
#include "qcustomplot.h"


class SignalToPlot
{
    public:
        SignalToPlot();
        SignalToPlot(QString Name);

        QString plotName;
        QCPGraph * ptrToGraph;

        void addData(QPointF newData);

        qreal min;
        qreal max;

    private:
       QVector<QPointF> data;


};

#endif // SIGNALTOPLOT_H

