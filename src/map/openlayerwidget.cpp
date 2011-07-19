

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

#include "map/openlayerwidget.h"




OpenLayerWidget::OpenLayerWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;
	
	setWindowTitle(tr("FGx OpenLayer Map"));
	setWindowIcon(QIcon(":/icon/mpmap"));
	
	setProperty("settings_namespace", QVariant("OpenLayerWidget_window"));
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
	lblLat->setFixedWidth(150);
	lblLat->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	toolbar->addWidget(lblLat);

	//QLabel *lblLng = QLabel();
	//toolbar.addAction("Lng:")
	toolbar->addWidget(new QLabel("Lon:"));
	lblLon = new QLabel();
	//lblLng->setStyleSheet(style);
	lblLon->setFixedWidth(150);
	toolbar->addWidget(lblLon);



	
	//=============================================================
	//== Cache
	
	networkDiskCache = new QNetworkDiskCache(this);
	networkDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
	
	networkCookieJar = new QNetworkCookieJar(this);
	
	//== Browser
	webView = new QWebView();
	mainLayout->addWidget(webView,10);
	webView->page()->networkAccessManager()->setCache(networkDiskCache);
	//webView->page()->networkAccessManager()->setCookieJar(networkCookieJar);
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
	//connect(groupZoom, SIGNAL(buttonClicked(QAbstractButton*)),
	//		this, SLOT(on_zoom_action(QAbstractButton*))
	//);
	/*
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
	*/



	//=== Initialise
	init_map();
}



//===========================================================================
//== Initialisaztion
//===========================================================================
void OpenLayerWidget::init_map(){

	//static bool map_initialized = false;
	//if(map_initialized == false){
		//= Read file if in dev_mode() - no need to "recompile" the resource file
		QFile file(
						QFile::exists("/home/ffs/SPETE_PC.txt")
						? "/home/ffs/fgx/src/resources/openlayers/map.html"
						: ":/openlayers/map.html"
					);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			qDebug() << "MAP: could not open file" << file.fileName();
			return;
		}


		QByteArray contents = file.readAll();
		//qDebug() << "contents OK";
		//webView->setHtml("<html><head><title>FOO</title></head><body><h1>HELP</h1><h1>HELP</h1><h1>HELP</h1><h1>HELP</h1></body></html>");
		webView->setHtml(contents, QUrl("qrc:///"));
		webView->page()->mainFrame()->addToJavaScriptWindowObject("Qt", this);
		//qDebug() << webView->title();

	//}
	//map_initialized = true;
}



//** Overide the closeEvent
void OpenLayerWidget::closeEvent(QCloseEvent *event)
{
	mainObject->settings->saveWindow(this);
	Q_UNUSED(event);
}

void OpenLayerWidget::loadFinished(bool)
{
    QVariant jsReturn = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("testFunction('loadFinished testFunction with QVariant jsReturn is working.')");
    qDebug() << jsReturn.toString();
}

void OpenLayerWidget::setLatLon()
{
	QVariant lon = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var lon = 5;");
    QVariant lat = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var lat = 40;");
}

void OpenLayerWidget::setZoom()
{
    QVariant zoom = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("var zoom = 5;");
}



//================================================
// Add Runway
void OpenLayerWidget::add_runway(QString apt, QString lat1, QString lon1, QString lat2, QString lon2)
{
	QString jstr = QString("add_runway('%1', %2, %3, %4, %5);").arg(apt).arg(lat1).arg(lon1).arg(lat2).arg(lon2);
	execute_js(jstr);
}
//================================================
// Zoom Airport
void OpenLayerWidget::zoom_to_airport(QString apt){
	QString jstr = QString("zoom_to_airport('%1');").arg(apt);
	execute_js(jstr);
}

//=================================================
// Show Aircraft
void OpenLayerWidget::show_aircraft(QString callsign, QString lat, QString lon, QString heading, QString altitude){
	QString jstr = QString("show_aircraft('%1', %2, %3, %4, %5);").arg(callsign).arg(lat).arg(lon).arg(heading).arg(altitude);
	execute_js(jstr);
}
//=================================================
// Show Aircraft
void OpenLayerWidget::focus_aircraft(QString callsign){
	QString jstr = QString("focus_aircraft('%1');").arg(callsign);
	execute_js(jstr);
}


//================================================
// Execute Javascript
//================================================
void OpenLayerWidget::execute_js(QString js_str){
	qDebug() << "> js= " << js_str;
	webView->page()->mainFrame()->evaluateJavaScript(js_str);
}





//===========================================================================
// Called from JS
//===========================================================================
void OpenLayerWidget::map_debug(QVariant mess){
	qDebug() << "< " << mess.toString();
}


//= JS - map_mouse_move
// - this is not firing ;-(
void OpenLayerWidget::map_mouse_move(QVariant lat, QVariant lon){
	lblLat->setText(lat.toString());
	lblLon->setText(QString::number(lon.toFloat()));
}


//== JS - map_click()
void OpenLayerWidget::map_click(QVariant lat, QVariant lon){
	Q_UNUSED(lat);
	Q_UNUSED(lon);
	qDebug() << "map_click()" << lat << lon;
}

//== JS - map_right_click()
void OpenLayerWidget::map_right_click(QVariant lat, QVariant lon){
	Q_UNUSED(lat);
	Q_UNUSED(lon);
	//qDebug() << "map_right_click()" << lat << lng;
}






void OpenLayerWidget::map_error(QVariant err){
	//qDebug("map_error()");
	Q_UNUSED(err);
}

void OpenLayerWidget::marker_clicked(QVariant marker, QVariant mId){
	//qDebug("marker_clicked()");
		Q_UNUSED(marker);
			Q_UNUSED(mId);
}

void OpenLayerWidget::marker_unselected(QVariant curr_idx, QVariant mLocationId){
	//qDebug("marker_unselected()");
		Q_UNUSED(curr_idx);
			Q_UNUSED(mLocationId);
}



//** JS
void OpenLayerWidget::map_zoom_changed(QVariant zoom){
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
void OpenLayerWidget::start_progress(){
	progressBar->setVisible(true);
}

void OpenLayerWidget::update_progress(int v){
	progressBar->setValue(v);
}
void OpenLayerWidget::end_progress(bool Ok){
	Q_UNUSED(Ok);
	progressBar->setVisible(false);
	statusBar->showMessage( webView->url().toString() );
}
