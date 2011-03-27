#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include "filterinterface.h"

class MedianFilter : public FilterInterface
{
    Q_OBJECT
public:
    explicit MedianFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const QImage &img);
	virtual QImage apply();

protected:
	QImage mImg;
	int mKernelWidth, mKernelHeight;
};

#endif // MEDIANFILTER_H
