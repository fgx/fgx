

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



	//=============================================
	// Cols Selector
	/*QToolButton *buttShowColumns = new QToolButton(this);
	buttShowColumns->setText("Show");
	buttShowColumns->setIcon(QIcon(":/icon/select_cols"));
	buttShowColumns->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttShowColumns->setPopupMode(QToolButton::InstantPopup);
	toolbar->addWidget(buttShowColumns);

	QMenu *menuCols = new QMenu();
	buttShowColumns->setMenu(menuCols);

	//= Cols Widget
	QWidget *widgetColsSelecta = new QWidget();
	QVBoxLayout *layCols = new QVBoxLayout();
	widgetColsSelecta->setLayout(layCols);


	XGroupVBox *groupBoxRadarCols = new XGroupVBox("Radar Layer- TODO");
	QButtonGroup *groupRadarCols = new QButtonGroup(this);
	groupRadarCols->setExclusive(true);


	QRadioButton *buttShowRadarAll = new QRadioButton();
	buttShowRadarAll->setText("Icons and Labels");
	buttShowRadarAll->setChecked(true);
	groupBoxRadarCols->addWidget(buttShowRadarAll);
	groupRadarCols->addButton(buttShowRadarAll, 1);

	QRadioButton *buttShowRadarImg = new QRadioButton();
	buttShowRadarImg->setText("Icons Only");
	groupBoxRadarCols->addWidget(buttShowRadarImg);
	groupRadarCols->addButton(buttShowRadarImg, 2);

	QRadioButton *buttShowRadarLabels = new QRadioButton();
	buttShowRadarLabels->setText("Labels Only");
	groupBoxRadarCols->addWidget(buttShowRadarLabels);
	groupRadarCols->addButton(buttShowRadarLabels, 3);

	QWidgetAction *colsWidgetAction = new QWidgetAction(this);
	colsWidgetAction->setDefaultWidget(groupBoxRadarCols);
	menuCols->addAction(colsWidgetAction);*/

	//=============================================
	// Map Type
	/*QToolButton *buttMapType = new QToolButton(this);
	buttMapType->setText("Map Type");
	buttMapType->setIcon(QIcon(":/icon/map_type"));
	buttMapType->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttMapType->setPopupMode(QToolButton::InstantPopup);
	toolbar->addWidget(buttMapType);

	QMenu *menuMapType = new QMenu();
	buttMapType->setMenu(menuMapType);

	QActionGroup *actionGroup = new QActionGroup(this);
	actionGroup->setExclusive(true);
	connect(actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(on_map_type(QAction*)));

	QAction *actionMapOsm =menuMapType->addAction("Open Street Map");
	actionMapOsm->setCheckable(true);
	actionMapOsm->setChecked(true);
	actionMapOsm->setProperty("map_type", "osm");
	actionGroup->addAction(actionMapOsm);

	QAction *actionMapGoogle =menuMapType->addAction("Google Satellite");
	actionMapGoogle->setCheckable(true);
	actionMapGoogle->setProperty("map_type", "google_satellite");
	actionGroup->addAction(actionMapGoogle);
	actionMapGoogle->setVisible(false);*/


	//============================================================================
	// Middle pane with map
	//============================================================================
	QHBoxLayout *midLayout = new QHBoxLayout();
	mainLayout->addLayout(midLayout);
	
	sliderZoom = new QSlider();
	sliderZoom->setRange(1,16);
	sliderZoom->setTickPosition(QSlider::TicksLeft);
	sliderZoom->setTickInterval(1);
	sliderZoom->setPageStep(1);
	midLayout->addWidget(sliderZoom);
	connect(sliderZoom, SIGNAL(valueChanged(int)), SLOT(zoom_to(int)));



	//=============================================================
	//== Cache
	
	networkDiskCache = new QNetworkDiskCache(this);
	networkDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
	
	networkCookieJar = new QNetworkCookieJar(this);
	
	//== Browser
	webView = new QWebView();
	midLayout->addWidget(webView,10);
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



//= Overide the closeEvent
void OpenLayerWidget::closeEvent(QCloseEvent *event)
{
	mainObject->settings->saveWindow(this);
	Q_UNUSED(event);
}




//================================================
// Add Runway
void OpenLayerWidget::add_runway(QString apt, QString rwy1, QString rwy2, QString lat1, QString lon1, QString lat2, QString lon2)
{
	QString jstr = QString("add_runway('%1', '%2', '%3', %4, %5, %6, %7);").arg(apt).arg(rwy1).arg(rwy2).arg(lat1).arg(lon1).arg(lat2).arg(lon2);
	execute_js(jstr);
	qDebug() << "add_runway jstr: " << jstr;
}
//================================================
// Add Stand
void OpenLayerWidget::add_stand(QString apt, QString name, QString lat, QString lon)
{
	QString jstr = QString("add_stand('%1', '%2', %3, %4);").arg(apt).arg(name).arg(lat).arg(lon);
	execute_js(jstr);
	//qDebug() << "add_stand jstr: " << jstr;
}




//================================================
// Zoom to Airport
void OpenLayerWidget::zoom_to_airport(QString apt){
	QString jstr = QString("zoom_to_airport('%1');").arg(apt);
	execute_js(jstr);
	//qDebug() << "zoom_to_airport jstr: " << jstr;
}


//================================================
// Zoom to point
void OpenLayerWidget::zoom_to_latlon(QString lat, QString lon, int zoom)
{
	QString jstr = QString("zoom_to_latlon(%1, %2, %3);").arg(lat).arg(lon).arg(zoom);
	execute_js(jstr);
}

//================================================
// Zoom to point
void OpenLayerWidget::zoom_to( int zoom)
{
	QString jstr = QString("zoom_to(%1);").arg(zoom);
	execute_js(jstr);
}



//=================================================
// Show Aircraft - This nukes existing and moves - ONLY ONE ALLOWED
void OpenLayerWidget::show_aircraft(QString callsign, QString lat, QString lon, QString heading, QString altitude){
	QString jstr = QString("show_aircraft('%1', %2, %3, %4, %5);").arg(callsign).arg(lat).arg(lon).arg(heading).arg(altitude);
	execute_js(jstr);
	//qDebug() << "show_aircraft jstr: " << jstr;
}




//================================================
// display_radar
void OpenLayerWidget::display_radar_layer( int viz)
{
	QString jstr = QString("display_radar_layer(%1);").arg(viz);
	execute_js(jstr);
	qDebug() << jstr;
}

//=================================================
// Show Aircraft Radar - this allows more than one aircraft.
void OpenLayerWidget::show_radar(QString callsign, QString lat, QString lon, QString heading, QString altitude){
	QString jstr = QString("show_radar('%1', %2, %3, %4, %5);").arg(callsign).arg(lat).arg(lon).arg(heading).arg(altitude);
	execute_js(jstr);
	//qDebug() << "show_radar jstr: " << jstr;
}

//=================================================
// Focus Aircraft
void OpenLayerWidget::focus_aircraft(QString callsign){
	QString jstr = QString("focus_aircraft('%1');").arg(callsign);
	execute_js(jstr);
}


//================================================
// Execute Javascript
//================================================
void OpenLayerWidget::execute_js(QString js_str){
	if(mainObject->debug_mode){
		qDebug() << "> js= " << js_str;
	}
	webView->page()->mainFrame()->evaluateJavaScript(js_str);
}





//===========================================================================
// Called from JS
//===========================================================================
void OpenLayerWidget::map_debug(QVariant mess){
	qDebug() << "< " << mess.toString();
}


//= < JS - map_mouse_move
void OpenLayerWidget::map_mouse_move(QVariant lat, QVariant lon){
	lblLat->setText(lat.toString());
	lblLon->setText(QString::number(lon.toFloat()));
}


//= < JS - map_click()
void OpenLayerWidget::map_click(QVariant lat, QVariant lon){
	Q_UNUSED(lat);
	Q_UNUSED(lon);
	qDebug() << "map_click()" << lat << lon;
}

//= < JS - map_right_click()
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



//== < JS - Map zoom changed
void OpenLayerWidget::map_zoom_changed(QVariant zoom){
	int z = zoom.toInt();
	if(z == 0){
		return; //leave as is
	}
	sliderZoom->setValue(z);
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
