#ifndef ROSENFELDFILTER_H
#define ROSENFELDFILTER_H

#include "imagefilter.h"

class RosenfeldFilter : public ImageFilter
{
	Q_OBJECT
public:
	explicit RosenfeldFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	int mValue;
};

#endif // ROSENFELDFILTER_H
