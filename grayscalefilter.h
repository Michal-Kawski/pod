#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "imagefilter.h"

class GrayScaleFilter : public ImageFilter
{
	Q_OBJECT
public:
	explicit GrayScaleFilter(QWidget *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	enum GrayScaleType {Min, Proportional};
	GrayScaleFilter::GrayScaleType mFilterType;
};

#endif // GRAYSCALEFILTER_H
