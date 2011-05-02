#ifndef BANDSTOPFILTER_H
#define BANDSTOPFILTER_H

#include "transformfilter.h"

class BandStopFilter : public TransformFilter
{
	Q_OBJECT
public:
	explicit BandStopFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

private:
	int mRadiusNear, mRadiusFar;
	QImage::Format mFormat;
};

#endif // BANDSTOPFILTER_H
