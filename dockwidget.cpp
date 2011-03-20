#include "dockwidget.h"

#include <QDebug>
#include <QPainter>
#include <QPen>
#include <math.h>


DockWidget::DockWidget(QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle("Histogramy");
    setParent(parent);
    krgb = new QVector< QVector<int> >(1);

    setFloating(true);
    resize(600, 600);
}

DockWidget::~DockWidget(){
    delete krgb;
}

void DockWidget::dockWidgetRepaint(){
    repaint();
}

void DockWidget::paintEvent(QPaintEvent *event){
    if(krgb->size() > 1){
        QVector<double> scales(4);
        qDebug()<<maxValues;
        scales[0] = 256.00 / (double)maxValues[0];
        scales[1] = 256.00 / (double)maxValues[1];
        scales[2] = 256.00 / (double)maxValues[2];
        scales[3] = 256.00 / (double)maxValues[3];

        qDebug()<<"scales: "<<scales;

        QPainter painter(this);
        double result;
        for(int i=0; i<256; i++){
            result = (double)krgb->at(0).at(i) * scales[0];
            painter.setPen(QPen(QColor(0, 0, 0)));
            painter.drawLine(i, 260, i, 260 - round(result));

            result = (double)krgb->at(1).at(i) * scales[1];
            painter.setPen(QPen(QColor(255, 0, 0)));
            painter.drawLine(300 + i, 260, 300 + i, 260 - round(result));

            result = (double)krgb->at(2).at(i) * scales[2];
            painter.setPen(QPen(QColor(0, 255, 0)));
            painter.drawLine(i, 560, i, 560 - round(result));

            result = (double)krgb->at(3).at(i) * scales[3];
            painter.setPen(QPen(QColor(0, 0, 255)));
            painter.drawLine(300 + i, 560, 300 + i, 560 - round(result));
        }
    }
}

void DockWidget::setKrgb(QVector< QVector<int> > *vector){
    delete krgb;
    this->krgb = new QVector< QVector<int> > (*vector);
}

void DockWidget::setMaxValues(QVector<int> max){
    maxValues = QVector<int> (max);
}
