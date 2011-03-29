#include "photowindow.h"
#include "ui_photowindow.h"
#include "negativefilter.h"
#include "convolutionfilter.h"
#include "medianfilter.h"
#include "slidingvaluedialog.h"
#include "brightnessfilter.h"
#include "grayscalefilter.h"
#include "averagefilter.h"
#include "contrastfilter.h"
#include "rosenfeldfilter.h"
#include "qualitychecker.h"

#include <QDebug>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QMenu>
#include <cmath>
#include <QAction>
#include <QRgb>

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
	qDebug() << "image format:" << mImage.format();
	ui->setupUi(this);

	dockWidget = new DockWidget(this);
	connect(dockWidget, SIGNAL(changeHistogram(int,int,float,QVector<int>)), this, SLOT(changeHistogram(int,int,float,QVector<int>)));
	menuBar()->addAction(dockWidget->toggleViewAction());

	mFiltersMenu = menuBar()->addMenu("Filters");
	this->setFocusPolicy(Qt::StrongFocus);

	ui->imageLabel->setPixmap(QPixmap::fromImage(mImage));
	this->setWindowTitle(title);
	QSize size = QApplication::desktop()->screenGeometry().size();
	size = mImage.size().boundedTo(size);
	this->resize(mImage.size());

	QAction *checkQuality = new QAction("check quality", this);
	connect(checkQuality, SIGNAL(triggered()), this, SLOT(qualityCheck()));
	menuBar()->addAction(checkQuality);

	//addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

	appendFilter(new NegativeFilter(this));
	appendFilter(new ConvolutionFilter(this));
	appendFilter(new MedianFilter(this));
	appendFilter(new BrightnessFilter(this));
	appendFilter(new GrayScaleFilter(this));
	appendFilter(new AverageFilter(this));
	appendFilter(new ContrastFilter(this));
	appendFilter(new RosenfeldFilter(this));

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

            krgb[0][qGray(color.rgb())]++;
            if(krgb[0][qGray(color.rgb())] > (int)maxK)
                maxK = krgb[0][qGray(color.rgb())];

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

void PhotoWindow::changeHistogram(int color, int gMin, float alfa, QVector<int> histValues){
    qDebug()<<"zmieniam histogram";
    QVector<int> tmpVector(256);
    QImage img = mImage.copy(0,0,mImage.width(), mImage.height());

    switch(color){
    case 0:
        for(int i=0; i<krgb.at(0).size(); i++){
            int tmp = calculateRaleigh(i, gMin, alfa, histValues);
            tmpVector[i] = tmp;
            changeImageColor(0, i, tmp, &mImage);
        }

        krgb.replace(0, tmpVector);
        break;
    case 1:
        for(int i=0; i<krgb.at(1).size(); i++){
            int tmp = calculateRaleigh(i, gMin, alfa, histValues);
            tmpVector[i] = tmp;
            changeImageColor(0, i, tmp, &mImage);
        }

        krgb.replace(1, tmpVector);
        break;
    case 2:
        for(int i=0; i<krgb.at(2).size(); i++){
            int tmp = calculateRaleigh(i, gMin, alfa, histValues);
            tmpVector[i] = tmp;
            changeImageColor(0, i, tmp, &mImage);
        }

        krgb.replace(2, tmpVector);
        break;
    case 3:
        for(int i=0; i<krgb.at(3).size(); i++){
            int tmp = calculateRaleigh(i, gMin, alfa, histValues);
            tmpVector[i] = tmp;
            changeImageColor(0, i, tmp, &mImage);
        }

        krgb.replace(3, tmpVector);
        break;
    }

    dockWidget->setKrgb(&krgb);
    dockWidget->setMaxValues(findMaxValues());

    mImage = img;
    dockWidget->update();
}

int PhotoWindow::calculateRaleigh(int position, int gMin, float alfa, QVector<int> histValues){
    int sum = 0.00;
    for(int i=0; i<position; i++){
        sum += histValues.at(i);
    }

    /*Jeżeli we wzorze N okresla calkowita ilosc pikseli to logarytm we wzorze wychodzi bliski zeru.
      Po obliczeniu reszty wzoru wynik koncowy jest praktycznie staly w kazdym przypadku (w kazdej iteracji)
      - zmieniaja sie liczby tylko po przecinku wiec nie wiem za bardzo dlaczemu jest zle :/
      Ktos mnie uprzedzal zebym uwazal wlasnie na wzory od tomczyka bo sie bledy trafiaja czesto. Jak szukalem
      na necie to nie znalazlem zadnego wzoru o tej nazwie.
     */
    int n = mImage.width() * mImage.height();

    if(sum == 0)
        sum = 1;
    //double ln = log((double)sum/(double)n);
    double ln = log(pow((double)sum / (double)n, -1));

    //double res = pow(ln, -1);
    double res = ln;
    res = res * 2 * alfa * alfa;
    res = pow(res, 0.5);
    res += gMin;

    if(res > 255)
        res = 255;
    if(res < 0)
        res = 0;

    qDebug()<<"res: "<<res;
    return res;
}

void PhotoWindow::qualityCheck()
{
	QString fileUrl = QFileDialog::getOpenFileName(this, tr("Wybierz plik do otwarcia: "), "photo.jpg");
	if (fileUrl.isEmpty()) {
		return;
	}

	QImage ref(fileUrl);
	QualityChecker q;
	if (q(ref, mImage)) {
		qDebug() << "mse:" << q.mse << ", snr:" << q.snr;
	}
}

QVector<int> PhotoWindow::findMaxValues(){
    QVector<int> max;
    max.resize(4);
    max.fill(0);

    for(int i=0; i<krgb.at(0).size(); i++){
        if(krgb.at(0).at(i) > max.at(0))
            max[0] = krgb.at(0).at(i);
        if(krgb.at(1).at(i) > max.at(1))
            max[1] = krgb.at(1).at(i);
        if(krgb.at(2).at(i) > max.at(2))
            max[2] = krgb.at(2).at(i);
        if(krgb.at(3).at(i) > max.at(3))
            max[3] = krgb.at(3).at(i);
    }

    return max;
}

void PhotoWindow::changeImageColor(int color, int oldValue, int newValue, QImage *img){
    switch(color){
    case 0:
        for(int i=0; i < img->width(); i++){
            for(int j=0; j<img->height(); i++){
                if(qGray(img->pixel(i,j)) == oldValue)
                    img->setPixel(i,j, newValue);
            }
        }
        break;
    case 1:
        for(int i=0; i < img->width(); i++){
            for(int j=0; j<img->height(); i++){
                if(QColor(img->pixel(i,j)).red() == oldValue)
                    img->setPixel(i,j, newValue);
            }
        }
        break;
    case 2:
        for(int i=0; i < img->width(); i++){
            for(int j=0; j<img->height(); i++){
                if(QColor(img->pixel(i,j)).green() == oldValue)
                    img->setPixel(i,j, newValue);
            }
        }
        break;
    case 3:
        for(int i=0; i < img->width(); i++){
            for(int j=0; j<img->height(); i++){
                if(QColor(img->pixel(i,j)).blue() == oldValue)
                    img->setPixel(i,j, newValue);
            }
        }
        break;
    }
}
