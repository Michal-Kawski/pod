#include "transformwindow.h"
#include "ui_transformwindow.h"
#include "displaywindow.h"
#include "filterinterface.h"

#include <QDebug>

Q_DECLARE_METATYPE(QUuid)

TransformWindow::TransformWindow(QImage magnitude, QImage phase, QString title, QWidget *parent) :
	DisplayWindow(parent),
	ui(new Ui::TransformWindow),
	mMagnitudeImage(magnitude),
	mPhaseImage(phase)
{
	constructorInternals(title);
}

TransformWindow::~TransformWindow()
{
    delete ui;
}

void TransformWindow::constructorInternals(QString title)
{
	ui->setupUi(this);
	ui->magnitudeDisplayLabel->setPixmap(QPixmap::fromImage(mMagnitudeImage));
	ui->phaseDisplayLabel->setPixmap(QPixmap::fromImage(mPhaseImage));
	setWindowTitle(title);

	mFiltersMenu = menuBar()->addMenu("Filters");

	connect(mFiltersMenu, SIGNAL(triggered(QAction*)), this, SLOT(applyFilter(QAction*)));
}

void TransformWindow::appendFilter(FilterInterface *filter)
{
	mFiltersHash.insert(filter->uuid(), filter);
	QAction *menuAction = new QAction(filter->name(), this);
	QVariant v;
	v.setValue(filter->uuid());
	menuAction->setData(v);
	mFiltersMenu->addAction(menuAction);
}

void TransformWindow::applyFilter(QAction *action)
{
	qDebug() << "applying filter" << action->data().value<QUuid>();
	FilterInterface *filter = mFiltersHash[action->data().value<QUuid>()];
	qDebug() << "filter name:" << filter->name();
	QList<QImage> imageList;
	imageList << mMagnitudeImage << mPhaseImage;
	if (filter->setup(imageList)) {
		DisplayWindow *dw = filter->apply(windowTitle());
		q_check_ptr(dw)->show();
	}
}
