#include "transformfilter.h"

TransformFilter::TransformFilter(QObject *parent) :
	FilterInterface(parent)
{
}

bool TransformFilter::setup(const QImage &img)
{
	return FilterInterface::setup(img);
}
