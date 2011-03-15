#include "photo.h"
#include "ui_photo.h"

#include <QDebug>
#include <QFileDialog>

Photo::Photo(QWidget *parent, QString newUrl, uint noumber) :
    QDialog(parent),
    ui(new Ui::Photo)
{
    this->url = newUrl;
    this->setFocusPolicy(Qt::StrongFocus);
    ui->setupUi(this);
    this->setWindowTitle("Photo" + QString::number(noumber));
}

Photo::~Photo()
{
    emit eraseThis(this);
    delete painter;
    delete photo;
    delete ui;
}

void Photo::paintEvent(QPaintEvent *){
    photo = new QImage(this->url);
    painter = new QPainter(this);
    int maxWidth = QApplication::desktop()->width() - 200;
    int maxHeight = QApplication::desktop()->height();

    if(photo->width() > maxWidth){
        *photo = photo->scaledToWidth(maxWidth);
    }

    if(photo->height() > maxHeight){
        *photo = photo->scaledToHeight(maxHeight);
    }

    this->resize(photo->width(), photo->height());

    painter->drawImage(0.0, 0.0, *photo);
}

void Photo::closeEvent(QCloseEvent *){
    delete this;
}

void Photo::saveToFile(Photo *p){
    qDebug()<<"zapisuje obraz";
    QString url = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Image Files(*.png, *.jpg, *.bmp"));
    photo->save(url, "JPG");
}

void Photo::focusInEvent(QFocusEvent *){
    emit markThisAsCurrent(this);
}
