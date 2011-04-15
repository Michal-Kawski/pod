#ifndef FFT_H
#define FFT_H

#include "transformfilter.h"
#include "complexarray.h"

class FFT : public TransformFilter
{
	Q_OBJECT
public:
	explicit FFT(QObject *parent = 0);
	~FFT();
	virtual QString name() const;

signals:

public slots:
	virtual QImage apply();
	virtual bool setup(const QImage &img);

private:
	void rearrange(QVector<Complex> &elements);
	void perform(bool inverse = false);
	void transform(QVector<Complex> &elements, bool inverse);
	ComplexArray *mCA;
	QSize mSize;
};

#endif // FFT_H
