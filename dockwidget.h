#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>
#include <QVector>

class QSlider;
class QLineEdit;
class QLabel;
class QPushButton;

class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget *parent = 0, bool grayScale = true);
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
    QVector<QPushButton *> buttons;
    bool grayScale;
    void paintEvent(QPaintEvent *event);
    int calculateColorValue(int color, int gMin, float alfa);

signals:
    void changeHistogram(int color, int  gMin, float alfa);

public slots:
    void buttonClicked();
    void lineEditChanged();
        void sliderValueChanged();
};

#endif // DOCKWIDGET_H
