#ifndef BRIGHTNESSFILTER_H
#define BRIGHTNESSFILTER_H

#include "imagefilter.h"

class BrightnessFilter : public ImageFilter
{
	Q_OBJECT
public:
	explicit BrightnessFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	int mValue;
};

#endif // BRIGHTNESSFILTER_H
