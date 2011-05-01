#ifndef AVERAGEFILTER_H
#define AVERAGEFILTER_H

#include "imagefilter.h"

class AverageFilter : public ImageFilter
{
	Q_OBJECT
public:
	explicit AverageFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	int mKernelWidth, mKernelHeight;
};

#endif // AVERAGEFILTER_H
