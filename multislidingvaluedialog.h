#ifndef MULTISLIDINGVALUEDIALOG_H
#define MULTISLIDINGVALUEDIALOG_H

#include <QDialog>
#include <QVector>

class QSlider;
class QLabel;

class MultiSlidingValueDialog : public QDialog
{
	Q_OBJECT
public:
	explicit MultiSlidingValueDialog(QString text, int defValue, int min,
									 int max, int count, QWidget *parent = 0);
	QVector<int> values() const;

signals:

public slots:

protected slots:
	void updateText(int value);

protected:
	QString mText;
	QVector<QSlider *> mSliders;
	QVector<QLabel *> mLabels;
};

#endif // MULTISLIDINGVALUEDIALOG_H
