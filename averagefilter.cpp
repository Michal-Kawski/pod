#include "averagefilter.h"
#include "sizedialog.h"
#include "colorparser.h"
#include "photowindow.h"

#include <QVector3D>

AverageFilter::AverageFilter(QObject *parent) :
	FilterInterface(parent)
{
}

QString AverageFilter::name() const
{
	return "Average";
}

bool AverageFilter::setup(const QList<QImage> &img)
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
	return FilterInterface::setup(img);
}

DisplayWindow *AverageFilter::apply(QString windowBaseName)
{
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
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
					average += cp.pixel(pos, mImg);
					count++;
				}
			}
			average /= count;
			cp.setPixel(x, y, result, average);
		}
	}
	// parent's parent should be MainWindow
	return new PhotoWindow(result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
