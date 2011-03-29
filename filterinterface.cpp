#include "filterinterface.h"

FilterInterface::FilterInterface(QObject *parent) :
	QObject(parent),
	mUuid(QUuid::createUuid()),
	mFormat(QImage::Format_Invalid)
{
}

FilterInterface::~FilterInterface()
{
}

QUuid FilterInterface::uuid() const
{
	return mUuid;
}

bool FilterInterface::setup(const QImage &img)
{
	mImg = img;
	mFormat = img.format();
	return true;
}
