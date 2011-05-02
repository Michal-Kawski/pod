#include "multislidingvaluedialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QSlider>
#include <QLabel>

MultiSlidingValueDialog::MultiSlidingValueDialog(QString text, int defValue,
												 int min, int max, int count,
												 QWidget *parent) :
	QDialog(parent),
	mText(text)
{
	QVBoxLayout *vbl = new QVBoxLayout(this);
	QWidget *widget = new QWidget(this);
	vbl->addWidget(widget);

	QFormLayout *fl = new QFormLayout(widget);

	for (int i = 0; i < count; i++) {
		QLabel *label = new QLabel(text, this);
		QSlider *slider = new QSlider(Qt::Horizontal, this);
		mLabels << label;
		mSliders << slider;
		slider->setObjectName(QString::number(i));
		slider->setMinimum(min);
		slider->setMaximum(max);
		connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateText(int)));
		slider->setValue(defValue);
		updateText(slider->value());
		fl->addRow(label, slider);
	}

	QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(dbb, SIGNAL(accepted()), this, SLOT(accept()));
	connect(dbb, SIGNAL(rejected()), this, SLOT(reject()));
	vbl->addWidget(dbb);
}

void MultiSlidingValueDialog::updateText(int value)
{
	int i;
	QObject *slider;
	if (sender() != NULL) {
		for (i = 0; i < mSliders.count(); i++) {
			if (mSliders.at(i) == sender()) {
				break;
			}
		}
		slider = sender();
	} else {
		i = mSliders.count() - 1;
		slider = mSliders.at(i);
	}
	QString name = slider->objectName();
	QString textVal = QString::number(value);
	mLabels[i]->setText(mText.arg(name, textVal));
}

QVector<int> MultiSlidingValueDialog::values() const
{
	QVector<int> results;
	foreach (QSlider *slider, mSliders) {
		results << slider->value();
	}
	return results;
}
