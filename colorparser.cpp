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
				int i = qGray(qRgb(color.x(), color.y(), color.z()));
				img.setPixel(pos, i);
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
				img.setPixel(pos, i);
			}
			break;
		default:
			qDebug() << "invalid image format" << mFormat;
	}
}
