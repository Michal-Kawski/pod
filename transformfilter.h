#ifndef TRANSFORMFILTER_H
#define TRANSFORMFILTER_H

#include "filterinterface.h"
#include "complexarray.h"

class TransformFilter : public FilterInterface
{
	Q_OBJECT
public:
	explicit TransformFilter(QObject *parent = 0);
	~TransformFilter();

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName) = 0;

private:
	ComplexArray *mCA;
};

#endif // TRANSFORMFILTER_H
