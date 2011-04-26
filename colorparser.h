#ifndef COLORPARSER_H
#define COLORPARSER_H

#include <QImage>
#include <QVector3D>

class ColorParser
{
public:
	ColorParser(QImage::Format format);
	QVector3D pixel(int x, int y, const QImage &img) const;
	QVector3D pixel(const QPoint &pos, const QImage &img) const;
	void setPixel(int x, int y, QImage &img, QVector3D color);
	void setPixel(const QPoint &pos, QImage &img, QVector3D color);
	QVector3D merge(const QVector3D &left, const QVector3D &right) const;

private:
	const QImage::Format mFormat;
};

#endif // COLORPARSER_H
