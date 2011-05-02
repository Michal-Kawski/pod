#ifndef MULTISPINVALUEDIALOG_H
#define MULTISPINVALUEDIALOG_H

#include <QDialog>

class QDoubleSpinBox;
class QLabel;

class MultiSpinValueDialog : public QDialog
{
	Q_OBJECT
public:
	explicit MultiSpinValueDialog(int count, QStringList strings, int defValue,
								  int min, int max, QWidget *parent = 0);
	QVector<double> values() const;

signals:

protected slots:
	void updateText(double value);

protected:
	QVector<QDoubleSpinBox *> mSpinners;
	QStringList mStrings;
	QVector<QLabel *> mLabels;
};

#endif // MULTISPINVALUEDIALOG_H
