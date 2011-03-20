#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    PhotoWindow *p = new PhotoWindow(this, fileUrl, photosList.size()+1);
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
