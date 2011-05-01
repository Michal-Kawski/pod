#ifndef FILTERDATA_H
#define FILTERDATA_H

#include "complexarray.h"

#include <QImage>

// this class is POD

class FilterData
{
public:
	FilterData();
	QImage image;
	ComplexArray transformData;
};

#endif // FILTERDATA_H
