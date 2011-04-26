#include "colorparser.h"

#include <QVector3D>

#include <QDebug>

ColorParser::ColorParser(QImage::Format format) :
	mFormat(format)
{
}

QVector3D ColorParser::pixel(int x, int y, const QImage &img) const
{
	QVector3D result;
	switch (mFormat) {
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32:
		case QImage::Format_ARGB32_Premultiplied:
			{
				QRgb c = img.pixel(x, y);
				result = QVector3D(qRed(c), qGreen(c), qBlue(c));
			}
			break;
		case QImage::Format_Indexed8:
			{
				int i = img.pixelIndex(x, y);
				result = QVector3D(i, i, i);
			}
			break;
		case QImage::Format_Mono:
			{
				int i = img.pixelIndex(x, y) * 255;
				result = QVector3D(i, i, i);
			}
			break;
		default:
			qDebug() << "invalid image format" << mFormat;
	}
	return result;
}

QVector3D ColorParser::pixel(const QPoint &pos, const QImage &img) const
{
	QVector3D result;
	switch (mFormat) {
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32:
		case QImage::Format_ARGB32_Premultiplied:
			{
				QRgb c = img.pixel(pos);
				result = QVector3D(qRed(c), qGreen(c), qBlue(c));
			}
			break;
		case QImage::Format_Indexed8:
			{
				int i = img.pixelIndex(pos);
				result = QVector3D(i, i, i);
			}
			break;
		case QImage::Format_Mono:
			{
				int i = img.pixelIndex(pos) * 255;
				result = QVector3D(i, i, i);
			}
			break;
		default:
			qDebug() << "invalid image format" << mFormat;
	}
	return result;
}

void ColorParser::setPixel(int x, int y, QImage &img, QVector3D color)
{
	switch (mFormat) {
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32:
		case QImage::Format_ARGB32_Premultiplied:
			{
				QRgb c(qRgb(color.x(), color.y(), color.z()));
				img.setPixel(x, y, c);
			}
			break;
		case QImage::Format_Indexed8:
			{
				int i = qGray(qRgb(color.x(), color.y(), color.z()));
				img.setPixel(x, y, i);
			}
			break;
		case QImage::Format_Mono:
			{
				int i = qGray(qRgb(color.x(), color.y(), color.z()));
				if (i > 127) {
					i = 1;
				} else {
					i = 0;
				}
				img.setPixel(x, y, i);
			}
			break;
		default:
			qDebug() << "invalid image format" << mFormat;
	}
}

void ColorParser::setPixel(const QPoint &pos, QImage &img, QVector3D color)
{
	switch (mFormat) {
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32:
		case QImage::Format_ARGB32_Premultiplied:
			{
				QRgb c(qRgb(color.x(), color.y(), color.z()));
				img.setPixel(pos, c);
			}
			break;
		case QImage::Format_Indexed8:
			{
				int i = qGray(qRgb(color.x(), color.x(), color.x()));
				img.setPixel(pos, i);
			}
			break;
		case QImage::Format_Mono:
			{
				int i = qGray(qRgb(color.x(), color.x(), color.x()));
				if (i > 127) {
					i = 1;
				} else {
					i = 0;
				}
				img.setPixel(pos, i);
			}
			break;
		default:
			qDebug() << "invalid image format" << mFormat;
	}
}

QVector3D ColorParser::merge(const QVector3D &left, const QVector3D &right) const
{
	QVector3D result;
	switch (mFormat) {
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32:
		case QImage::Format_ARGB32_Premultiplied:
			result.setX((int)left.x() | (int)right.x());
			result.setY((int)left.y() | (int)right.y());
			result.setZ((int)left.z() | (int)right.z());
			break;
		case QImage::Format_Indexed8:
			{
				int val = (int)left.x() | (int)right.x();
				result = QVector3D(val, val, val);
			}
			break;
		default:
			qDebug() << "unsupported image format" << mFormat;
	}
	return result;
}
