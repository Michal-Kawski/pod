#include "phaseshiftfilter.h"
#include "multispinvaluedialog.h"
#include "transformwindow.h"

PhaseShiftFilter::PhaseShiftFilter(QObject *parent) :
	TransformFilter(parent)
{
}

QString PhaseShiftFilter::name() const
{
	return "Phase shift";
}

bool PhaseShiftFilter::setup(const FilterData &data)
{
	QStringList names;
	names << "k" << "l";
	MultiSpinValueDialog svd(names.count(), names, 0, 0, 1000,
							 qobject_cast<QWidget *>(this));
	if (svd.exec() != QDialog::Accepted) {
		return false;
	}
	mK = svd.values().at(0);
	mL = svd.values().at(1);
	mFormat = data.image.format();
	return TransformFilter::setup(data);
}

DisplayWindow *PhaseShiftFilter::apply(QString windowBaseName)
{
	int layers = mCA->shape()[0];
	int w = mCA->shape()[1];
	int h = mCA->shape()[2];

	qreal k = -mK * 2 * M_PI / w;
	qreal l = -mL * 2 * M_PI / h;
	qreal sum = (mK + mL) * M_PI;

	rearrangeQuadrants();
	for (int i = 0; i < layers; i++) {
		for (int p = 0; p < w; p++) {
			for (int q = 0; q < h; q++) {
				Complex c = (*mCA)[i][p][q];
				c *= Complex::fromPowerPhase(1.0, k * p + l * q + sum);
				(*mCA)[i][p][q] = c;
			}
		}
	}
	rearrangeQuadrants();

	ComplexArray *ca = new ComplexArray(boost::extents[layers][w][h]);
	*ca = *mCA;
	return new TransformWindow(ca, mFormat, windowBaseName, q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
