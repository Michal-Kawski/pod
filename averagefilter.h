#ifndef AVERAGEFILTER_H
#define AVERAGEFILTER_H

#include "filterinterface.h"

class AverageFilter : public FilterInterface
{
	Q_OBJECT
public:
	explicit AverageFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const QImage &img);
	virtual QImage apply();

protected:
	QImage mImg;
	int mKernelWidth, mKernelHeight;
};

#endif // AVERAGEFILTER_H
