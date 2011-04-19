#include "medianfilter.h"
#include "sizedialog.h"
#include "colorparser.h"
#include "photowindow.h"

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
	return FilterInterface::setup(img);
}

DisplayWindow *MedianFilter::apply(QString windowBaseName)
{
	QVector<int> red, green, blue;
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
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
					QVector3D v = cp.pixel(pos, mImg);
					red << v.x();
					green << v.y();
					blue << v.z();
				}
			}
			qSort(red);
			qSort(green);
			qSort(blue);
			QVector3D median;
			int count = red.count();
			if (count % 2 == 1) {
				median.setX(red.at(count / 2));
				median.setY(green.at(count / 2));
				median.setZ(blue.at(count / 2));
			} else {
				median.setX((red.at(count / 2 - 1) + red.at(count / 2)) / 2);
				median.setY((green.at(count / 2 - 1) + green.at(count / 2)) / 2);
				median.setZ((blue.at(count / 2 - 1) + blue.at(count / 2)) / 2);
			}
			cp.setPixel(x, y, result, median);
		}
	}
	// parent's parent should be MainWindow
	return new PhotoWindow(result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
