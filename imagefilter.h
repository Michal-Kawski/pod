#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include "filterinterface.h"

#include <QImage>

class ImageFilter : public FilterInterface
{
	Q_OBJECT
public:
	explicit ImageFilter(QObject *parent = 0);

signals:

public slots:
	virtual bool setup(const FilterData &data);

protected:
	QImage mImg;
	QImage::Format mFormat;
};

#endif // IMAGEFILTER_H
