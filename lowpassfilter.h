#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include "transformfilter.h"

class LowPassFilter : public TransformFilter
{
	Q_OBJECT
public:
	explicit LowPassFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

private:
	int mRadius;
	QImage::Format mFormat;
};

#endif // LOWPASSFILTER_H
