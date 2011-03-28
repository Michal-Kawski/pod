#include "slider.h"

#include <QDebug>

Slider::Slider(QWidget *parent) :
    QSlider(parent)
{
    setParent(parent);
}

void Slider::mouseReleaseEvent(QMouseEvent *ev){
    emit sliderValueChanged(this);
}
