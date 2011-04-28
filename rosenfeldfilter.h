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
	virtual bool setup(const QList<QImage> &img);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	int mValue;
};

#endif // ROSENFELDFILTER_H
