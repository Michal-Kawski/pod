#ifndef TRANSFORMWINDOW_H
#define TRANSFORMWINDOW_H

#include "displaywindow.h"

#include <QHash>
#include <QUuid>

namespace Ui {
	class TransformWindow;
}

class FilterInterface;

class TransformWindow : public DisplayWindow
{
	Q_OBJECT

public:
	explicit TransformWindow(QImage magnitude, QImage phase, QString title, QWidget *parent = 0);
	~TransformWindow();

private:
	void constructorInternals(QString title);
	void appendFilter(FilterInterface *filter);

	Ui::TransformWindow *ui;
	QImage mMagnitudeImage;
	QImage mPhaseImage;
	QHash<QUuid, FilterInterface *> mFiltersHash;
	QMenu *mFiltersMenu;

private slots:
	void applyFilter(QAction *action);
};

#endif // TRANSFORMWINDOW_H
