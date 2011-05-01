#include "transformfilter.h"

TransformFilter::TransformFilter(QObject *parent) :
	ImageFilter(parent)
{
}

bool TransformFilter::setup(const FilterData &data)
{
	return ImageFilter::setup(data);
}

qreal TransformFilter::extractColor(const QRgb &color, int which) const
{
	int c = ((color >> (8 * which)) & 0xff);
	return c / 255.0;
}
