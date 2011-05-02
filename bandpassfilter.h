#ifndef BANDPASSFILTER_H
#define BANDPASSFILTER_H

#include "transformfilter.h"

class BandPassFilter : public TransformFilter
{
	Q_OBJECT
public:
	explicit BandPassFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

private:
	int mRadiusNear, mRadiusFar;
	QImage::Format mFormat;
};

#endif // BANDPASSFILTER_H
