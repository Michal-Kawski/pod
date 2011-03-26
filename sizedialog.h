#ifndef SIZEDIALOG_H
#define SIZEDIALOG_H

#include <QDialog>

class QSpinBox;

class SizeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SizeDialog(QWidget *parent = 0);
	QSize kernelSize() const;

signals:

public slots:

private:
	QSpinBox *mWidthSpinBox, *mHeightSpinBox;
};

#endif // SIZEDIALOG_H
