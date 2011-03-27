#include "medianfilter.h"
#include "sizedialog.h"

MedianFilter::MedianFilter(QObject *parent) :
    FilterInterface(parent)
{
}

QString MedianFilter::name() const
{
	return "Median";
}

bool MedianFilter::setup(const QImage &img)
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

QImage MedianFilter::apply()
{
	QVector<int> red, green, blue;
	QImage result(mImg.size(), QImage::Format_ARGB32);
	red.reserve(((mKernelWidth * 2) + 1) * ((mKernelHeight * 2) + 1));
	green.reserve(((mKernelWidth * 2) + 1) * ((mKernelHeight * 2) + 1));
	blue.reserve(((mKernelWidth * 2) + 1) * ((mKernelHeight * 2) + 1));
	for (int x = 0; x < mImg.size().width(); x++) {
		for (int y = 0; y < mImg.size().height(); y++) {
			red.resize(0);
			green.resize(0);
			blue.resize(0);
			for (int j = -mKernelHeight; j < mKernelHeight + 1; j++) {
				for (int i = -mKernelWidth; i < mKernelWidth + 1; i++) {
					QPoint pos(x + i, y + j);
					// this point is outside of the image, so just skip it
					if (!mImg.rect().contains(pos)) {
						continue;
					}
					QColor rgbColor = QColor(mImg.pixel(pos));
					red << rgbColor.red();
					green << rgbColor.green();
					blue << rgbColor.blue();
				}
			}
			qSort(red);
			qSort(green);
			qSort(blue);
			QColor median;
			int count = red.count();
			if (count % 2 == 1) {
				median.setRed(red.at(count / 2));
				median.setGreen(green.at(count / 2));
				median.setBlue(blue.at(count / 2));
			} else {
				median.setRed((red.at(count / 2 - 1) + red.at(count / 2)) / 2);
				median.setGreen((green.at(count / 2 - 1) + green.at(count / 2)) / 2);
				median.setBlue((blue.at(count / 2 - 1) + blue.at(count / 2)) / 2);
			}
			result.setPixel(x, y, median.rgb());
		}
	}
	return result;
}
