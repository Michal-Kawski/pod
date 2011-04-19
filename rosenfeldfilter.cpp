#include "rosenfeldfilter.h"
#include "slidingvaluedialog.h"
#include "colorparser.h"
#include "photowindow.h"

#include <QVector3D>

RosenfeldFilter::RosenfeldFilter(QObject *parent) :
	FilterInterface(parent)
{
}

QString RosenfeldFilter::name() const
{
	return "Rosenfeld";
}

bool RosenfeldFilter::setup(const QImage &img)
{
	{
		SlidingValueDialog svd(name(), 1, 1, img.width(),
							   qobject_cast<QWidget *>(this));
		if (svd.exec() != QDialog::Accepted) {
			return false;
		}
		mValue = svd.value();
	}
	return FilterInterface::setup(img);
}

DisplayWindow *RosenfeldFilter::apply(QString windowBaseName)
{
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QVector3D sumLeft, sumRight;
			for (int i = 1; i < mValue; i++) {
				QPoint pos(x + i - 1, y);
				// this point is outside of the image, so just skip it
				if (!mImg.rect().contains(pos)) {
					continue;
				}
				QVector3D v = cp.pixel(pos, mImg);
				sumRight += v;
			}
			for (int i = 1; i < mValue; i++) {
				QPoint pos(x - i, y);
				// this point is outside of the image, so just skip it
				if (!mImg.rect().contains(pos)) {
					continue;
				}
				QVector3D v = cp.pixel(pos, mImg);
				sumLeft += v;
			}
			QVector3D diff = sumRight - sumLeft;
			diff /= (qreal)mValue;
			cp.setPixel(x, y, result, diff);
		}
	}
	// parent's parent should be MainWindow
	return new PhotoWindow(result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
