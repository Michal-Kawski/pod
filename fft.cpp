#include "fft.h"
#include "transformwindow.h"

// http://www.librow.com/articles/article-10

FFT::FFT(QObject *parent) :
	TransformFilter(parent),
	mCA(NULL)
{
}

FFT::~FFT()
{
	delete mCA;
}

bool FFT::setup(const QImage &img)
{
	if (TransformFilter::setup(img)) {
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
	perform();
	QImage result(mSize, mFormat);
	result.fill(Qt::black);
	for (unsigned int i = 0; i < mCA->shape()[0]; i++) {
		qreal min = 0;
		qreal max = 0;
		for (unsigned int j = 0; j < mCA->shape()[1]; j++) {
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				qreal phase = (*mCA)[i][j][k].phase();
				if (phase > max) {
					max = phase;
				} else if (phase < min) {
					min = phase;
				}
			}
		}

		for (unsigned int j = 0; j < mCA->shape()[1]; j++) {
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				unsigned int p = ((*mCA)[i][j][k].phase() - min) / (max - min) * 255.0;
				p <<= (8 * i);
				result.setPixel(j, k, result.pixel(j, k) | p);
			}
		}
	}
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	// parent's parent should be MainWindow
	return new TransformWindow(result, result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
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

void FFT::perform(bool inverse)
{
	Q_ASSERT(mCA->num_dimensions() == 3);
	for (unsigned int i = 0; i < mCA->shape()[0]; i++) {
		for (unsigned int j = 0; j < mCA->shape()[2]; j++) {
			QVector<Complex> elements;
			elements.reserve(mCA->shape()[1]);
			for (unsigned int k = 0; k < mCA->shape()[1]; k++) {
				elements << (*mCA)[i][k][j];
			}
			rearrange(elements);
			transform(elements, inverse);
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				(*mCA)[i][k][j] = elements.at(k);
			}
		}

		for (unsigned int j = 0; j < mCA->shape()[1]; j++) {
			QVector<Complex> elements;
			elements.reserve(mCA->shape()[2]);
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				elements << (*mCA)[i][j][k];
			}
			rearrange(elements);
			transform(elements, inverse);
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				(*mCA)[i][j][k] = elements.at(k);
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
