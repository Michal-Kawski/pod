#include "slidingvaluedialog.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QSlider>
#include <QLabel>

SlidingValueDialog::SlidingValueDialog(QString text, int defValue, int min,
									   int max, QWidget *parent) :
	QDialog(parent),
	mText(text)
{
	QVBoxLayout *vbl = new QVBoxLayout(this);
	mLabel = new QLabel(text, this);
	mSlider = new QSlider(Qt::Horizontal, this);
	mSlider->setMinimum(min);
	mSlider->setMaximum(max);
	connect(mSlider, SIGNAL(valueChanged(int)), this, SLOT(updateText(int)));
	mSlider->setValue(defValue);
	QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(dbb, SIGNAL(accepted()), this, SLOT(accept()));
	connect(dbb, SIGNAL(rejected()), this, SLOT(reject()));
	vbl->addWidget(mLabel);
	vbl->addWidget(mSlider);
	vbl->addWidget(dbb);
}

void SlidingValueDialog::updateText(int value)
{
	mLabel->setText(mText + " " + QString::number(value));
}

int SlidingValueDialog::value() const
{
	return mSlider->value();
}
