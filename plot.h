#ifndef PLOT_H
#define PLOT_H

#include "signaltoplot.h"

#include <QWidget>
#include <QHash>
#include <QString>

namespace Ui {
class Plot;
}

class Plot : public QWidget
{
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = 0);
    ~Plot();

    void registerSignalToPlot(QString Name);
    void addData(QString dataName, QPointF data);

private:
    Ui::Plot *ui;
    QCPPlotTitle *plotTitle;

    QHash<QString, SignalToPlot*> signalsToPlot;


private slots:
    void titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title);
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();


    void on_SavePlot_clicked();
};

#endif // PLOT_H
