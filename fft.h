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
	virtual DisplayWindow *apply(QString windowBaseName);
	// QImage is QSharedData based, so as long we don't detach it, we don't
	// consume additional memory
	virtual bool setup(const QList<QImage> &img);

private:
	void rearrange(QVector<Complex> &elements);
	void perform(bool inverse = false);
	void transform(QVector<Complex> &elements, bool inverse);
	void rearrangeQuadrants(QImage &phase, QImage &magnitude) const;
	ComplexArray *mCA;
	QSize mSize;
};

#endif // FFT_H
