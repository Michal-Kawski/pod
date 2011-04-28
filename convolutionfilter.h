#ifndef CONVOLUTIONFILTER_H
#define CONVOLUTIONFILTER_H

#include "filterinterface.h"

class ConvolutionFilter : public FilterInterface
{
    Q_OBJECT
public:
    explicit ConvolutionFilter(QObject *parent = 0);
	virtual QString name() const;

signals:

public slots:
	virtual bool setup(const QList<QImage> &img);
	virtual DisplayWindow *apply(QString windowBaseName);

protected:
	QVector<QVector<int> > mKernel;
	// this is really a radius
	int mKernelWidth, mKernelHeight;
};

#endif // CONVOLUTIONFILTER_H
