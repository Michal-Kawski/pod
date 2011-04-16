#include "grayscalefilter.h"
#include "colorparser.h"
#include "photowindow.h"

#include <QButtonGroup>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>

GrayScaleFilter::GrayScaleFilter(QWidget *parent) :
	FilterInterface(parent)
{
}

QString GrayScaleFilter::name() const
{
	return "Grayscale";
}

bool GrayScaleFilter::setup(const QImage &img)
{
	{
		QDialog typeDialog(qobject_cast<QWidget *>(this));
		QVBoxLayout vbl(qobject_cast<QWidget *>(&typeDialog));
		QButtonGroup bg(qobject_cast<QWidget *>(&typeDialog));
		bg.addButton(new QRadioButton("min(r, g, b)",
									  qobject_cast<QWidget *>(&typeDialog)),
					 Min);
		bg.addButton(new QRadioButton("r: .299, g: .587, b: .114",
									  qobject_cast<QWidget *>(&typeDialog)),
					 Proportional);
		QDialogButtonBox dbb(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, qobject_cast<QWidget *>(&typeDialog));
		connect(&dbb, SIGNAL(accepted()), &typeDialog, SLOT(accept()));
		connect(&dbb, SIGNAL(rejected()), &typeDialog, SLOT(reject()));
		foreach (QAbstractButton *button, bg.buttons()) {
			vbl.addWidget(button);
		}
		vbl.addWidget(&dbb);
		if (typeDialog.exec() != QDialog::Accepted) {
			return false;
		}
		if (bg.checkedId() == -1) {
			QMessageBox::warning(qobject_cast<QWidget *>(this), "Error",
								 "You haven't selected any filter!");
			return false;
		} else {
			mFilterType = GrayScaleType(bg.checkedId());
		}
	}
	return FilterInterface::setup(img);
}

DisplayWindow *GrayScaleFilter::apply(QString windowBaseName)
{
	QImage result(mImg.size(), mFormat);
	if (mFormat == QImage::Format_Indexed8 || mFormat == QImage::Format_Mono) {
		result.setColorTable(mImg.colorTable());
	}
	ColorParser cp(mFormat);
	for (int x = 0; x < result.size().width(); x++) {
		for (int y = 0; y < result.size().height(); y++) {
			QColor color;
			QVector3D v;
			switch (mFilterType) {
				case Min:
					{
						v = cp.pixel(x, y, mImg);
						int gray = qMin(qMin(v.x(), v.y()), v.z());
						v = QVector3D(gray, gray, gray);
					}
					break;
				case Proportional:
					{
						v = cp.pixel(x, y, mImg);
						int r = 0.299 * v.x();
						int g = 0.587 * v.y();
						int b = 0.114 * v.z();
						int gray = qBound(0, r + g + b, 255);
						v = QVector3D(gray, gray, gray);
					}
					break;
			}
			cp.setPixel(x, y, result, v);
		}
	}
	// parent's parent should be MainWindow
	return new PhotoWindow(result, windowBaseName + ", " + name(), q_check_ptr(qobject_cast<QWidget *>(parent()->parent())));
}
