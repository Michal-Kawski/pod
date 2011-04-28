#include "fft.h"
#include "transformwindow.h"
#include "colorparser.h"

#include "QDebug"

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

bool FFT::setup(const QList<QImage> &img)
{
	if (TransformFilter::setup(img)) {
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
	perform();
	QImage resultPhase(mSize, mFormat);
	QImage resultMagnitude(mSize, mFormat);
	if (mFormat == QImage::Format_Indexed8) {
		QVector<QRgb> colors;
		colors.reserve(256);
		for (int i = 0; i < 256; i++) {
			colors << qRgb(i, i, i);
		}
		resultPhase.setColorTable(colors);
		resultMagnitude.setColorTable(colors);
		resultPhase.fill(0);
		resultMagnitude.fill(0);
	} else {
		resultPhase.fill(Qt::black);
		resultMagnitude.fill(Qt::black);
	}
	for (unsigned int i = 0; i < mCA->shape()[0]; i++) {
		qreal minp = 0;
		qreal maxp = 0;
		qreal minm = 0;
		qreal maxm = 0;
		for (unsigned int j = 0; j < mCA->shape()[1]; j++) {
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				qreal phase = (*mCA)[i][j][k].phase();
				qreal magnitude = (*mCA)[i][j][k].abs();
				if (phase > maxp) {
					maxp = phase;
				} else if (phase < minp) {
					minp = phase;
				}
				if (magnitude > maxm) {
					maxm = magnitude;
				} else if (magnitude < minm) {
					minm = magnitude;
				}
			}
		}

		ColorParser cp(mFormat);
		if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
			resultPhase.setColorTable(mImg.colorTable());
			resultMagnitude.setColorTable(mImg.colorTable());
		}
		for (unsigned int j = 0; j < mCA->shape()[1]; j++) {
			for (unsigned int k = 0; k < mCA->shape()[2]; k++) {
				qreal p = ((*mCA)[i][j][k].phase() - minp) / (maxp - minp) * 255.0;
				{
					QVector3D oldPixel = cp.pixel(k, j, resultPhase);
					QVector3D newPixel;
					switch (i) {
						case 0:
							newPixel.setX(p);
							break;
						case 1:
							newPixel.setY(p);
							break;
						case 2:
							newPixel.setZ(p);
							break;
						default:
							break;
					}
					cp.setPixel(k, j, resultPhase, cp.merge(oldPixel, newPixel));
				}

				// logarithmic scale
				// implementaion: http://homepages.inf.ed.ac.uk/rbf/HIPR2/pixlog.htm
				// idea: http://homepages.inf.ed.ac.uk/rbf/HIPR2/fourier.htm#guidelines
				p = (*mCA)[i][j][k].abs();
				{
					qreal c = 255.0 / log(1.0 + abs(maxm - minm));
					p = c * log(1.0 + p);
					QVector3D oldPixel = cp.pixel(k, j, resultMagnitude);
					QVector3D newPixel;
					switch (i) {
						case 0:
							newPixel.setX(p);
							break;
						case 1:
							newPixel.setY(p);
							break;
						case 2:
							newPixel.setZ(p);
							break;
						default:
							break;
					}
					cp.setPixel(k, j, resultMagnitude, cp.merge(oldPixel, newPixel));
				}
			}
		}
	}
	rearrangeQuadrants(resultPhase, resultMagnitude);
	// parent's parent should be MainWindow
	return new TransformWindow(resultMagnitude, resultPhase, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
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
			for (unsigned int k = 0; k < mCA->shape()[1]; k++) {
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

void FFT::rearrangeQuadrants(QImage &phase, QImage &magnitude) const
{
	// http://paulbourke.net/miscellaneous/imagefilter/
	int hw = phase.width() / 2;
	int hh = phase.height() / 2;
	if (phase.format() != QImage::Format_Indexed8) {
		for (int i = 0; i < hw; i++) {
			for (int j = 0; j < hh; j++) {
				QRgb tempColor = phase.pixel(i, j);
				phase.setPixel(i, j, phase.pixel(i + hw, j + hh));
				phase.setPixel(i + hw, j + hh, tempColor);

				tempColor = phase.pixel(i + hw, j);
				phase.setPixel(i + hw, j, phase.pixel(i, j + hh));
				phase.setPixel(i, j + hh, tempColor);


				tempColor = magnitude.pixel(i, j);
				magnitude.setPixel(i, j, magnitude.pixel(i + hw, j + hh));
				magnitude.setPixel(i + hw, j + hh, tempColor);

				tempColor = magnitude.pixel(i + hw, j);
				magnitude.setPixel(i + hw, j, magnitude.pixel(i, j + hh));
				magnitude.setPixel(i, j + hh, tempColor);
			}
		}
	} else {
		for (int i = 0; i < hw; i++) {
			for (int j = 0; j < hh; j++) {
				int tempColor = phase.pixelIndex(i, j);
				phase.setPixel(i, j, phase.pixelIndex(i + hw, j + hh));
				phase.setPixel(i + hw, j + hh, tempColor);

				tempColor = phase.pixelIndex(i + hw, j);
				phase.setPixel(i + hw, j, phase.pixelIndex(i, j + hh));
				phase.setPixel(i, j + hh, tempColor);


				tempColor = magnitude.pixelIndex(i, j);
				magnitude.setPixel(i, j, magnitude.pixelIndex(i + hw, j + hh));
				magnitude.setPixel(i + hw, j + hh, tempColor);

				tempColor = magnitude.pixelIndex(i + hw, j);
				magnitude.setPixel(i + hw, j, magnitude.pixelIndex(i, j + hh));
				magnitude.setPixel(i, j + hh, tempColor);
			}
		}
	}
}
