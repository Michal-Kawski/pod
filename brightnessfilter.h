#ifndef BRIGHTNESSFILTER_H
#define BRIGHTNESSFILTER_H

#include "filterinterface.h"

class BrightnessFilter : public FilterInterface
{
	Q_OBJECT
public:
	explicit BrightnessFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const QList<QImage> &img);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	int mValue;
};

#endif // BRIGHTNESSFILTER_H
