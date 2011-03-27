#include "averagefilter.h"
#include "sizedialog.h"

#include <QVector3D>

AverageFilter::AverageFilter(QObject *parent) :
	FilterInterface(parent)
{
}

QString AverageFilter::name() const
{
	return "Average";
}

bool AverageFilter::setup(const QImage &img)
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
	mImg = img;
	return true;
}

QImage AverageFilter::apply()
{
	QImage result(mImg.size(), QImage::Format_ARGB32);
	for (int x = 0; x < mImg.size().width(); x++) {
		for (int y = 0; y < mImg.size().height(); y++) {
			int count = 0;
			QVector3D average;
			for (int j = -mKernelHeight; j < mKernelHeight + 1; j++) {
				for (int i = -mKernelWidth; i < mKernelWidth + 1; i++) {
					QPoint pos(x + i, y + j);
					// this point is outside of the image, so just skip it
					if (!mImg.rect().contains(pos)) {
						continue;
					}
					QColor color(mImg.pixel(pos));
					average += QVector3D(color.red(), color.green(), color.blue());
					count++;
				}
			}
			average /= count;
			QColor color(average.x(), average.y(), average.z());
			result.setPixel(x, y, color.rgb());
		}
	}
	return result;
}
