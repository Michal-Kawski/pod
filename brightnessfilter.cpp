#include "brightnessfilter.h"

#include "slidingvaluedialog.h"

BrightnessFilter::BrightnessFilter(QObject *parent) :
    FilterInterface(parent)
{
}

bool BrightnessFilter::setup(const QImage &img)
{
	{
		SlidingValueDialog svd(name(), 0, -255, 255,
							   qobject_cast<QWidget *>(this));
		if (svd.exec() != QDialog::Accepted) {
			return false;
		}
		mValue = svd.value();
	}
	mImg = img;
	return true;
}

QString BrightnessFilter::name() const
{
	return "Brightness";
}

QImage BrightnessFilter::apply()
{
	QImage result(mImg.size(), QImage::Format_ARGB32);
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QColor color(mImg.pixel(x, y));
			color.setRed(qBound(0, color.red() + mValue, 255));
			color.setGreen(qBound(0, color.green() + mValue, 255));
			color.setBlue(qBound(0, color.blue() + mValue, 255));
			result.setPixel(x, y, color.rgb());
		}
	}
	return result;
}
