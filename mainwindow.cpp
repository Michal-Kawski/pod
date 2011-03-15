#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "photo.h"

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

    Photo *p = new Photo(this, fileUrl, photosList.size()+1);
    connect(p, SIGNAL(eraseThis(Photo*)), this, SLOT(eraseFromList(Photo*)));
    connect(p, SIGNAL(markThisAsCurrent(Photo*)), this, SLOT(setCurrentPhoto(Photo*)));
    connect(this, SIGNAL(savePhoto(Photo*)), p, SLOT(saveToFile(Photo*)));
    p->show();
    photosList.append(p);
}

void MainWindow::on_actionZapisz_2_triggered()
{
    emit savePhoto(currentPhoto);
}

void MainWindow::on_actionZapisz_triggered()
{
    qDebug()<<"zapisz dźwięk";
}

void MainWindow::on_actionOtw_rz_2_triggered()
{
    qDebug()<<"otwórz dźwięk";
}

void MainWindow::setCurrentPhoto(Photo *p){
    currentPhoto = p;
}

void MainWindow::eraseFromList(Photo *p){
    for(int i=0; i<photosList.size(); i++){
        if(photosList.at(i) == p){
            photosList.removeAt(i);
            qDebug()<<"okno "<<p<<" usuniete z listy";
            break;
        }
    }
}
