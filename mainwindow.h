#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

#include "photowindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	void on_actionOtw_rz_2_triggered();
	void on_actionZapisz_triggered();
    void on_actionOtw_rz_triggered();
    void eraseFromList(PhotoWindow* p);
    void setCurrentPhoto(PhotoWindow* p);
	void open(QString fileName);

private:
    Ui::MainWindow *ui;
    QList<PhotoWindow*> photosList;
    PhotoWindow* currentPhoto;
};

#endif // MAINWINDOW_H
