#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>
#include <QVector>

#include "slider.h"


class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget *parent = 0);
    ~DockWidget();
    void dockWidgetRepaint();
    void setKrgb(QVector< QVector<int> > *vector);
    void setMaxValues(QVector<int> max);

private:
    QVector< QVector<int> > *krgb;
    QVector<int> maxValues;
    void paintEvent(QPaintEvent *event);
    QVector<Slider*> sliders;

signals:

public slots:

};

#endif // DOCKWIDGET_H
