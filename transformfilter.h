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
	virtual QString name() const = 0;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName) = 0;

protected:
	ComplexArray *mCA;
	void rearrangeQuadrants();
};

#endif // TRANSFORMFILTER_H
