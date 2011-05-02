#ifndef PHASESHIFTFILTER_H
#define PHASESHIFTFILTER_H

#include "transformfilter.h"

class PhaseShiftFilter : public TransformFilter
{
	Q_OBJECT
public:
	explicit PhaseShiftFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

private:
	qreal mK, mL;
	QImage::Format mFormat;
};

#endif // PHASESHIFTFILTER_H
