#include "plot.h"
#include "ui_plot.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QList>

#define CHANGE_SPLITTER_SIZE(SPLITTER,SCAL_A,SCAL_B)                      \
    QList<int> listSpl = SPLITTER->sizes();             \
    listSpl.replace(0,this->width()*SCAL_A/(SCAL_A+SCAL_B));       \
    listSpl.replace(1,this->width()*SCAL_B/(SCAL_A+SCAL_B));       \
    SPLITTER->setSizes(listSpl);

Plot::Plot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plot)
{
    ui->setupUi(this);
    CHANGE_SPLITTER_SIZE(ui->vSplitter,15,1)

    ui->QCP_Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->QCP_Plot->axisRect()->setupFullAxesBox();

    ui->QCP_Plot->plotLayout()->insertRow(0);
    plotTitle = new QCPPlotTitle(ui->QCP_Plot, "Potential Field Signals");
    ui->QCP_Plot->plotLayout()->addElement(0, 0, plotTitle);

    ui->QCP_Plot->xAxis->setLabel("time");
    ui->QCP_Plot->yAxis->setLabel("signals");
    ui->QCP_Plot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->QCP_Plot->legend->setFont(legendFont);
    ui->QCP_Plot->legend->setSelectedFont(legendFont);
    ui->QCP_Plot->legend->setSelectableParts(QCPLegend::spItems);

    connect(ui->QCP_Plot, SIGNAL(selectionChangedByUser()), this,
                                                      SLOT(selectionChanged()));
    connect(ui->QCP_Plot, SIGNAL(mousePress(QMouseEvent*)), this,
                                                            SLOT(mousePress()));
    connect(ui->QCP_Plot, SIGNAL(mouseWheel(QWheelEvent*)), this,
                                                            SLOT(mouseWheel()));
    connect(ui->QCP_Plot->xAxis, SIGNAL(rangeChanged(QCPRange)),
                                ui->QCP_Plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->QCP_Plot->yAxis, SIGNAL(rangeChanged(QCPRange)),
                                ui->QCP_Plot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->QCP_Plot, SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)),
                      this, SLOT(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)));
    connect(ui->QCP_Plot,
         SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(ui->QCP_Plot,
      SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)),
              this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

    ui->Yscale->setChecked(true);
    ui->Xscale->setChecked(true);


}

Plot::~Plot()
{
    delete ui;
}

void Plot::titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title)
{
  Q_UNUSED(event)
  // Set the plot title by double clicking on it
  bool ok;
  QString newTitle = QInputDialog::getText(this, "Set title name",
                      "New plot title:", QLineEdit::Normal, title->text(), &ok);
  if (ok)
  {
    title->setText(newTitle);
    ui->QCP_Plot->replot();
  }
}

void Plot::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual
  {                     //axis label is clicked, not tick label or axis backbone
    bool ok;
    QString newLabel = QInputDialog::getText(this, "Set axis label",
                      "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->QCP_Plot->replot();
    }
  }
}

void Plot::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border
            // padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem =qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "Set plot name",
        "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->QCP_Plot->replot();
    }
  }
}

void Plot::mousePress()
{

  if (ui->QCP_Plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
  {
    ui->QCP_Plot->axisRect()->setRangeDrag(ui->QCP_Plot->xAxis->orientation());
    ui->Xscale->setChecked(false);
  }
  else if (ui->QCP_Plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
  {
    ui->QCP_Plot->axisRect()->setRangeDrag(ui->QCP_Plot->yAxis->orientation());
    ui->Yscale->setChecked(false);
  }
  else
  {
      ui->QCP_Plot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
  }
}

void Plot::mouseWheel()
{

  if (ui->QCP_Plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
  {
    ui->QCP_Plot->axisRect()->setRangeZoom(ui->QCP_Plot->xAxis->orientation());
    ui->Xscale->setChecked(false);
  }
  else if (ui->QCP_Plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
  {
    ui->QCP_Plot->axisRect()->setRangeZoom(ui->QCP_Plot->yAxis->orientation());
    ui->Yscale->setChecked(false);
  }
  else
  {
      ui->QCP_Plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
  }
}

void Plot::registerSignalToPlot(QString Name)
{
    SignalToPlot * newSignal = new SignalToPlot(Name);
    signalsToPlot.insert(Name,newSignal);

    newSignal->ptrToGraph = ui->QCP_Plot->addGraph();
    newSignal->ptrToGraph->setName(Name);
    newSignal->ptrToGraph->setPen(QPen(QColor(rand()%245+9)));
    newSignal->ptrToGraph->setLineStyle(QCPGraph::lsLine);

}

void Plot::selectionChanged()
{
  if (ui->QCP_Plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis)
       || ui->QCP_Plot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels)
       || ui->QCP_Plot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) ||
        ui->QCP_Plot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->QCP_Plot->xAxis2->setSelectedParts(QCPAxis::spAxis|
                                                         QCPAxis::spTickLabels);
    ui->QCP_Plot->xAxis->setSelectedParts(QCPAxis::spAxis|
                                                         QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and
  // tick labels as one selectable object:
  if (ui->QCP_Plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis)
       || ui->QCP_Plot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels)
       || ui->QCP_Plot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis)
       || ui->QCP_Plot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->QCP_Plot->yAxis2->setSelectedParts(QCPAxis::spAxis
                                                        |QCPAxis::spTickLabels);
    ui->QCP_Plot->yAxis->setSelectedParts(QCPAxis::spAxis
                                                        |QCPAxis::spTickLabels);
  }

  //synchronize selection of graphs with selection of corresponding legend items
  for (int i=0; i<ui->QCP_Plot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->QCP_Plot->graph(i);
    QCPPlottableLegendItem *legendItem = ui->QCP_Plot->legend
                                                     ->itemWithPlottable(graph);
    if (legendItem->selected() || graph->selected())
    {
      legendItem->setSelected(true);
      graph->setSelectedPen( QPen() );
      graph->setSelected(true);
    }
  }
}

void Plot::addData(QString dataName, QPointF data)
{
    SignalToPlot *currentSignal = signalsToPlot.value(dataName);

    currentSignal->addData(data);
    currentSignal->ptrToGraph->addData(data.x(),data.y());

    QCPRange Tmp=ui->QCP_Plot->xAxis->range();
    if ( ui->Yscale->isChecked() == true )
       ui->QCP_Plot->rescaleAxes();

    if (ui->Xscale->isChecked() == true)
        ui->QCP_Plot->xAxis->setRange(data.x()+1,ui->period->value(),Qt::AlignRight);
    else
        ui->QCP_Plot->xAxis->setRange(Tmp.lower, Tmp.upper);


    currentSignal->ptrToGraph->removeDataBefore(data.x()-ui->timeBehold->value());

    ui->QCP_Plot->replot();


}

void Plot::on_SavePlot_clicked()
{
    QString outName=QFileDialog::getSaveFileName(this, "Save File",
                                            "",
                                            "Images (*.png *.bmp *.jpg) *.pdf");
    QString format=outName.right(3);

    QStringList myFormats;
    myFormats << "png" << "bmp" << "jpg" << "pdf";

    switch (myFormats.indexOf(format))
    {
    case 0: //"png":
        if (ui->cbUseCurrentSize->isChecked())
            ui->QCP_Plot->savePng(outName);
        else ui->QCP_Plot->savePng(outName,
                                   ui->sbWidth->value(), ui->sbHeight->value());
        break;
    case 1: //"bmp":
        if (ui->cbUseCurrentSize->isChecked())
            ui->QCP_Plot->saveBmp(outName);
        else ui->QCP_Plot->saveBmp(outName,
                                   ui->sbWidth->value(), ui->sbHeight->value());
        break;
    case 2: //"jpg":
        if (ui->cbUseCurrentSize->isChecked())
            ui->QCP_Plot->saveJpg(outName);
        else ui->QCP_Plot->saveJpg(outName,
                                   ui->sbWidth->value(), ui->sbHeight->value());
        break;
    case 3: //"pdf":
        if (ui->cbUseCurrentSize->isChecked())
            ui->QCP_Plot->savePdf(outName);
        else ui->QCP_Plot->savePdf(outName,false,
                                   ui->sbWidth->value(), ui->sbHeight->value());
        break;
    }

}



