#include "contrastfilter.h"
#include "slidingvaluedialog.h"

#include <QVector3D>

#include <QDebug>

ContrastFilter::ContrastFilter(QObject *parent) :
	FilterInterface(parent)
{
}

QString ContrastFilter::name() const
{
	return "Contrast";
}

bool ContrastFilter::setup(const QImage &img)
{
	{
		SlidingValueDialog svd(name(), 0, -255, 255,
							   qobject_cast<QWidget *>(this));
		if (svd.exec() != QDialog::Accepted) {
			return false;
		}
		mValue = svd.value();
	}
	mImg = img;
	return true;
}

QImage ContrastFilter::apply()
{
	QImage result(mImg.size(), QImage::Format_ARGB32);
	float c = mValue / 255.0f + 1.0f;
	qDebug() << "contrast:" << c;
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QColor color(mImg.pixel(x, y));
			QVector3D colorVec(color.red(), color.green(), color.blue());
			colorVec *= c;
			color.setRed(qBound(0, (int)colorVec.x(), 255));
			color.setGreen(qBound(0, (int)colorVec.y(), 255));
			color.setBlue(qBound(0, (int)colorVec.z(), 255));
			result.setPixel(x, y, color.rgb());
		}
	}
	return result;
}
