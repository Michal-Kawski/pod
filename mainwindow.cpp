#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	QStringList filters;
	filters << "average" << "median";
	QStringList base;
	base << "lenac" << "lena";
	QStringList noises;
	noises << "normal3" << "uniform3" << "impulse3";
	QStringList masks;
	masks << "3x3" << "5x5" << "7x7";
	foreach (QString filter, filters) {
		foreach (QString baseName, base) {
			foreach (QString noise, noises) {
				foreach (QString mask, masks) {
					QString file = "/home/janisozaur/Dropbox/workspace/pod/pierwsze/sprawko/img/" + baseName + "_" + noise + "_" + filter + "_" + mask + ".png";
					//qDebug() << "opening" << file;
					open(file);
				}
			}
			qDebug() << "*****";
		}
	}
	foreach (QString baseName, base) {
		foreach (QString noise, noises) {
			QString file = "/home/janisozaur/Dropbox/workspace/pod/pierwsze/sprawko/img/" + baseName + "_" + noise + ".png";
			//qDebug() << "opening" << file;
			open(file);
		}
	}
}

void MainWindow::open(QString fileName)
{
	QString fileUrl = fileName;
	QFileInfo fi(fileUrl);
	PhotoWindow *p = new PhotoWindow(fileUrl, fi.fileName(), this);
	connect(p, SIGNAL(eraseThis(PhotoWindow*)), this, SLOT(eraseFromList(PhotoWindow*)));
	connect(p, SIGNAL(markThisAsCurrent(PhotoWindow*)), this, SLOT(setCurrentPhoto(PhotoWindow*)));
	p->show();
	photosList.append(p);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOtw_rz_triggered()
{
    QString fileUrl = QFileDialog::getOpenFileName(this, tr("Wybierz plik do otwarcia: "), "photo.jpg");
    if(fileUrl.isEmpty())
        return;

	QFileInfo fi(fileUrl);
	PhotoWindow *p = new PhotoWindow(fileUrl, fi.fileName(), this);
    connect(p, SIGNAL(eraseThis(PhotoWindow*)), this, SLOT(eraseFromList(PhotoWindow*)));
	connect(p, SIGNAL(markThisAsCurrent(PhotoWindow*)), this, SLOT(setCurrentPhoto(PhotoWindow*)));
    p->show();
    photosList.append(p);
}

void MainWindow::on_actionZapisz_triggered()
{
    qDebug()<<"zapisz dźwięk";
}

void MainWindow::on_actionOtw_rz_2_triggered()
{
    qDebug()<<"otwórz dźwięk";
}

void MainWindow::setCurrentPhoto(PhotoWindow *p){
    currentPhoto = p;
}

void MainWindow::eraseFromList(PhotoWindow *p){
    for(int i=0; i<photosList.size(); i++){
        if(photosList.at(i) == p){
            photosList.removeAt(i);
            qDebug()<<"okno "<<p<<" usuniete z listy";
            break;
        }
    }
}
