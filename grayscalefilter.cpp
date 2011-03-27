#include "grayscalefilter.h"

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
		bg.addButton(new QRadioButton("qGray",
									  qobject_cast<QWidget *>(&typeDialog)),
					 Gray);
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
	mImg = img;
	return true;
}

QImage GrayScaleFilter::apply()
{
	QImage result(mImg.size(), QImage::Format_ARGB32);
	for (int x = 0; x < result.size().width(); x++) {
		for (int y = 0; y < result.size().height(); y++) {
			QColor color;
			switch (mFilterType) {
				case Gray:
					{
						int gray = qGray(mImg.pixel(x, y));
						color = QColor(gray, gray, gray);
					}
					break;
				case Proportional:
					{
						QColor pixel(mImg.pixel(x, y));
						int r = 0.299 * pixel.red();
						int g = 0.587 * pixel.green();
						int b = 0.114 * pixel.blue();
						int gray = qBound(0, r + g + b, 255);
						color = QColor(gray, gray, gray);
					}
					break;
			}
			result.setPixel(x, y, color.rgb());
		}
	}
	return result;
}
