#include "displaywindow.h"

DisplayWindow::DisplayWindow(QWidget *parent) :
	QMainWindow(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
}

DisplayWindow::~DisplayWindow()
{
}
