#include "negativefilter.h"

#include <QColor>

NegativeFilter::NegativeFilter(QObject *parent) :
	FilterInterface(parent)
{
}

bool NegativeFilter::setup(const QImage &img)
{
	mImg = img;
	return true;
}

QImage NegativeFilter::apply()
{
	QImage result(mImg.size(), QImage::Format_ARGB32);
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QColor c = mImg.pixel(x, y);
			int r = 255 - c.red();
			int g = 255 - c.green();
			int b = 255 - c.blue();
			c = QColor(r, g, b);
			result.setPixel(x, y, c.rgb());
		}
	}
	return result;
}

QString NegativeFilter::name() const
{
	return "Negative";
}
