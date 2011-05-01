#ifndef IMAGETRANSFORMFILTER_H
#define IMAGETRANSFORMFILTER_H

#include "imagefilter.h"

class ImageTransformFilter : public ImageFilter
{
	Q_OBJECT
public:
	explicit ImageTransformFilter(QObject *parent);
	virtual QString name() const = 0;

signals:

public slots:
	virtual bool setup(const FilterData &data);
	virtual DisplayWindow *apply(QString windowBaseName) = 0;

protected:
	qreal extractColor(const QRgb &color, int which) const;
};

#endif // IMAGETRANSFORMFILTER_H
