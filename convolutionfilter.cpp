#include "convolutionfilter.h"
#include "kernelvaluesdialog.h"
#include "sizedialog.h"
#include "colorparser.h"
#include "photowindow.h"

#include <QDialog>
#include <QLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QVector3D>

#include <QDebug>

ConvolutionFilter::ConvolutionFilter(QObject *parent) :
    FilterInterface(parent)
{
}

bool ConvolutionFilter::setup(const QImage &img)
{
	// get kernel size
	{
		SizeDialog sizeDialog;
		if (sizeDialog.exec() != QDialog::Accepted) {
			return false;
		} else {
			QSize kernelSize = sizeDialog.kernelSize();
			mKernelWidth = kernelSize.width();
			mKernelHeight = kernelSize.height();
		}
	}
	// get kernel values
	{
		KernelValuesDialog kernelValuesDialog(mKernelWidth, mKernelHeight, qobject_cast<QWidget *>(this));
		if (kernelValuesDialog.exec() != QDialog::Accepted) {
			return false;
		}
		mKernel = kernelValuesDialog.values();
	}
	return FilterInterface::setup(img);
}

DisplayWindow *ConvolutionFilter::apply(QString windowBaseName)
{
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
	QVector<QVector<QVector3D> > colors(mImg.height(), QVector<QVector3D>(mImg.width()));
	QVector3D min, max;
	for (int x = 0; x < result.size().width(); x++) {
		for (int y = 0; y < result.size().height(); y++) {
			QVector3D color;
			for (int j = -mKernelHeight; j < mKernelHeight + 1; j++) {
				for (int i = -mKernelWidth; i < mKernelWidth + 1; i++) {
					QPoint pos(x + i, y + j);
					// this point is outside of the image, so just skip it
					if (!mImg.rect().contains(pos)) {
						continue;
					}
					color += cp.pixel(pos, mImg)
							* mKernel.at(j + mKernelHeight).at(i + mKernelWidth);
				}
			}
			colors[y][x] = color;
			if (color.x() > max.x()) {
				max.setX(color.x());
			} else if (color.x() < min.x()) {
				min.setX(color.x());
			}
			if (color.y() > max.y()) {
				max.setY(color.y());
			} else if (color.y() < min.y()) {
				min.setY(color.y());
			}
			if (color.z() > max.z()) {
				max.setZ(color.z());
			} else if (color.z() < min.z()) {
				min.setZ(color.z());
			}
		}
	}
	QVector3D spread = max - min;
	for (int y = 0; y < result.size().height(); y++) {
		for (int x = 0; x < result.size().width(); x++) {
			int red = 255 / spread.x() * (colors.at(y).at(x).x() - min.x());
			int green = 255 / spread.y() * (colors.at(y).at(x).y() - min.y());
			int blue = 255 / spread.z() * (colors.at(y).at(x).z() - min.z());
			QColor color(red, green, blue);
			if (!color.isValid()) {
				qDebug() << "invalid color at position" << x << ", " << y <<
							", it is r:" << red << ", g:" << green << ", b:" <<
							blue << "raw:" << colors.at(y).at(x);
			} else {
				QVector3D v(red, green, blue);
				cp.setPixel(x, y, result, v);
			}
		}
	}
	qDebug() << "spread:" << spread << "max:" << max << "min:" << min;
	// parent's parent should be MainWindow
	return new PhotoWindow(result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}

QString ConvolutionFilter::name() const
{
	return "Convolution";
}
