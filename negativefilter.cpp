#include "negativefilter.h"
#include "colorparser.h"
#include "photowindow.h"

#include <QColor>

NegativeFilter::NegativeFilter(QObject *parent) :
	ImageFilter(parent)
{
}

bool NegativeFilter::setup(const FilterData &data)
{
	return ImageFilter::setup(data);
}

DisplayWindow *NegativeFilter::apply(QString windowBaseName)
{
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QVector3D v = cp.pixel(x, y, mImg);
			int r = 255 - v.x();
			int g = 255 - v.y();
			int b = 255 - v.z();
			v = QVector3D(r, g, b);
			cp.setPixel(x, y, result, v);
		}
	}
	// parent's parent should be MainWindow
	return new PhotoWindow(result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}

QString NegativeFilter::name() const
{
	return "Negative";
}
