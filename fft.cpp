#include "fft.h"
#include "transformwindow.h"
#include "colorparser.h"

#include "QDebug"

// http://www.librow.com/articles/article-10

FFT::FFT(QObject *parent) :
	ImageTransformFilter(parent),
	mCA(NULL)
{
}

FFT::~FFT()
{
	delete mCA;
}

bool FFT::setup(const FilterData &data)
{
	if (ImageTransformFilter::setup(data)) {
		if (mFormat == QImage::Format_ARGB32 || mFormat == QImage::Format_ARGB32_Premultiplied) {
			qWarning() << "transparancy in the image is discarded";
			mImg.convertToFormat(QImage::Format_RGB32);
		}
		mSize = QSize(1, 1);
		while (mSize.width() < mImg.width() || mSize.height() < mImg.height()) {
			mSize *= 2;
		}

		int layers;
		switch (mFormat) {
			case QImage::Format_Indexed8:
				layers = 1;
				break;
			case QImage::Format_ARGB32:
			case QImage::Format_RGB32:
				layers = 3;
				break;
			default:
				return false;
		}

		// the array created here might be bigger than image size - it has to be
		// a square of side length 2^n
		delete mCA;
		int w = mSize.width();
		int h = mSize.height();
		mCA = new ComplexArray(boost::extents[layers][w][h]);

		// fill only the data that exists in the image
		for (int i = 0; i < layers; i++) {
			for (int y = 0; y < mImg.height(); y++) {
				for (int x = 0; x < mImg.width(); x++) {
					(*mCA)[i][y][x] = Complex(extractColor(mImg.pixel(x, y), i), 0);
				}
			}
		}

		return true;
	} else {
		return false;
	}
}

QString FFT::name() const
{
	return "FFT";
}

DisplayWindow *FFT::apply(QString windowBaseName)
{
	perform(mCA);
	int layers = mCA->shape()[0];
	int w = mSize.width();
	int h = mSize.height();
	ComplexArray *ca = new ComplexArray(boost::extents[layers][w][h]);
	*ca = *mCA;
	// parent's parent should be MainWindow
	return new TransformWindow(ca, mFormat, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}

void FFT::rearrange(QVector<Complex> &elements)
{
	int target = 0;
	for (int position = 0; position < elements.count(); position++) {
		if (target > position) {
			Complex temp = elements.at(position);
			elements[position] = elements.at(target);
			elements[target] = temp;
		}

		unsigned int mask = elements.count();

		while (target & (mask >>= 1)) {
			target &= ~mask;
		}

		target |= mask;
	}
}

void FFT::perform(ComplexArray *ca, bool inverse)
{
	Q_ASSERT(ca->num_dimensions() == 3);
	for (unsigned int i = 0; i < ca->shape()[0]; i++) {
		for (unsigned int j = 0; j < ca->shape()[2]; j++) {
			QVector<Complex> elements;
			elements.reserve(ca->shape()[1]);
			for (unsigned int k = 0; k < ca->shape()[1]; k++) {
				elements << (*ca)[i][k][j];
			}
			rearrange(elements);
			transform(elements, inverse);
			for (unsigned int k = 0; k < ca->shape()[1]; k++) {
				(*ca)[i][k][j] = elements.at(k);
			}
		}

		for (unsigned int j = 0; j < ca->shape()[1]; j++) {
			QVector<Complex> elements;
			elements.reserve(ca->shape()[2]);
			for (unsigned int k = 0; k < ca->shape()[2]; k++) {
				elements << (*ca)[i][j][k];
			}
			rearrange(elements);
			transform(elements, inverse);
			for (unsigned int k = 0; k < ca->shape()[2]; k++) {
				(*ca)[i][j][k] = elements.at(k);
			}
		}
	}
}

void FFT::transform(QVector<Complex> &elements, bool inverse)
{
	const unsigned int N = elements.count();
	const qreal pi = inverse ? M_PI : -M_PI;
	for (unsigned int step = 1; step < N; step <<= 1) {
		const unsigned int jump = step << 1;
		const qreal delta = pi / qreal(step);
		const qreal sine = sin(delta * .5);
		const Complex multiplier(-2. * sine * sine, sin(delta));
		Complex factor(1, 0);

		for (unsigned int group = 0; group < step; ++group) {
			for (unsigned int pair = group; pair < N; pair += jump) {
				const unsigned int match = pair + step;
				const Complex product(factor * elements.at(match));
				elements[match] = elements.at(pair) - product;
				elements[pair] += product;
			}
			factor = multiplier * factor + factor;
		}
	}
}
