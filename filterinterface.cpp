#include "filterinterface.h"

FilterInterface::FilterInterface(QObject *parent) :
	QObject(parent),
	mUuid(QUuid::createUuid())
{
}

FilterInterface::~FilterInterface()
{
}

QUuid FilterInterface::uuid() const
{
	return mUuid;
}
