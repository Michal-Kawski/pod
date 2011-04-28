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

bool FilterInterface::setup(const QList<QImage> &img)
{
	Q_ASSERT(img.length() > 0);
	mImg = img.at(0);
	mFormat = img.at(0).format();
	return true;
}
