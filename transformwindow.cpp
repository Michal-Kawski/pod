#include "transformwindow.h"
#include "ui_transformwindow.h"
#include "displaywindow.h"
#include "filterinterface.h"
#include "fft.h"
#include "colorparser.h"
#include "photowindow.h"
#include "highpassfilter.h"

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

TransformWindow::TransformWindow(ComplexArray *ca, QImage::Format format, QString title, QWidget *parent) :
	DisplayWindow(parent),
	ui(new Ui::TransformWindow),
	mCA(ca)
{
	QSize size(ca->shape()[1], ca->shape()[2]);
	mPhaseImage = QImage(size, format);
	mMagnitudeImage = QImage(size, format);
	if (format == QImage::Format_Indexed8) {
		QVector<QRgb> colors;
		colors.reserve(256);
		for (int i = 0; i < 256; i++) {
			colors << qRgb(i, i, i);
		}
		mPhaseImage.setColorTable(colors);
		mMagnitudeImage.setColorTable(colors);
		mPhaseImage.fill(0);
		mMagnitudeImage.fill(0);
	} else {
		mPhaseImage.fill(Qt::black);
		mMagnitudeImage.fill(Qt::black);
	}
	for (unsigned int i = 0; i < mCA->shape()[0]; i++) {
		qreal minp = 0;
		qreal maxp = 0;
		qreal minm = 0;
		qreal maxm = 0;
		for (unsigned int j = 0; j < mCA->shape()[1]; j++) {
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				qreal phase = (*mCA)[i][j][k].phase();
				qreal magnitude = (*mCA)[i][j][k].abs();
				if (phase > maxp) {
					maxp = phase;
				} else if (phase < minp) {
					minp = phase;
				}
				if (magnitude > maxm) {
					maxm = magnitude;
				} else if (magnitude < minm) {
					minm = magnitude;
				}
			}
		}

		ColorParser cp(format);
		for (unsigned int j = 0; j < mCA->shape()[1]; j++) {
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				qreal p = ((*mCA)[i][j][k].phase() - minp) / (maxp - minp) * 255.0;
				{
					QVector3D oldPixel = cp.pixel(k, j, mPhaseImage);
					QVector3D newPixel;
					switch (i) {
						case 0:
							newPixel.setX(p);
							break;
						case 1:
							newPixel.setY(p);
							break;
						case 2:
							newPixel.setZ(p);
							break;
						default:
							break;
					}
					cp.setPixel(k, j, mPhaseImage, cp.merge(oldPixel, newPixel));
				}

				// logarithmic scale
				// implementaion: http://homepages.inf.ed.ac.uk/rbf/HIPR2/pixlog.htm
				// idea: http://homepages.inf.ed.ac.uk/rbf/HIPR2/fourier.htm#guidelines
				p = (*mCA)[i][j][k].abs();
				{
					qreal c = 255.0 / log(1.0 + abs(maxm - minm));
					p = c * log(1.0 + p);
					QVector3D oldPixel = cp.pixel(k, j, mMagnitudeImage);
					QVector3D newPixel;
					switch (i) {
						case 0:
							newPixel.setX(p);
							break;
						case 1:
							newPixel.setY(p);
							break;
						case 2:
							newPixel.setZ(p);
							break;
						default:
							break;
					}
					cp.setPixel(k, j, mMagnitudeImage, cp.merge(oldPixel, newPixel));
				}
			}
		}
	}
	rearrangeQuadrants(mPhaseImage, mMagnitudeImage);
	constructorInternals(title);
}

TransformWindow::~TransformWindow()
{
    delete ui;
	delete mCA;
}

void TransformWindow::rearrangeQuadrants(QImage &phase, QImage &magnitude) const
{
	// http://paulbourke.net/miscellaneous/imagefilter/
	int hw = phase.width() / 2;
	int hh = phase.height() / 2;
	if (phase.format() != QImage::Format_Indexed8) {
		for (int i = 0; i < hw; i++) {
			for (int j = 0; j < hh; j++) {
				QRgb tempColor = phase.pixel(i, j);
				phase.setPixel(i, j, phase.pixel(i + hw, j + hh));
				phase.setPixel(i + hw, j + hh, tempColor);

				tempColor = phase.pixel(i + hw, j);
				phase.setPixel(i + hw, j, phase.pixel(i, j + hh));
				phase.setPixel(i, j + hh, tempColor);


				tempColor = magnitude.pixel(i, j);
				magnitude.setPixel(i, j, magnitude.pixel(i + hw, j + hh));
				magnitude.setPixel(i + hw, j + hh, tempColor);

				tempColor = magnitude.pixel(i + hw, j);
				magnitude.setPixel(i + hw, j, magnitude.pixel(i, j + hh));
				magnitude.setPixel(i, j + hh, tempColor);
			}
		}
	} else {
		for (int i = 0; i < hw; i++) {
			for (int j = 0; j < hh; j++) {
				int tempColor = phase.pixelIndex(i, j);
				phase.setPixel(i, j, phase.pixelIndex(i + hw, j + hh));
				phase.setPixel(i + hw, j + hh, tempColor);

				tempColor = phase.pixelIndex(i + hw, j);
				phase.setPixel(i + hw, j, phase.pixelIndex(i, j + hh));
				phase.setPixel(i, j + hh, tempColor);


				tempColor = magnitude.pixelIndex(i, j);
				magnitude.setPixel(i, j, magnitude.pixelIndex(i + hw, j + hh));
				magnitude.setPixel(i + hw, j + hh, tempColor);

				tempColor = magnitude.pixelIndex(i + hw, j);
				magnitude.setPixel(i + hw, j, magnitude.pixelIndex(i, j + hh));
				magnitude.setPixel(i, j + hh, tempColor);
			}
		}
	}
}

void TransformWindow::constructorInternals(QString title)
{
	ui->setupUi(this);
	ui->magnitudeDisplayLabel->setPixmap(QPixmap::fromImage(mMagnitudeImage));
	ui->phaseDisplayLabel->setPixmap(QPixmap::fromImage(mPhaseImage));
	setWindowTitle(title);

	mFiltersMenu = menuBar()->addMenu("Filters");

	connect(mFiltersMenu, SIGNAL(triggered(QAction*)), this, SLOT(applyFilter(QAction*)));

	appendFilter(new HighPassFilter(this));

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
	FilterData fd;
	fd.image = mMagnitudeImage;
	fd.transformData.resize(boost::extents[mCA->shape()[0]][mCA->shape()[1]][mCA->shape()[2]]);
	fd.transformData = *mCA;
	if (filter->setup(fd)) {
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
						case 0:
							newPixel.setX(p);
							break;
						case 1:
							newPixel.setY(p);
							break;
						case 2:
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
	result = result.rgbSwapped();
	PhotoWindow *pw = new PhotoWindow(result, windowTitle() + ", IFFT", qobject_cast<QWidget*>(parent()));
	pw->show();
	delete ca;
}
