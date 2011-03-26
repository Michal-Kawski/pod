#include "kernelvaluesdialog.h"

#include <QSpinBox>
#include <QGridLayout>
#include <QDialogButtonBox>

#include <QDebug>

KernelValuesDialog::KernelValuesDialog(int width, int height, QWidget *parent) :
    QDialog(parent)
{
	QVBoxLayout *vbl = new QVBoxLayout(this);
	QWidget *inputWidget = new QWidget(this);
	inputGridLayout = new QGridLayout(inputWidget);
	for (int i = 0; i < 2 * width + 1; i++) {
		for (int j = 0; j < 2 * height + 1; j++) {
			QSpinBox *spin = new QSpinBox(this);
			spin->setMinimum(-100);
			inputGridLayout->addWidget(spin, j, i);
		}
	}
	QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(dbb, SIGNAL(accepted()), this, SLOT(accept()));
	connect(dbb, SIGNAL(rejected()), this, SLOT(reject()));
	vbl->addWidget(inputWidget);
	vbl->addWidget(dbb);
}

QVector<QVector<int> > KernelValuesDialog::values() const
{
	QVector<QVector<int> > result;
	result.reserve(inputGridLayout->rowCount());
	for (int i = 0; i < inputGridLayout->rowCount(); i++) {
		QVector<int> row;
		for (int j = 0; j < inputGridLayout->columnCount(); j++) {
			QSpinBox *spin = qobject_cast<QSpinBox *>(inputGridLayout->itemAtPosition(i, j)->widget());
			if (spin == NULL) {
				qDebug() << "element at " << i << ", " << j << " is not a spinbox";
				continue;
			}
			row << spin->value();
		}
		result << row;
	}
	return result;
}
