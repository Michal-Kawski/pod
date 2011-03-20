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

class QImage;

class PhotoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PhotoWindow(QWidget *parent = 0, QString newUrl = "photo.jpg", uint noumber = 1);
	~PhotoWindow();

private:
	Ui::PhotoWindow *ui;
    DockWidget *dockWidget;
    QVector< QVector<int> > krgb;
	QImage mImage;
    void closeEvent(QCloseEvent *);
    void focusInEvent(QFocusEvent *);
    void drawHistograms(QImage* photo);

signals:
    void eraseThis(PhotoWindow* p);
    void markThisAsCurrent(PhotoWindow* p);

private slots:
	void on_actionGeneruj_histogramy_triggered();
	void on_actionSave_triggered();
};

#endif // PHOTOWINDOW_H
