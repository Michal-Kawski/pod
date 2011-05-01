#include "transformfilter.h"

TransformFilter::TransformFilter(QObject *parent) :
	FilterInterface(parent),
	mCA(NULL)
{
}

TransformFilter::~TransformFilter()
{
	delete mCA;
}

bool TransformFilter::setup(const FilterData &data)
{
	delete mCA;
	int layers = data.transformData.shape()[0];
	int w = data.transformData.shape()[1];
	int h = data.transformData.shape()[2];
	mCA = new ComplexArray(boost::extents[layers][w][h]);
	*mCA = data.transformData;
	return true;
}
