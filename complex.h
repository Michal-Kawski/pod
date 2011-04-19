#ifndef COMPLEX_H
#define COMPLEX_H

#include <QVector2D>

class QDebug;

class Complex
{
public:
	Complex();
	Complex(const Complex &other);
	Complex(const qreal &real, const qreal &imaginary);

	static Complex fromPowerPhase(const qreal &power, const qreal &phase);

	qreal abs() const;
	qreal real() const;
	qreal imaginary() const;
	qreal phase() const;
	void setReal(const qreal &real);
	void setImaginary(const qreal &imaginary);

	Complex &operator =(const Complex &rhs);
	Complex &operator+=(const Complex &rhs);
	Complex &operator-=(const Complex &rhs);
	Complex &operator*=(const Complex &rhs);
	const Complex operator +(const Complex &rhs) const;
	const Complex operator -(const Complex &rhs) const;
	const Complex operator *(const Complex &rhs) const;
	bool operator==(const Complex &rhs) const;
	bool operator!=(const Complex &rhs) const;
	friend QDebug operator<<(QDebug &stream, const Complex &rhs);

private:
	QVector2D mNumber;
};

#endif // COMPLEX_H
