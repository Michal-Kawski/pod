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
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QColor c = mImg.pixel(x, y);
			int r = 255 - c.red();
			int g = 255 - c.green();
			int b = 255 - c.blue();
			c = QColor(r, g, b);
			mImg.setPixel(x, y, c.rgba());
		}
	}
	return mImg;
}

QString NegativeFilter::name() const
{
	return "Negative";
}
