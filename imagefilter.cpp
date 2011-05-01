#include "imagefilter.h"

ImageFilter::ImageFilter(QObject *parent) :
	FilterInterface(parent),
	mFormat(QImage::Format_Invalid)
{
}

bool ImageFilter::setup(const FilterData &data)
{
	Q_ASSERT(!data.image.isNull());
	mImg = data.image;
	mFormat = data.image.format();
	return true;
}
