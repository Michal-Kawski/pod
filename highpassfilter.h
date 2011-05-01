#ifndef HIGHPASSFILTER_H
#define HIGHPASSFILTER_H

#include "transformfilter.h"

class HighPassFilter : public TransformFilter
{
	Q_OBJECT
public:
	explicit HighPassFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

private:
	int mRadius;
	QImage::Format mFormat;
};

#endif // HIGHPASSFILTER_H
