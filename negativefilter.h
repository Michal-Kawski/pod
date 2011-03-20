#ifndef NEGATIVEFILTER_H
#define NEGATIVEFILTER_H

#include "filterinterface.h"

#include <QImage>

class NegativeFilter : public FilterInterface
{
    Q_OBJECT
public:
    explicit NegativeFilter(QObject *parent = 0);
	QString name() const;

signals:

public slots:
	bool setup(const QImage &img);
	QImage apply();

protected:
	QImage mImg;
};

#endif // NEGATIVEFILTER_H
