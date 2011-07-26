

#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QStringList>


#include <QtGui/QVBoxLayout>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QToolButton>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QSplitter>


#include <QtWebKit/QWebFrame>
#include <QtGui/QDesktopServices>



#include "map/openlayerwidget.h"
#include "airports/airportswidget.h"




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
	


	toolbarAirports = new QToolBar();
	mainLayout->addWidget(toolbarAirports);
	
	toolbarAirports->addWidget(new QLabel(tr("Lat:")));
	txtLat = new QLineEdit();
	toolbarAirports->addWidget(txtLat);
	connect(txtLat, SIGNAL(textChanged(QString)), this, SLOT(on_coords_changed()));
	
	toolbarAirports->addWidget(new QLabel(tr("Lon:")));
	txtLon = new QLineEdit();
	toolbarAirports->addWidget(txtLon);
	connect(txtLon, SIGNAL(textChanged(QString)), this, SLOT(on_coords_changed()));
	
	
	toolbarAirports->addWidget(new QLabel(tr("Heading:")));
	spinHeading = new QDoubleSpinBox();
	spinHeading->setRange(0.0, 359.99);
	spinHeading->setSingleStep(0.1);
	toolbarAirports->addWidget(spinHeading);
	connect(spinHeading, SIGNAL(valueChanged(QString)), this, SLOT(on_coords_changed()));
	
	


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
	
	//== Zoom Bar
	QVBoxLayout *layoutZoom = new QVBoxLayout();
	midLayout->addLayout(layoutZoom, 0);

	QString zbstyle("font-weight: bold; font-size: 10pt; color: black; padding: 0px; margin: 0px;");

	QToolButton *buttZoomIn = new QToolButton();
	buttZoomIn->setText("+");
	buttZoomIn->setAutoRaise(true);
	buttZoomIn->setStyleSheet(zbstyle);
	layoutZoom->addWidget(buttZoomIn, 0);
	connect(buttZoomIn, SIGNAL(clicked()), this, SLOT(on_zoom_in()));

	sliderZoom = new QSlider();
	sliderZoom->setRange(1,16);
	sliderZoom->setTickPosition(QSlider::TicksLeft);
	sliderZoom->setTickInterval(1);
	sliderZoom->setPageStep(1);
	layoutZoom->addWidget(sliderZoom, 200);
	connect(sliderZoom, SIGNAL(valueChanged(int)), SLOT(zoom_to(int)));

	QToolButton *buttZoomOut = new QToolButton();
	buttZoomOut->setText("-");
	buttZoomOut->setAutoRaise(true);
	buttZoomOut->setStyleSheet(zbstyle);
	layoutZoom->addWidget(buttZoomOut, 0);
	connect(buttZoomOut, SIGNAL(clicked()), this, SLOT(on_zoom_out()));

	
	lblZoom = new QLabel();
	lblZoom->setFixedWidth(20);
	lblZoom->setStyleSheet(zbstyle);
	layoutZoom->addWidget(lblZoom);
	
	

	//=============================================================
	//== Cache
	networkDiskCache = new QNetworkDiskCache(this);
	networkDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
	
	
	//== Browser
	webView = new QWebView();
	midLayout->addWidget(webView, 10);
	webView->page()->networkAccessManager()->setCache(networkDiskCache);

	webInspector = new QWebInspector();
	webInspector->hide();


	connect(webView, SIGNAL(loadStarted()), this, SLOT(start_progress()));
	connect(webView, SIGNAL(loadProgress(int)), this, SLOT(update_progress(int)));
	connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(end_progress(bool)));
	
	
	//============================================================
	//== Status Bar
	//============================================================
    statusBar = new QStatusBar(this);
    mainLayout->addWidget(statusBar);
    statusBar->showMessage("Idle");
	statusBar->setSizeGripEnabled(false);
	
	//== Progress Bar
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    statusBar->addPermanentWidget(progressBar);



	//===================================================
	//= View Layers
	buttonGroupViewLayers = new QButtonGroup(this);
	buttonGroupViewLayers->setExclusive(false);
	connect(buttonGroupViewLayers,	SIGNAL(buttonClicked(QAbstractButton*)),
			this,					SLOT(on_display_layer(QAbstractButton*))
	);
	mainObject->settings->beginGroup("map_display_layers");


	/*chkViewGridLines = new QCheckBox();
	chkViewGridLines->setText("Grid Lines");
	chkViewGridLines->setProperty("layer","grid_lines");
	chkViewGridLines->setChecked(mainObject->settings->value("grid_lines", "0").toBool());
	statusBar->addPermanentWidget(chkViewGridLines);
	buttonGroupViewLayers->addButton(chkViewGridLines);*/



	/*chkViewStands = new QCheckBox();
	chkViewStands->setText("Stands");
	chkViewStands->setProperty("layer","stands");
	statusBar->addPermanentWidget(chkViewStands);
	chkViewStands->setChecked(mainObject->settings->value("stands", "1").toBool());
	buttonGroupViewLayers->addButton(chkViewStands);*/

	/*chkViewRunwayLabels = new QCheckBox();
	chkViewRunwayLabels->setText("Runway Labels");
	chkViewRunwayLabels->setProperty("layer","runway_labels");
	statusBar->addPermanentWidget(chkViewRunwayLabels);
	chkViewRunwayLabels->setChecked(mainObject->settings->value("runway_labels", "1").toBool());
	buttonGroupViewLayers->addButton(chkViewRunwayLabels);*/

	/*chkViewRunwayLines = new QCheckBox();
	chkViewRunwayLines->setText("Runway Lines");
	chkViewRunwayLines->setProperty("layer","runway_lines");
	statusBar->addPermanentWidget(chkViewRunwayLines);
	chkViewRunwayLines->setChecked(mainObject->settings->value("runway_lines", "1").toBool());
	buttonGroupViewLayers->addButton(chkViewRunwayLines);*/

	mainObject->settings->endGroup();

	buttDebug = new QToolButton();
	buttDebug->setText("Debug");
	buttDebug->setVisible(mainObject->debug_mode == true);
	statusBar->addPermanentWidget(buttDebug);
	connect(buttDebug, SIGNAL(clicked()), this, SLOT(on_show_debugger()));


	
	//============================================================================
	//== Main Settings connection
	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));
	connect(mainObject, SIGNAL(on_debug_mode(bool)), this, SLOT(on_debug_mode(bool)));
}



//===========================================================================
//== Load Map
//===========================================================================
void OpenLayerWidget::load_map(QString m_typ)
{
	this->map_type = m_typ;
	QFile file(":/openlayers/map.html");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		//qDebug() << "MAP: could not open file" << file.fileName();
		return;
	}
	QByteArray contents = file.readAll();
	webView->setHtml(contents, QUrl("qrc:///")); // This prefix does not work with src:///openlayers/ .. help.. Geoff.. gral.?
	webView->page()->mainFrame()->addToJavaScriptWindowObject("Qt", this);

	webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, mainObject->debug_mode);

	webInspector->setPage(webView->page());

	//= Show Hide widgets
	bool is_airport = map_type == "airport";
	//chkViewRunwayLines->setVisible(is_airport);
	//chkViewRunwayLabels->setVisible(is_airport);
	//chkViewStands->setVisible(is_airport);

	toolbarAirports->setVisible(is_airport);

}

void OpenLayerWidget::map_initialised()
{
	QString js_mtype = QString("set_map_type('%1');").arg(map_type);
	execute_js(js_mtype);

	QList<QAbstractButton *> buttons = buttonGroupViewLayers->buttons();
	for(int idx = 0; idx < buttons.size(); idx++){
		QString jstr = QString("display_layer('%1', %2);").arg(	buttons.at(idx)->property("layer").toString()
														).arg(	buttons.at(idx)->isChecked() ? 1 : 0
														);
		execute_js(jstr);
	}

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
	//qDebug() << "add_runway jstr: " << jstr;
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
// Add Tower
void OpenLayerWidget::add_tower(QString name, QString lat, QString lon)
{
	QString jstr = QString("add_tower('%1', %2, %3);").arg(name).arg(lat).arg(lon);
	execute_js(jstr);
	//qDebug() << "add_tower " << jstr;
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
	lblZoom->setText(QString::number(zoom));
}

//== Zoom in out buttons
void OpenLayerWidget::on_zoom_in(){
	sliderZoom->setValue(sliderZoom->value() + 1);
}
void OpenLayerWidget::on_zoom_out(){
	sliderZoom->setValue(sliderZoom->value() - 1);
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
/*void OpenLayerWidget::display_radar_layer( int viz)
{
	QString jstr = QString("display_radar_layer(%1);").arg(viz);
	execute_js(jstr);
	//qDebug() << jstr;
}*/

//=================================================
// Show Aircraft Radar - this allows more than one aircraft.
void OpenLayerWidget::show_radar(QString callsign, QString lat, QString lon, QString heading, QString altitude, bool is_tower){
	QString jstr;
	if(is_tower){
		add_tower(callsign, lat, lon);
	}else{
		jstr = QString("show_radar('%1', %2, %3, %4, %5);").arg(callsign).arg(lat).arg(lon).arg(heading).arg(altitude);
	}
	execute_js(jstr);
	//qDebug() << "show_radar jstr: " << jstr;
}

void OpenLayerWidget::clear_radar()
{
	execute_js("clear_radar();");
}

//=================================================
// Focus Aircraft
void OpenLayerWidget::focus_aircraft(QString callsign){
	QString jstr = QString("focus_aircraft('%1');").arg(callsign);
	execute_js(jstr);
}

//= TODO later Clear Airport Markers
void OpenLayerWidget::clear_airport(QString apt)
{
	QString jstr = QString("clear_airport('%1');").arg(apt);
	execute_js(jstr);
	qDebug() << "clear prt" << jstr;
}

void OpenLayerWidget::clear_map()
{
	QString jstr = QString("clear_map();");
	execute_js(jstr);
	//qDebug() << "clear map" << jstr;
}

//================================================
// Execute Javascript
//================================================
void OpenLayerWidget::execute_js(QString js_str){
	if(mainObject->debug_mode){
		//qDebug() << "> js= " << js_str;
	}
	webView->page()->mainFrame()->evaluateJavaScript(js_str);
}





//===========================================================================
// Called from JS
//===========================================================================
void OpenLayerWidget::map_debug(QVariant mess){
	//qDebug() << "< " << mess.toString();
	Q_UNUSED(mess);
}


//= < JS - map_mouse_move
/*void OpenLayerWidget::map_mouse_move(QVariant lat, QVariant lon){
	lblLat->setText(lat.toString());
	lblLon->setText(QString::number(lon.toFloat()));
}*/

//= < JS - map_show_coords()
void OpenLayerWidget::map_show_coords(QVariant lat, QVariant lon){
	txtLat->setText(lat.toString());
	txtLon->setText(lon.toString());
}

void OpenLayerWidget::on_coords_changed(){
	if(map_type == "radar"){
		return;
	}
	emit setx("--lat=", true, txtLat->text());
	emit setx("--lon=", true, txtLon->text());
	emit setx("--heading=", true, QString::number(spinHeading->value()) );
	
	show_aircraft(mainObject->X->getx("--callsign="),
							 mainObject->X->getx("--lat="),
							 mainObject->X->getx("--lon="),
							 mainObject->X->getx("--heading="),
							 "0"
							 //mainObject->X->getx("--altitude=") --> this we will have later
							 );
	
}



void OpenLayerWidget::on_upx(QString option, bool enabled, QString value)
{
	Q_UNUSED(option);
	Q_UNUSED(enabled);
	Q_UNUSED(value);
	
	if(option == "--lat="){
		txtLat->setText(value);
		
	}else if(option == "--lon="){
		txtLon->setText(value);
	
	}else if(option == "--heading="){
		spinHeading->setValue(value.toDouble());
	
	}
}


//= < JS - map_click() get click to log
void OpenLayerWidget::map_click(QVariant lat, QVariant lon){
	Q_UNUSED(lat);
	Q_UNUSED(lon);
	//qDebug() << "map_click()" << lat << lon;
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

/*void OpenLayerWidget::on_map_type(QAction *act)
	{
	QString jstr = QString("set_map_type('%1');").arg(act->property("map_type").toString());
	execute_js(jstr);
	
	}*/


void OpenLayerWidget::on_display_layer(QAbstractButton *chkBox)
{
	QString jstr = QString("display_layer('%1', %2);").arg(	chkBox->property("layer").toString()
													).arg(	chkBox->isChecked() ? 1 : 0
													);
	execute_js(jstr);
	mainObject->settings->beginGroup("map_display_layers");
	mainObject->settings->setValue(chkBox->property("layer").toString(), chkBox->isChecked());
	mainObject->settings->endGroup();
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
	

void OpenLayerWidget::on_show_debugger()
{
	webInspector->show();
	webInspector->setMinimumWidth(500);
}


void OpenLayerWidget::on_debug_mode(bool debug_mode)
{
	webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, debug_mode);
	buttDebug->setVisible(debug_mode == true);
}
