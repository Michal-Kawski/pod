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

void TransformFilter::rearrangeQuadrants()
{
	// http://paulbourke.net/miscellaneous/imagefilter/
	int hw = mCA->shape()[1] / 2;
	int hh = mCA->shape()[2] / 2;
	for (unsigned int i = 0; i < mCA->shape()[0]; i++) {
		for (int j = 0; j < hw; j++) {
			for (int k = 0; k < hh; k++) {
				Complex temp = (*mCA)[i][j][k];
				(*mCA)[i][j][k] = (*mCA)[i][j + hw][k + hh];
				(*mCA)[i][j + hw][k + hh] = temp;

				temp = (*mCA)[i][j + hw][k];
				(*mCA)[i][j + hw][k] = (*mCA)[i][j][k + hh];
				(*mCA)[i][j][k + hh] = temp;
			}
		}
	}
}
