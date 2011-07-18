

#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QStringList>


#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QToolButton>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QSplitter>

#include <QtWebKit/QWebFrame>
#include <QtGui/QDesktopServices>

#include "mpmap/xopenlayer.h"




XOpenLayerWidget::XOpenLayerWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;
	
	setWindowTitle(tr("FGx OpenLayer Map"));
	setWindowIcon(QIcon(":/icon/mpmap"));
	
	setProperty("settings_namespace", QVariant("xopenlayerwidget_window"));
	mainObject->settings->restoreWindow(this);

	//======================================================
	//= Main Layout and Splitter
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
	
	QLabel *label_notice = new QLabel();
	label_notice->setText("Experimental OpenLayer Widget");
	label_notice->setStyleSheet("background-color: #eeeedd; color: #000099; padding: 5px;");
	mainLayout->addWidget(label_notice, 0);
	

	
	//=============================================================
	//== Cache
	
	networkDiskCache = new QNetworkDiskCache(this);
	networkDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
	
	networkCookieJar = new QNetworkCookieJar(this);
	
	//== Browser
	webView = new QWebView();
	mainLayout->addWidget(webView, 500);
	webView->page()->networkAccessManager()->setCache(networkDiskCache);
	webView->page()->networkAccessManager()->setCookieJar(networkCookieJar);
	connect(webView, SIGNAL(loadStarted()), this, SLOT(start_progress()));
	connect(webView, SIGNAL(loadProgress(int)), this, SLOT(update_progress(int)));
	connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(end_progress(bool)));
	
	
    //*** Status Bar
    statusBar = new QStatusBar(this);
    mainLayout->addWidget(statusBar);
    statusBar->showMessage("Idle");
	
    //** Progress Bar
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    statusBar->addPermanentWidget(progressBar);
	
    //*** Initialise
	init_xmap();
}

//** Progress Slots
void XOpenLayerWidget::start_progress(){
    progressBar->setVisible(true);
}

void XOpenLayerWidget::update_progress(int v){
    progressBar->setValue(v);
}
void XOpenLayerWidget::end_progress(bool Ok){
	Q_UNUSED(Ok);
    progressBar->setVisible(false);
	statusBar->showMessage( webView->url().toString() );
}


void XOpenLayerWidget::init_xmap(){
	webView->page()->mainFrame()->addToJavaScriptWindowObject("Qt", this);
	//QUrl server_url( "/Users/raoulquittarco/Desktop/fgx/fgx/fgx/src/resources/openlayers/fgx-map/fgx-map.html" );
	QUrl server_url( ":/map/fgx-map" );
	connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	connect(webView, SIGNAL(loadStarted()), this, SLOT(setLatLon()));
	connect(webView, SIGNAL(loadStarted()), this, SLOT(setZoom()));
	webView->load( server_url );
	statusBar->showMessage(QString("Loading: ").append( server_url.toString()) );
}

//** Overide the closeEvent
void XOpenLayerWidget::closeEvent(QCloseEvent *event)
{
	mainObject->settings->saveWindow(this);
	Q_UNUSED(event);
}

void XOpenLayerWidget::loadFinished(bool)
{
    QVariant jsReturn = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("testFunction('loadFinished testFunction with QVariant jsReturn is working.')");
    qDebug() << jsReturn.toString();
}

void XOpenLayerWidget::setLatLon()
{
	QVariant lon = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var lon = 5;");
    QVariant lat = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var lat = 40;");
}

void XOpenLayerWidget::setZoom()
{
    QVariant zoom = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var zoom = 5;");
}
