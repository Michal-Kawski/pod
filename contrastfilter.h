#ifndef CONTRASTFILTER_H
#define CONTRASTFILTER_H

#include "filterinterface.h"

class ContrastFilter : public FilterInterface
{
    Q_OBJECT
public:
	explicit ContrastFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const QImage &img);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	int mValue;
};

#endif // CONTRASTFILTER_H
