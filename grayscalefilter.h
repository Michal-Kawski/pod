#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "filterinterface.h"

class GrayScaleFilter : public FilterInterface
{
	Q_OBJECT
public:
	explicit GrayScaleFilter(QWidget *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const QImage &img);
	virtual QImage apply();

protected:
	enum GrayScaleType {Gray, Proportional};
	QImage mImg;
	GrayScaleFilter::GrayScaleType mFilterType;
};

#endif // GRAYSCALEFILTER_H
