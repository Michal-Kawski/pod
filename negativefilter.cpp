#include "negativefilter.h"
#include "colorparser.h"

#include <QColor>

NegativeFilter::NegativeFilter(QObject *parent) :
	FilterInterface(parent)
{
}

bool NegativeFilter::setup(const QImage &img)
{
	return FilterInterface::setup(img);
}

QImage NegativeFilter::apply()
{
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QVector3D v = cp.pixel(x, y, mImg);
			int r = 255 - v.x();
			int g = 255 - v.y();
			int b = 255 - v.z();
			v = QVector3D(r, g, b);
			cp.setPixel(x, y, result, v);
		}
	}
	return result;
}

QString NegativeFilter::name() const
{
	return "Negative";
}
