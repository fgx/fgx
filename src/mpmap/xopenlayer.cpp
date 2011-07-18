

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
	

	QToolBar *toolbar = new QToolBar();
	mainLayout->addWidget(toolbar);


	//== Lat Lng Labels
	toolbar->addWidget(new QLabel("Lat:"));
	lblLat = new QLabel();
	//lblLat.setStyleSheet(style)
	lblLat->setFixedWidth(80);
	lblLat->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	toolbar->addWidget(lblLat);

	//QLabel *lblLng = QLabel();
	//toolbar.addAction("Lng:")
	toolbar->addWidget(new QLabel("Lon:"));
	lblLon = new QLabel();
	//lblLng->setStyleSheet(style);
	lblLon->setFixedWidth(80);
	toolbar->addWidget(lblLon);



	
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
	
	
	//============================================================
	//== Status Bar
	//============================================================
    statusBar = new QStatusBar(this);
    mainLayout->addWidget(statusBar);
    statusBar->showMessage("Idle");
	
	//== Progress Bar
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    statusBar->addPermanentWidget(progressBar);


	//== Zoooomm
	statusBar->addPermanentWidget(new QLabel("Zoom:"));
	groupZoom = new QButtonGroup(this);
	groupZoom->setExclusive(true);
	connect(groupZoom, SIGNAL(buttonClicked(QAbstractButton*)),
			this, SLOT(on_zoom_action(QAbstractButton*))
	);
	for(int z=1; z < 15; z++){
		QToolButton *act = new QToolButton(this);
		act->setText(QString(" %1 ").arg(z));
		act->setProperty("zoom", QVariant(z));
		act->setCheckable(true);
		act->setAutoRaise(true);
		act->setDown(z == 12);
		statusBar->addPermanentWidget(act);
		groupZoom->addButton(act);
	}




    //*** Initialise
	//init_xmap();
}


void XOpenLayerWidget::init_xmap(){
	return;
	webView->page()->mainFrame()->addToJavaScriptWindowObject("Qt", this);
	QUrl server_url( "/Users/raoulquittarco/Desktop/fgx/fgx/fgx/src/resources/openlayers/fgx-map/fgx-map.html" );
	//QUrl server_url( QDir::currentPath().append("/fgx.app/Contents/Resources/openlayers/fgx-map/fgx-map.html"));
	//connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
	connect(webView, SIGNAL(loadStarted()), this, SLOT(setLatLon()));
	connect(webView, SIGNAL(loadStarted()), this, SLOT(setZoom()));
	connect(webView, SIGNAL(loadStarted()), this, SLOT(addRunway()));
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

void XOpenLayerWidget::addRunway()
{
	
	QVariant rwyLon1 = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var rwyLon1 = 5;");
    QVariant rwyLat1 = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var rwyLat1 = 40;");
	QVariant rwyLon2 = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var rwyLon2 = 6;");
    QVariant rwyLat2 = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var rwyLat2 = 41;");
}







//===========================================================================
// Called from JS
//===========================================================================
void XOpenLayerWidget::map_debug(QVariant mess){
	qDebug() << "< " << mess.toString();
}


//= JS - map_mouse_move
// - this is not firing ;-(
void XOpenLayerWidget::map_mouse_move(QVariant dLat, QVariant dLon){
	//qDebug("YES");
	//qDebug() << "map_mouse_move" << dLat.toString();
	lblLat->setText(dLat.toString());
	lblLon->setText(dLon.toString());
}


//== JS - map_click()
void XOpenLayerWidget::map_click(QVariant lat, QVariant lon){
	Q_UNUSED(lat);
	Q_UNUSED(lon);
	//qDebug() << "map_click()" << lat << lng;
}

//== JS - map_right_click()
void XOpenLayerWidget::map_right_click(QVariant lat, QVariant lon){
	Q_UNUSED(lat);
	Q_UNUSED(lon);
	//qDebug() << "map_right_click()" << lat << lng;
}






void XOpenLayerWidget::map_error(QVariant err){
	//qDebug("map_error()");
	Q_UNUSED(err);
}

void XOpenLayerWidget::marker_clicked(QVariant marker, QVariant mId){
	//qDebug("marker_clicked()");
		Q_UNUSED(marker);
			Q_UNUSED(mId);
}

void XOpenLayerWidget::marker_unselected(QVariant curr_idx, QVariant mLocationId){
	//qDebug("marker_unselected()");
		Q_UNUSED(curr_idx);
			Q_UNUSED(mLocationId);
}



//** JS
void XOpenLayerWidget::map_zoom_changed(QVariant zoom){
	//qDebug() << "map_zoom_changed" << zoom;
	return;
	buttZoom->setText(zoom.toString());
	int zoomInt = zoom.toInt();
	QList<QAbstractButton *> buttons = groupZoom->buttons();
	for (int i = 0; i < buttons.size(); ++i) {
		if (buttons.at(i)->property("zoom").toInt() == zoomInt){
			 buttons.at(i)->setChecked(true);
			 return;
		}
	 }
//    for a in self.groupZoom.actions():
//            checked = a.property("zoom").toString() == new_zoom_level
//            a.setChecked(checked)
	  qDebug("map_zoom_changed()");
}








//============================================================================================
//== Progress Slots
//=============================================================
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
