#ifndef ROSENFELDFILTER_H
#define ROSENFELDFILTER_H

#include "filterinterface.h"

class RosenfeldFilter : public FilterInterface
{
	Q_OBJECT
public:
	explicit RosenfeldFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const QImage &img);
	virtual QImage apply();

protected:
	QImage mImg;
	int mValue;
};

#endif // ROSENFELDFILTER_H
