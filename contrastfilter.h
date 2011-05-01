#ifndef CONTRASTFILTER_H
#define CONTRASTFILTER_H

#include "imagefilter.h"

class ContrastFilter : public ImageFilter
{
    Q_OBJECT
public:
	explicit ContrastFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	int mValue;
};

#endif // CONTRASTFILTER_H
