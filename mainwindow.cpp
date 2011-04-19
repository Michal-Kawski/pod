#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <QDebug>

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
	QString fileUrl = QFileDialog::getOpenFileName(this, tr("Wybierz plik do otwarcia: "), QString(), "Images (*.png *.xpm *.jpg *.bmp);;Any (*)");
    if(fileUrl.isEmpty())
        return;

    QFileInfo fi(fileUrl);
	PhotoWindow *p = new PhotoWindow(fileUrl, fi.fileName(), this);
	p->show();
}

void MainWindow::on_actionZapisz_triggered()
{
    qDebug()<<"zapisz dźwięk";
}

void MainWindow::on_actionOtw_rz_2_triggered()
{
    qDebug()<<"otwórz dźwięk";
}
