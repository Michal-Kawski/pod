#ifndef TRANSFORMFILTER_H
#define TRANSFORMFILTER_H

#include "filterinterface.h"

class TransformFilter : public FilterInterface
{
	Q_OBJECT
public:
	explicit TransformFilter(QObject *parent);
	virtual QString name() const = 0;

signals:

public slots:
	virtual bool setup(const QImage &img);
	virtual QImage apply() = 0;
};

#endif // TRANSFORMFILTER_H
