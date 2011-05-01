#include "lowpassfilter.h"
#include "slidingvaluedialog.h"
#include "transformwindow.h"

LowPassFilter::LowPassFilter(QObject *parent) :
	TransformFilter(parent)
{
}

QString LowPassFilter::name() const
{
	return "Low pass filter";
}

bool LowPassFilter::setup(const FilterData &data)
{
	int radiusMax = data.transformData.shape()[1] / 2;
	SlidingValueDialog svd(name() + ", x: ", 0, 0, radiusMax,
						   qobject_cast<QWidget *>(this));
	if (svd.exec() != QDialog::Accepted) {
		return false;
	}
	mRadius = svd.value() * svd.value();
	mFormat = data.image.format();
	return TransformFilter::setup(data);
}

DisplayWindow *LowPassFilter::apply(QString windowBaseName)
{
	Q_ASSERT(mCA->dimensionality == 3);
	QVector2D center(mCA->shape()[1] / 2, mCA->shape()[2] / 2);
	rearrangeQuadrants();
	for (unsigned int i = 0; i < mCA->shape()[0]; i++) {
		for (unsigned int j = 0; j < mCA->shape()[1]; j++) {
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				QVector2D point(j, k);
				if ((point - center).lengthSquared() >= mRadius) {
					(*mCA)[i][j][k] = Complex();
				}
			}
		}
	}
	rearrangeQuadrants();
	int layers = mCA->shape()[0];
	int w = mCA->shape()[1];
	int h = mCA->shape()[2];
	ComplexArray *ca = new ComplexArray(boost::extents[layers][w][h]);
	*ca = *mCA;
	return new TransformWindow(ca, mFormat, windowBaseName, q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
