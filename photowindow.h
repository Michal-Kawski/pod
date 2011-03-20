#ifndef PHOTOWINDOW_H
#define PHOTOWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QPaintEvent>
//#include <QDockWidget>
#include <QVector>

#include "dockwidget.h"


namespace Ui {
    class PhotoWindow;
}

class PhotoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PhotoWindow(QWidget *parent = 0, QString newUrl = "photo.jpg", uint noumber = 1);
    ~PhotoWindow();
    virtual void paintEvent(QPaintEvent *);

private:
    Ui::PhotoWindow *ui;
    QString url;
    QImage* photo;
    QPainter* painter;
    DockWidget *dockWidget;
    QVector< QVector<int> > krgb;
    void closeEvent(QCloseEvent *);
    void focusInEvent(QFocusEvent *);
    void drawHistograms(QImage* photo);

public slots:
    void saveToFile(PhotoWindow *p);

signals:
    void eraseThis(PhotoWindow* p);
    void markThisAsCurrent(PhotoWindow* p);

private slots:
    void on_actionGeneruj_histogramy_triggered();
};

#endif // PHOTOWINDOW_H
