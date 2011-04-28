#include "transformfilter.h"

TransformFilter::TransformFilter(QObject *parent) :
	FilterInterface(parent)
{
}

bool TransformFilter::setup(const QList<QImage> &img)
{
	return FilterInterface::setup(img);
}

qreal TransformFilter::extractColor(const QRgb &color, int which) const
{
	int c = ((color >> (8 * which)) & 0xff);
	return c / 255.0;
}
