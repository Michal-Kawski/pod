#ifndef PHOTO_H
#define PHOTO_H

#include <QDialog>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QPaintEvent>

namespace Ui {
    class Photo;
}

class Photo : public QDialog
{
    Q_OBJECT

public:
    explicit Photo(QWidget *parent = 0, QString newUrl = "photo.jpg", uint noumber = 1);
    ~Photo();
    virtual void paintEvent(QPaintEvent *);

private:
    Ui::Photo *ui;
    QString url;
    QImage* photo;
    QPainter* painter;
    void closeEvent(QCloseEvent *);
    void focusInEvent(QFocusEvent *);

public slots:
    void saveToFile(Photo *p);

signals:
    void eraseThis(Photo* p);
    void markThisAsCurrent(Photo* p);
};

#endif // PHOTO_H
