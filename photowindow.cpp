#include "photowindow.h"
#include "ui_photowindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>

PhotoWindow::PhotoWindow(QWidget *parent, QString newUrl, uint noumber) :
    QMainWindow(parent),
	ui(new Ui::PhotoWindow),
	mImage(newUrl)
{
	ui->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);

	ui->imageLabel->setPixmap(QPixmap::fromImage(mImage));
    this->setWindowTitle("Photo" + QString::number(noumber));
	this->resize(mImage.size());

    dockWidget = new DockWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
}

PhotoWindow::~PhotoWindow()
{
    emit eraseThis(this);
	delete dockWidget;
    delete ui;
}

void PhotoWindow::closeEvent(QCloseEvent *){
    delete this;
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

	for(int i=0; i < mImage.width(); i++){
		for(int j=0; j < mImage.height(); j++){
			color = mImage.pixel(i, j);

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
	dockWidget->update();
}

void PhotoWindow::on_actionSave_triggered()
{
	QString url = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("png image (*.png)"));
	mImage.save(url);
}
