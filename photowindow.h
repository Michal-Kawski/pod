#ifndef PHOTOWINDOW_H
#define PHOTOWINDOW_H

#include "dockwidget.h"
#include "filterinterface.h"

#include <QMainWindow>
#include <QHash>
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
	explicit PhotoWindow(QImage img, QString title, QWidget *parent = 0);
	~PhotoWindow();

private:
	Ui::PhotoWindow *ui;
    DockWidget *dockWidget;
    QVector< QVector<int> > krgb;
	QHash<QUuid, FilterInterface *> mFiltersHash;
	QMenu *mFiltersMenu;
	QImage mImage;
    void drawHistograms(QImage* photo);
    int calculateRaleigh(int position, int gMin, float alfa, QVector<int> histValues);
	void appendFilter(FilterInterface *filter);
        void constructorInternals(const QString &title);
    QVector<int> findMaxValues();
    void changeImageColor(int color, int oldValue, int newValue, QImage *img);

private slots:
	void applyFilter(QAction *action);
	void on_actionGeneruj_histogramy_triggered();
	void on_actionSave_triggered();
        void changeHistogram(int color, int gMin, float alfa);
	void qualityCheck();
};

#endif // PHOTOWINDOW_H
