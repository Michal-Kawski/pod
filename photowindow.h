#ifndef PHOTOWINDOW_H
#define PHOTOWINDOW_H

#include "dockwidget.h"
#include "filterinterface.h"

#include <QMainWindow>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QPaintEvent>
//#include <QDockWidget>
#include <QVector>

namespace Ui {
    class PhotoWindow;
}

class QImage;
class QAction;

class PhotoWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit PhotoWindow(QString newUrl, QString title, QWidget *parent = 0);
	~PhotoWindow();

private:
	Ui::PhotoWindow *ui;
    DockWidget *dockWidget;
    QVector< QVector<int> > krgb;
	QHash<QUuid, FilterInterface *> mFiltersHash;
	QMenu *mFiltersMenu;
	QImage mImage;
    void closeEvent(QCloseEvent *);
    void focusInEvent(QFocusEvent *);
    void drawHistograms(QImage* photo);
	void appendFilter(FilterInterface *filter);

signals:
    void eraseThis(PhotoWindow* p);
    void markThisAsCurrent(PhotoWindow* p);

private slots:
	void applyFilter(QAction *action);
	void on_actionGeneruj_histogramy_triggered();
	void on_actionSave_triggered();
};

#endif // PHOTOWINDOW_H
