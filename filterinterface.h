#ifndef FILTERINTERFACE_H
#define FILTERINTERFACE_H

#include <QObject>
#include <QImage>
#include <QUuid>

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
	virtual bool setup(const QImage &img) = 0;
	virtual QImage apply() = 0;
};

#endif // FILTERINTERFACE_H
