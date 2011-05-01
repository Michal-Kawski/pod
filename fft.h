#ifndef FFT_H
#define FFT_H

#include "imagetransformfilter.h"
#include "complexarray.h"

class FFT : public ImageTransformFilter
{
	Q_OBJECT
public:
	explicit FFT(QObject *parent = 0);
	~FFT();
	virtual QString name() const;
	void perform(ComplexArray *ca, bool inverse = false);

signals:

public slots:
	virtual DisplayWindow *apply(QString windowBaseName);
	virtual bool setup(const FilterData &data);

private:
	void rearrange(QVector<Complex> &elements);
	void transform(QVector<Complex> &elements, bool inverse);
	void rearrangeQuadrants(QImage &phase, QImage &magnitude) const;
	ComplexArray *mCA;
	QSize mSize;
};

#endif // FFT_H
