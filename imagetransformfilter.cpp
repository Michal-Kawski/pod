#include "imagetransformfilter.h"

ImageTransformFilter::ImageTransformFilter(QObject *parent) :
	ImageFilter(parent)
{
}

bool ImageTransformFilter::setup(const FilterData &data)
{
	return ImageFilter::setup(data);
}

qreal ImageTransformFilter::extractColor(const QRgb &color, int which) const
{
	int c = ((color >> (8 * which)) & 0xff);
	return c / 255.0;
}
