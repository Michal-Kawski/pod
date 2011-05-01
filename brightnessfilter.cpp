#include "brightnessfilter.h"
#include "slidingvaluedialog.h"
#include "colorparser.h"
#include "photowindow.h"

BrightnessFilter::BrightnessFilter(QObject *parent) :
	ImageFilter(parent)
{
}

bool BrightnessFilter::setup(const FilterData &data)
{
	{
		SlidingValueDialog svd(name(), 0, -255, 255,
							   qobject_cast<QWidget *>(this));
		if (svd.exec() != QDialog::Accepted) {
			return false;
		}
		mValue = svd.value();
	}
	return ImageFilter::setup(data);
}

QString BrightnessFilter::name() const
{
	return "Brightness";
}

DisplayWindow *BrightnessFilter::apply(QString windowBaseName)
{
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QVector3D color = cp.pixel(x, y, mImg);
			color.setX(qBound(0, int(color.x()) + mValue, 255));
			color.setY(qBound(0, int(color.y()) + mValue, 255));
			color.setZ(qBound(0, int(color.z()) + mValue, 255));
			cp.setPixel(x, y, result, color);
		}
	}
	// parent's parent should be MainWindow
	return new PhotoWindow(result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
