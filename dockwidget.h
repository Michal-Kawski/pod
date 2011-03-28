#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>
#include <QVector>

class QSlider;
class QLineEdit;
class QLabel;

class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget *parent = 0);
    ~DockWidget();
    void setKrgb(QVector< QVector<int> > *vector);
    void setMaxValues(QVector<int> max);

private:
    QVector< QVector<int> > *krgb;
    QVector<int> maxValues;
	QVector<QSlider*> sliders;
    QVector<QLabel*> gLabels;
    QVector<QLineEdit*> gLinesEdit;
    QVector<QLabel *> aLabels;
    QVector<QLineEdit *> aLinesEdit;
    void paintEvent(QPaintEvent *event);
    void changeHistogramValues(int histogram);
    int calculateColorValue(int color, int gMin, float alfa);

signals:
    void changeHistogram(int color, int  gMin, float alfa, QVector<int> histValues);

public slots:
	void sliderValueChanged();
};

#endif // DOCKWIDGET_H
