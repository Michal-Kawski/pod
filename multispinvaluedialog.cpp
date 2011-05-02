#include "multispinvaluedialog.h"

#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>

MultiSpinValueDialog::MultiSpinValueDialog(int count, QStringList strings,
										   int defValue, int min, int max,
										   QWidget *parent) :
	QDialog(parent),
	mStrings(strings)
{
	Q_ASSERT(count == strings.count());

	QVBoxLayout *vbl = new QVBoxLayout(this);
	QWidget *widget = new QWidget(this);
	vbl->addWidget(widget);

	QFormLayout *fl = new QFormLayout(widget);

	for (int i = 0; i < count; i++) {
		QDoubleSpinBox *spinner = new QDoubleSpinBox(this);
		QLabel *label = new QLabel(mStrings.at(i), this);
		mSpinners << spinner;
		mLabels << label;
		spinner->setMinimum(min);
		spinner->setMaximum(max);
		spinner->setValue(defValue);
		connect(spinner, SIGNAL(valueChanged(double)), this, SLOT(updateText(double)));
		updateText(spinner->value());
		fl->addRow(label, spinner);
	}

	QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(dbb, SIGNAL(accepted()), this, SLOT(accept()));
	connect(dbb, SIGNAL(rejected()), this, SLOT(reject()));
	vbl->addWidget(dbb);
}

void MultiSpinValueDialog::updateText(double value)
{
	int i;
	if (sender() != NULL) {
		for (i = 0; i < mSpinners.count(); i++) {
			if (mSpinners.at(i) == sender()) {
				break;
			}
		}
	} else {
		i = mSpinners.count() - 1;
	}
	QString textVal = QString::number(value);
	mLabels[i]->setText(mStrings.at(i).arg(textVal));
}

QVector<double> MultiSpinValueDialog::values() const
{
	QVector<double> results;
	foreach (QDoubleSpinBox *spinner, mSpinners) {
		results << spinner->value();
	}
	return results;
}
