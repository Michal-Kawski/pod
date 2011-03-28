#include "qualitychecker.h"

#include <QImage>
#include <cmath>
#include <QRgb>
#include <QVector3D>

QualityChecker::QualityChecker()
{
}

bool QualityChecker::operator() (const QImage &ref, const QImage &img)
{
	if (ref.size() != img.size()) {
		return false;
	}
	mse = 0;
	snr = 0;
	for (int y = 0; y < img.height(); y++) {
		for (int x = 0; x < img.width(); x++) {
			QRgb refC = ref.pixel(x, y);
			QRgb imgC = img.pixel(x, y);
			QVector3D refCV(qRed(refC), qGreen(refC), qBlue(refC));
			QVector3D imgCV(qRed(imgC), qGreen(imgC), qBlue(imgC));
			mse += pow(refCV.x() - imgCV.x(), 2);
			mse += pow(refCV.y() - imgCV.y(), 2);
			mse += pow(refCV.z() - imgCV.z(), 2);
			snr += pow(refCV.x(), 2);
			snr += pow(refCV.y(), 2);
			snr += pow(refCV.z(), 2);
		}
	}
	mse /= (double)(img.height() * img.width());
	if (mse == 0) {
		snr = 0;
	} else {
		snr = 10 * log10(snr / mse);
	}
	return true;
}
