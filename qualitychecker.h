#ifndef QUALITYCHECKER_H
#define QUALITYCHECKER_H

class QImage;

class QualityChecker
{
public:
	QualityChecker();
	bool operator() (const QImage &ref, const QImage &img);
	double snr;
	double mse;
};

#endif // QUALITYCHECKER_H
