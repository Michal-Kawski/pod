#include "photowindow.h"
#include "ui_photowindow.h"

#include <QDebug>
#include <QFileDialog>


PhotoWindow::PhotoWindow(QWidget *parent, QString newUrl, uint noumber) :
    QMainWindow(parent),
    ui(new Ui::PhotoWindow)
{
    this->setFocusPolicy(Qt::StrongFocus);

    url = newUrl;

    ui->setupUi(this);
    this->setWindowTitle("Photo" + QString::number(noumber));

    dockWidget = new DockWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
}

PhotoWindow::~PhotoWindow()
{
    emit eraseThis(this);
    delete dockWidget;
    delete painter;
    delete photo;
    delete ui;
}

void PhotoWindow::paintEvent(QPaintEvent *){
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

void PhotoWindow::closeEvent(QCloseEvent *){
    delete this;
}

void PhotoWindow::saveToFile(PhotoWindow *p){
    QString url = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Image Files(*.png, *.jpg, *.bmp"));
    photo->save(url, "JPG");
}

void PhotoWindow::focusInEvent(QFocusEvent *){
    emit markThisAsCurrent(this);
}

void PhotoWindow::on_actionGeneruj_histogramy_triggered()
{
    krgb.resize(4);
    krgb[0].resize(256);
    krgb[0].fill(0);
    krgb[1].resize(256);
    krgb[1].fill(0);
    krgb[2].resize(256);
    krgb[2].fill(0);
    krgb[3].resize(256);
    krgb[3].fill(0);

    QColor color;
    uint maxK = 0;
    uint maxR = 0;
    uint maxG = 0;
    uint maxB = 0;

    for(int i=0; i<photo->width(); i++){
        for(int j=0; j<photo->height(); j++){
            color = photo->pixel(i, j);

            krgb[0][color.black()]++;
            if(krgb[0][color.black()] > (int)maxK)
                maxK = krgb[0][color.black()];

            krgb[1][color.red()]++;
            if(krgb[1][color.red()] > (int)maxR)
                maxR = krgb[1][color.red()];

            krgb[2][color.green()]++;
            if(krgb[2][color.green()] > (int)maxG)
                maxG = krgb[2][color.green()];

            krgb[3][color.blue()]++;
            if(krgb[3][color.blue()] > (int)maxB)
                maxB = krgb[3][color.blue()];
        }
    }

    QVector<int> v;
    v.append(maxK);
    v.append(maxR);
    v.append(maxG);
    v.append(maxB);

    dockWidget->setMaxValues(v);
    dockWidget->setKrgb(&krgb);
}
