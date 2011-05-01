#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include "imagefilter.h"

class MedianFilter : public ImageFilter
{
    Q_OBJECT
public:
    explicit MedianFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	int mKernelWidth, mKernelHeight;
};

#endif // MEDIANFILTER_H
