#include "transformwindow.h"
#include "ui_transformwindow.h"
#include "displaywindow.h"
#include "filterinterface.h"
#include "fft.h"
#include "colorparser.h"
#include "photowindow.h"

#include <QDebug>

Q_DECLARE_METATYPE(QUuid)

TransformWindow::TransformWindow(QImage magnitude, QImage phase, ComplexArray *ca, QString title, QWidget *parent) :
	DisplayWindow(parent),
	ui(new Ui::TransformWindow),
	mMagnitudeImage(magnitude),
	mPhaseImage(phase),
	mCA(ca)
{
	constructorInternals(title);
}

TransformWindow::~TransformWindow()
{
    delete ui;
	delete mCA;
}

void TransformWindow::constructorInternals(QString title)
{
	ui->setupUi(this);
	ui->magnitudeDisplayLabel->setPixmap(QPixmap::fromImage(mMagnitudeImage));
	ui->phaseDisplayLabel->setPixmap(QPixmap::fromImage(mPhaseImage));
	setWindowTitle(title);

	mFiltersMenu = menuBar()->addMenu("Filters");

	connect(mFiltersMenu, SIGNAL(triggered(QAction*)), this, SLOT(applyFilter(QAction*)));

	QAction *fftAction = new QAction("Invert", this);
	connect(fftAction, SIGNAL(triggered()), this, SLOT(invert()));
	menuBar()->addAction(fftAction);
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

void TransformWindow::invert()
{
	FFT fft;
	int layers = mCA->shape()[0];
	int w = mCA->shape()[1];
	int h = mCA->shape()[2];
	ComplexArray *ca = new ComplexArray(boost::extents[layers][w][h]);
	*ca = *mCA;
	fft.perform(ca, true);
	QImage::Format format = mPhaseImage.format();
	ColorParser cp(format);
	QImage result(w, h, format);
	result.fill(Qt::black);
	if (format == QImage::Format_Indexed8 || format == QImage::Format_Mono) {
		QVector<QRgb> colors;
		colors.reserve(256);
		for (int i = 0; i < 256; i++) {
			colors << qRgb(i, i, i);
		}
		result.setColorTable(colors);
	}
	for (unsigned int i = 0; i < ca->shape()[0]; i++) {
		qreal min = 0;
		qreal max = 0;
		for (unsigned int j = 0; j < ca->shape()[1]; j++) {
			for (unsigned int k = 0; k < ca->shape()[2]; k++) {
				qreal real = (*ca)[i][j][k].real();
				if (real > max) {
					max = real;
				} else if (real < min) {
					min = real;
				}
			}
		}

		for (unsigned int j = 0; j < ca->shape()[1]; j++) {
			for (unsigned int k = 0; k < ca->shape()[2]; k++) {
				qreal p = ((*ca)[i][j][k].real() - min) / (max - min) * 255.0;
				{
					QVector3D oldPixel = cp.pixel(k, j, result);
					QVector3D newPixel;
					switch (i) {
						case 2:
							newPixel.setX(p);
							break;
						case 1:
							newPixel.setY(p);
							break;
						case 0:
							newPixel.setZ(p);
							break;
						default:
							break;
					}
					cp.setPixel(k, j, result, cp.merge(oldPixel, newPixel));
				}
			}
		}
	}
	PhotoWindow *pw = new PhotoWindow(result, windowTitle() + ", IFFT", qobject_cast<QWidget*>(parent()));
	pw->show();
	delete ca;
}
