#include "photowindow.h"
#include "ui_photowindow.h"
#include "negativefilter.h"
#include "convolutionfilter.h"
#include "medianfilter.h"
#include "slidingvaluedialog.h"
#include "brightnessfilter.h"
#include "grayscalefilter.h"
#include "averagefilter.h"

#include <QDebug>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QMenu>

Q_DECLARE_METATYPE(QUuid)

PhotoWindow::PhotoWindow(QString newUrl, QString title, QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::PhotoWindow),
	mImage(newUrl)
{
	constructorInternals(title);
}

PhotoWindow::PhotoWindow(QImage img, QString title, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::PhotoWindow),
	mImage(img)
{
	constructorInternals(title);
}

void PhotoWindow::constructorInternals(const QString &title)
{
	ui->setupUi(this);

	dockWidget = new DockWidget(this);
	menuBar()->addAction(dockWidget->toggleViewAction());

	mFiltersMenu = menuBar()->addMenu("Filters");
	this->setFocusPolicy(Qt::StrongFocus);

	ui->imageLabel->setPixmap(QPixmap::fromImage(mImage));
	this->setWindowTitle(title);
	QSize size = QApplication::desktop()->screenGeometry().size();
	size = mImage.size().boundedTo(size);
	this->resize(mImage.size());

	//addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

	appendFilter(new NegativeFilter(this));
	appendFilter(new ConvolutionFilter(this));
	appendFilter(new MedianFilter(this));
	appendFilter(new BrightnessFilter(this));
	appendFilter(new GrayScaleFilter(this));
	appendFilter(new AverageFilter(this));

	connect(mFiltersMenu, SIGNAL(triggered(QAction*)), this, SLOT(applyFilter(QAction*)));
}

PhotoWindow::~PhotoWindow()
{
    emit eraseThis(this);
	delete dockWidget;
    delete ui;
}

void PhotoWindow::applyFilter(QAction *action)
{
	qDebug() << "applying filter" << action->data().value<QUuid>();
	FilterInterface *filter = mFiltersHash[action->data().value<QUuid>()];
	qDebug() << "filter name:" << filter->name();
	if (filter->setup(mImage)) {
		QImage img = filter->apply();
		// new PhotoWindow should always be a child of MainWindow
		PhotoWindow *pw = new PhotoWindow(img,
										  windowTitle() + ", " + filter->name(),
										  this->parentWidget());
		pw->show();
	}
}

void PhotoWindow::appendFilter(FilterInterface *filter)
{
	mFiltersHash.insert(filter->uuid(), filter);
	QAction *menuAction = new QAction(filter->name(), this);
	QVariant v;
	v.setValue(filter->uuid());
	qDebug() << filter->uuid();
	menuAction->setData(v);
	mFiltersMenu->addAction(menuAction);
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
