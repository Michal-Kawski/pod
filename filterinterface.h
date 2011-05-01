#ifndef FILTERINTERFACE_H
#define FILTERINTERFACE_H

#include "filterdata.h"

#include <QObject>
#include <QUuid>

class DisplayWindow;

class FilterInterface : public QObject
{
    Q_OBJECT
public:
	explicit FilterInterface(QObject *parent);
	virtual ~FilterInterface();					// dummy
	virtual QString name() const = 0;
	QUuid uuid() const;

private:
	QUuid mUuid;

signals:

public slots:
	virtual bool setup(const FilterData &data) = 0;
	virtual DisplayWindow *apply(QString windowBaseName) = 0;
};

#endif // FILTERINTERFACE_H
