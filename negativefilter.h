#ifndef NEGATIVEFILTER_H
#define NEGATIVEFILTER_H

#include "imagefilter.h"

class NegativeFilter : public ImageFilter
{
    Q_OBJECT
public:
    explicit NegativeFilter(QObject *parent = 0);
	QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);
};

#endif // NEGATIVEFILTER_H
