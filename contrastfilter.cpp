#include "contrastfilter.h"
#include "slidingvaluedialog.h"
#include "colorparser.h"
#include "photowindow.h"

#include <QVector3D>

#include <QDebug>

ContrastFilter::ContrastFilter(QObject *parent) :
	FilterInterface(parent)
{
}

QString ContrastFilter::name() const
{
	return "Contrast";
}

bool ContrastFilter::setup(const QList<QImage> &img)
{
	{
		SlidingValueDialog svd(name(), 0, -255, 255,
							   qobject_cast<QWidget *>(this));
		if (svd.exec() != QDialog::Accepted) {
			return false;
		}
		mValue = svd.value();
	}
	return FilterInterface::setup(img);
}

DisplayWindow *ContrastFilter::apply(QString windowBaseName)
{
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
	float c = mValue / 255.0f + 1.0f;
	qDebug() << "contrast:" << c;
	for (int x = 0; x < mImg.width(); x++) {
		for (int y = 0; y < mImg.height(); y++) {
			QVector3D colorVec = cp.pixel(x, y, mImg);
			colorVec *= c;
			colorVec.setX(qBound(0, (int)colorVec.x(), 255));
			colorVec.setY(qBound(0, (int)colorVec.y(), 255));
			colorVec.setZ(qBound(0, (int)colorVec.z(), 255));
			cp.setPixel(x, y, result, colorVec);
		}
	}
	// parent's parent should be MainWindow
	return new PhotoWindow(result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
