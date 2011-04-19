#ifndef FILTERINTERFACE_H
#define FILTERINTERFACE_H

#include <QObject>
#include <QImage>
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
	virtual bool setup(const QImage &img);
	virtual DisplayWindow *apply(QString windowBaseName) = 0;

protected:
	QImage mImg;
	QImage::Format mFormat;
};

#endif // FILTERINTERFACE_H
