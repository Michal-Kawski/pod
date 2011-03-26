#include "sizedialog.h"

#include <QVBoxLayout>
#include <QSpinBox>
#include <QDialogButtonBox>

SizeDialog::SizeDialog(QWidget *parent) :
    QDialog(parent)
{
	QVBoxLayout *vbl = new QVBoxLayout(this);
	QWidget *maskDataWidget = new QWidget(qobject_cast<QWidget *>(this));
	QHBoxLayout *mdl = new QHBoxLayout(maskDataWidget);
	vbl->addWidget(maskDataWidget);
	mWidthSpinBox = new QSpinBox(this);
	mWidthSpinBox->setToolTip("width");
	mHeightSpinBox = new QSpinBox(this);
	mHeightSpinBox->setToolTip("height");
	mdl->addWidget(mWidthSpinBox);
	mdl->addWidget(mHeightSpinBox);
	QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	vbl->addWidget(dbb);

	connect(dbb, SIGNAL(accepted()), this, SLOT(accept()));
	connect(dbb, SIGNAL(rejected()), this, SLOT(reject()));
}

QSize SizeDialog::kernelSize() const
{
	QSize result;
	result.setHeight(mHeightSpinBox->value());
	result.setWidth(mWidthSpinBox->value());
	return result;
}
