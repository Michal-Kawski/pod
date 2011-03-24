#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>

class Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = 0);
    void mouseReleaseEvent(QMouseEvent *ev);

signals:

public slots:

};

#endif // SLIDER_H
