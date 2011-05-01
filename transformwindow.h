#ifndef TRANSFORMWINDOW_H
#define TRANSFORMWINDOW_H

#include "displaywindow.h"
#include "complexarray.h"

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
	explicit TransformWindow(QImage magnitude, QImage phase, ComplexArray *ca, QString title, QWidget *parent = 0);
	explicit TransformWindow(ComplexArray *ca, QImage::Format format, QString title, QWidget *parent);
	~TransformWindow();

private:
	void constructorInternals(QString title);
	void appendFilter(FilterInterface *filter);
	void rearrangeQuadrants(QImage &phase, QImage &magnitude) const;

	Ui::TransformWindow *ui;
	QImage mMagnitudeImage;
	QImage mPhaseImage;
	ComplexArray *mCA;
	QHash<QUuid, FilterInterface *> mFiltersHash;
	QMenu *mFiltersMenu;

private slots:
	void applyFilter(QAction *action);
	void invert();
};

#endif // TRANSFORMWINDOW_H
