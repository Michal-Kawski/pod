#include "rosenfeldfilter.h"
#include "slidingvaluedialog.h"

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
	mImg = img;
	return true;
}

QImage RosenfeldFilter::apply()
{
	QImage result(mImg.size(), QImage::Format_ARGB32);
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QVector3D sumLeft, sumRight;
			for (int i = 1; i < mValue; i++) {
				QPoint pos(x + i - 1, y);
				// this point is outside of the image, so just skip it
				if (!mImg.rect().contains(pos)) {
					continue;
				}
				QRgb rgb = mImg.pixel(pos);
				sumRight += QVector3D(qRed(rgb), qGreen(rgb), qBlue(rgb));
			}
			for (int i = 1; i < mValue; i++) {
				QPoint pos(x - i, y);
				// this point is outside of the image, so just skip it
				if (!mImg.rect().contains(pos)) {
					continue;
				}
				QRgb rgb = mImg.pixel(pos);
				sumLeft += QVector3D(qRed(rgb), qGreen(rgb), qBlue(rgb));
			}
			QVector3D diff = sumRight - sumLeft;
			diff /= (qreal)mValue;
			QRgb rgb = qRgb(diff.x(), diff.y(), diff.z());
			result.setPixel(x, y, rgb);
		}
	}
	return result;
}
