#ifndef SLIDINGVALUEDIALOG_H
#define SLIDINGVALUEDIALOG_H

#include <QDialog>

class QSlider;
class QLabel;

class SlidingValueDialog : public QDialog
{
	Q_OBJECT
public:
	explicit SlidingValueDialog(QString text, int defValue = 127, int min = 0,
								int max = 255, QWidget *parent = 0);
	int value() const;

signals:

public slots:

protected slots:
	void updateText(int value);

protected:
	QString mText;
	QSlider *mSlider;
	QLabel *mLabel;
};

#endif // SLIDINGVALUEDIALOG_H
