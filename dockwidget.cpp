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

    for(int i=0; i<4; i++){
        sliders.append(new Slider(this));
        sliders.at(i)->setGeometry(300, i * 160 + 70, 256, 18);
        sliders.at(i)->setMaximum(256);
        sliders.at(i)->setMinimum(0);
        sliders.at(i)->setOrientation(Qt::Horizontal);
    }

    setFloating(true);
    resize(600, 680);
}

DockWidget::~DockWidget(){
    for(int i=0; i<sliders.size(); i++)
        delete sliders.at(i);
    delete krgb;
}

void DockWidget::dockWidgetRepaint(){
    repaint();
}

void DockWidget::paintEvent(QPaintEvent *event){
    if(krgb->size() > 1){
        QVector<double> scales(4);
        qDebug()<<maxValues;
        scales[0] = 153.00 / (double)maxValues[0];
        scales[1] = 153.00 / (double)maxValues[1];
        scales[2] = 153.00 / (double)maxValues[2];
        scales[3] = 153.00 / (double)maxValues[3];

        qDebug()<<"scales: "<<scales;

        QPainter painter(this);
        double result;
        for(int i=0; i<256; i++){
            result = (double)krgb->at(0).at(i) * scales[0];
            painter.setPen(QPen(QColor(0, 0, 0)));
            painter.drawLine(i, 160, i, 160 - round(result));

            result = (double)krgb->at(1).at(i) * scales[1];
            painter.setPen(QPen(QColor(255, 0, 0)));
            painter.drawLine(i, 320, i, 320 - round(result));

            result = (double)krgb->at(2).at(i) * scales[2];
            painter.setPen(QPen(QColor(0, 255, 0)));
            painter.drawLine(i, 480, i, 480 - round(result));

            result = (double)krgb->at(3).at(i) * scales[3];
            painter.setPen(QPen(QColor(0, 0, 255)));
            painter.drawLine(i, 640, i, 640 - round(result));
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
