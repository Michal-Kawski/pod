#ifndef KERNELVALUESDIALOG_H
#define KERNELVALUESDIALOG_H

#include <QDialog>

class QGridLayout;

class KernelValuesDialog : public QDialog
{
    Q_OBJECT
public:
	explicit KernelValuesDialog(int width, int height, QWidget *parent = 0);
	QVector<QVector<int> > values() const;

signals:

public slots:

private:
	QGridLayout *inputGridLayout;
};

#endif // KERNELVALUESDIALOG_H
