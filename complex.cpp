#include "complex.h"

#include <cmath>
#include <QDebug>

/*
 * see http://www.cs.caltech.edu/courses/cs11/material/cpp/donnie/cpp-ops.html
 */

Complex::Complex()
{
}

Complex::Complex(const Complex &other)
{
	this->mNumber = other.mNumber;
}

Complex::Complex(const qreal &real, const qreal &imaginary) :
	mNumber(QVector2D(real, imaginary))
{
}

Complex Complex::fromPowerPhase(const qreal &power, const qreal &phase)
{
	return Complex(power * cos(phase), power * sin(phase));
}

qreal Complex::abs() const
{
	return mNumber.length();
}

qreal Complex::phase() const
{
	// http://en.wikipedia.org/wiki/Atan2
	qreal result = atan2(imaginary(), real());
	return result;
}

qreal Complex::real() const
{
	return mNumber.x();
}

qreal Complex::imaginary() const
{
	return mNumber.y();
}

void Complex::setReal(const qreal &real)
{
	mNumber.setX(real);
}

void Complex::setImaginary(const qreal &imaginary)
{
	mNumber.setY(imaginary);
}

Complex &Complex::operator =(const Complex &rhs)
{
	// it is safe to skip self-assignment check here
	this->mNumber = rhs.mNumber;
	return *this;
}

Complex &Complex::operator+=(const Complex &rhs)
{
	this->mNumber += rhs.mNumber;
	return *this;
}

Complex &Complex::operator-=(const Complex &rhs)
{
	this->mNumber -= rhs.mNumber;
	return *this;
}

Complex &Complex::operator*=(const Complex &rhs)
{
	// http://en.wikipedia.org/wiki/Complex_number#Multiplication_and_division
	QVector2D result;
	result.setX(real() * rhs.real() - imaginary() * rhs.imaginary());
	result.setY(imaginary() * rhs.real() + real() * rhs.imaginary());
	this->mNumber = result;
	return *this;
}

const Complex Complex::operator +(const Complex &rhs) const
{
	return Complex(*this) += rhs;
}

const Complex Complex::operator -(const Complex &rhs) const
{
	return Complex(*this) -= rhs;
}

const Complex Complex::operator *(const Complex &rhs) const
{
	return Complex(*this) *= rhs;
}

bool Complex::operator ==(const Complex &rhs) const
{
	return this->mNumber == rhs.mNumber;
}

bool Complex::operator !=(const Complex &rhs) const
{
	return this->mNumber != rhs.mNumber;
}

QDebug operator <<(QDebug &stream, const Complex &rhs)
{
	stream << "Complex(" << rhs.mNumber.x() << "," << rhs.mNumber.y() << ")";
	return stream.space();
}
