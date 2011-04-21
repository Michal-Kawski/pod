#include "dockwidget.h"

#include <QDebug>
#include <QPainter>
#include <QPen>
#include <cmath>
#include <QSlider>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

DockWidget::DockWidget(QWidget *parent, bool grayScale) :
    QDockWidget(parent)
{
        setWindowTitle("Histogramy");

    krgb = new QVector< QVector<int> >(1);
    this->grayScale = grayScale;

    if(grayScale){
        sliders.append(new QSlider(this));
        sliders.at(0)->setGeometry(300, 70, 256, 18);
        sliders.at(0)->setMaximum(256);
        sliders.at(0)->setMinimum(0);
        sliders.at(0)->setOrientation(Qt::Horizontal);
        sliders.at(0)->setEnabled(false);
        connect(sliders.at(0), SIGNAL(sliderReleased()), this, SLOT(sliderValueChanged()));

        gLabels.append(new QLabel("g_min:", this));
        gLabels.at(0)->setGeometry(300, 90, 30, 20);

        gLinesEdit.append(new QLineEdit("0", this));
        gLinesEdit.at(0)->setGeometry(332, 90, 40, 20);
        connect(gLinesEdit.at(0), SIGNAL(textChanged(QString)), this, SLOT(lineEditChanged()));

        aLabels.append(new QLabel("alfa:", this));
        aLabels.at(0)->setGeometry(385, 90, 40, 20);

        aLinesEdit.append(new QLineEdit("0", this));
        aLinesEdit.at(0)->setGeometry(410, 90, 40, 20);

        buttons.append(new QPushButton("Transform", this));
        buttons.at(0)->setGeometry(480, 120, 100, 20);
        connect(buttons.at(0), SIGNAL(released()), this, SLOT(buttonClicked()));
        resize(600, 300);
    }else{
        for(int i=0; i<4; i++){
            sliders.append(new QSlider(this));
            sliders.at(i)->setGeometry(300, i * 160 + 70, 256, 18);
            sliders.at(i)->setMaximum(256);
            sliders.at(i)->setMinimum(0);
            sliders.at(i)->setOrientation(Qt::Horizontal);
            sliders.at(i)->setEnabled(false);
                    connect(sliders.at(i), SIGNAL(sliderReleased()), this, SLOT(sliderValueChanged()));

            gLabels.append(new QLabel("g_min:", this));
            gLabels.at(i)->setGeometry(300, i*160 + 90, 30, 20);

            gLinesEdit.append(new QLineEdit("0", this));
            gLinesEdit.at(i)->setGeometry(332, i*160 + 90, 40, 20);
            connect(gLinesEdit.at(i), SIGNAL(textChanged(QString)), this, SLOT(lineEditChanged()));

            aLabels.append(new QLabel("alfa:", this));
            aLabels.at(i)->setGeometry(385, i*160 + 90, 40, 20);

            aLinesEdit.append(new QLineEdit("0", this));
            aLinesEdit.at(i)->setGeometry(410, i*160 + 90, 40, 20);

            buttons.append(new QPushButton("Transform", this));
            buttons.at(i)->setGeometry(480, i*160 + 120, 100, 20);
            connect(buttons.at(i), SIGNAL(released()), this, SLOT(buttonClicked()));
        }
        resize(600, 680);
    }

    setFloating(true);
        hide();
}

DockWidget::~DockWidget(){
    for(int i=0; i<sliders.size(); i++)
        delete sliders.at(i);
    for(int i=0; i<gLabels.size(); i++)
        delete gLabels.at(i);
    for(int i=0; i<gLinesEdit.size(); i++)
        delete gLinesEdit.at(i);
    for(int i=0; i<aLabels.size(); i++)
        delete aLabels.at(i);
    for(int i=0; i<aLinesEdit.size(); i++)
        delete aLinesEdit.at(i);
    delete krgb;
}

void DockWidget::paintEvent(QPaintEvent */*event*/)
{
    if(krgb->size() > 1){
        if(sliders.size() > 1){
            for(int i=0; i<sliders.size(); i++){
                sliders.at(i)->setEnabled(true);
            }

            QVector<double> scales(4);
            scales[0] = 153.00 / (double)maxValues[0];
            scales[1] = 153.00 / (double)maxValues[1];
            scales[2] = 153.00 / (double)maxValues[2];
            scales[3] = 153.00 / (double)maxValues[3];

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
        }else{
            sliders.at(0)->setEnabled(true);

            QVector<double> scales(4);
            scales[0] = 153.00 / (double)maxValues[0];

            QPainter painter(this);

            double result;
            for(int i=0; i<256; i++){
                result = (double)krgb->at(0).at(i) * scales[0];
                painter.setPen(QPen(QColor(0, 0, 0)));
                painter.drawLine(i, 160, i, 160 - round(result));
            }
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

void DockWidget::sliderValueChanged(){
        QSlider *s = qobject_cast<QSlider *>(QObject::sender());
    for(int i=0; i<sliders.size(); i++){
        if(sliders.at(i) == s){
            switch(i){
            case 0:
                gLinesEdit.at(i)->setText(QString::number(sliders.at(i)->value()));
                break;
            case 1:
                gLinesEdit.at(i)->setText(QString::number(sliders.at(i)->value()));
                break;
            case 2:
                gLinesEdit.at(i)->setText(QString::number(sliders.at(i)->value()));
                break;
            case 3:
                gLinesEdit.at(i)->setText(QString::number(sliders.at(i)->value()));
                break;
            }
        }
    }
}

void DockWidget::buttonClicked(){
    QPushButton *bt = qobject_cast<QPushButton *>(QObject::sender());
    for(int i=0; i<buttons.size(); i++){
        if(buttons.at(i) == bt){
            switch(i){
            case 0:
                emit changeHistogram(0, sliders.at(i)->value(), aLinesEdit.at(i)->text().toInt());
                break;
            case 1:
                emit changeHistogram(1, sliders.at(i)->value(), aLinesEdit.at(i)->text().toInt());
                break;
            case 2:
                emit changeHistogram(2, sliders.at(i)->value(), aLinesEdit.at(i)->text().toInt());
                break;
            case 3:
                emit changeHistogram(3, sliders.at(i)->value(), aLinesEdit.at(i)->text().toInt());
                break;
            }
        }
    }
}

void DockWidget::lineEditChanged(){
    QLineEdit *line = qobject_cast<QLineEdit *>(QObject::sender());
    for(int i=0; i<gLinesEdit.size(); i++){
        if(gLinesEdit.at(i) == line){
            int value = gLinesEdit.at(i)->text().toInt();
            if(value < 0)
                value = 0;
            if(value > 255)
                value = 255;
            sliders.at(i)->setValue(value);
        }
    }
}
