#include "contrastfilter.h"
#include "slidingvaluedialog.h"
#include "colorparser.h"
#include "photowindow.h"

#include <QVector3D>
#include <cmath>

#include <QDebug>

ContrastFilter::ContrastFilter(QObject *parent) :
	ImageFilter(parent)
{
}

QString ContrastFilter::name() const
{
	return "Contrast";
}

bool ContrastFilter::setup(const FilterData &data)
{
	{
		SlidingValueDialog svd(name() + " tan(x); x * 100 =", M_PI_4 * 100, 0, M_PI_2 * 100,
							   qobject_cast<QWidget *>(this));
		if (svd.exec() != QDialog::Accepted) {
			return false;
		}
		mValue = svd.value();
	}
	return ImageFilter::setup(data);
}

DisplayWindow *ContrastFilter::apply(QString windowBaseName)
{
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
	float c = std::tan((float)mValue / 100.0f);
	qDebug() << "contrast:" << c;
	QVector<int> lut;
	lut.reserve(256);
	for (int i = 0; i < 256; i++) {
		lut << qBound(0, (int)(c * (i - 127) + 127), 255);
	}
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QVector3D colorVec = cp.pixel(x, y, mImg);
			colorVec.setX(lut.at((int)colorVec.x()));
			colorVec.setY(lut.at((int)colorVec.y()));
			colorVec.setZ(lut.at((int)colorVec.z()));
			cp.setPixel(x, y, result, colorVec);
		}
	}
	// parent's parent should be MainWindow
	return new PhotoWindow(result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
