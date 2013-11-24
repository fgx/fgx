

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


#include "xwidgets/toolbargroup.h"

#include "map/mapcorewidget.h"
//#include "map/radarplugin.h"



#include "marble/MarbleModel.h"
#include "marble/PluginInterface.h"


MapCoreWidget::MapCoreWidget(MainObject *mob, QWidget *parent) :
	QWidget(parent)
{

	mainObject = mob;

    //setWindowTitle(tr("MAP CORE"));
    //setWindowIcon(QIcon(":/icon/mpmap"));

    //setProperty("settings_namespace", QVariant("OpenLayerWidget_window"));
    //mainObject->settings->restoreWindow(this);

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
	//connect(txtLat, SIGNAL(textChanged(QString)), this, SLOT(on_coords_changed()));

	toolbarAirports->addWidget(new QLabel(tr("Lon:")));
	txtLon = new QLineEdit();
	toolbarAirports->addWidget(txtLon);
	//connect(txtLon, SIGNAL(textChanged(QString)), this, SLOT(on_coords_changed()));


	toolbarAirports->addWidget(new QLabel(tr("Heading:")));
	spinHeading = new QDoubleSpinBox();
	spinHeading->setRange(0.0, 359.99);
	spinHeading->setSingleStep(0.1);
	toolbarAirports->addWidget(spinHeading);
	//connect(spinHeading, SIGNAL(valueChanged(QString)), this, SLOT(on_coords_changed()));




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
	mainLayout->addLayout(midLayout, 200);


	//====================================================================
	//== Zoom Bar
	//====================================================================
	QVBoxLayout *layoutZoom = new QVBoxLayout();
	midLayout->addLayout(layoutZoom, 0);

	QString zbstyle("font-weight: bold; font-size: 10pt; color: black; padding: 0px; margin: 0px;");

	QToolButton *buttZoomIn = new QToolButton();
	buttZoomIn->setText("+");
	buttZoomIn->setAutoRaise(true);
	buttZoomIn->setStyleSheet(zbstyle);
	layoutZoom->addWidget(buttZoomIn, 0);
	connect(buttZoomIn, SIGNAL(clicked()), this, SLOT(on_butt_zoom_in()));

	sliderZoom = new QSlider();
    //sliderZoom->setRange(1150, 4000);
	sliderZoom->setTickPosition(QSlider::TicksLeft);
	sliderZoom->setTickInterval(1);
	sliderZoom->setPageStep(500);
	layoutZoom->addWidget(sliderZoom, 200);
	connect(sliderZoom, SIGNAL(valueChanged(int)), SLOT(on_slider_zoom(int)));

	QToolButton *buttZoomOut = new QToolButton();
	buttZoomOut->setText("-");
	buttZoomOut->setAutoRaise(true);
	buttZoomOut->setStyleSheet(zbstyle);
	layoutZoom->addWidget(buttZoomOut, 0);
	connect(buttZoomOut, SIGNAL(clicked()), this, SLOT(on_butt_zoom_out()));

	lblZoom = new QLabel("SS");
	lblZoom->setStyleSheet("border: none; font-size: 8pt; margin: 0px; padding: 0px;");
	//sslblZoom->setFixedWidth(80);
	layoutZoom->addWidget(lblZoom);

	//====================================================================
	//== Marble Map
	//====================================================================
    marbleWidget = new XMarbleWidget();
	midLayout->addWidget(marbleWidget, 10);
    marbleWidget->register_flights_model(this->mainObject->flightsModel);


    marbleWidget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
	//marbleWidget->setMapThemeId("earth/bluemarble/bluemarble.dgml");
	//marbleWidget->setMapThemeId("earth/plain/plain.dgml");
    //marbleWidget->setMapThemeId("earth/srtm/srtm.dgml");

    //marbleWidget->setProjection( Marble::Equirectangular );
    marbleWidget->setProjection( Marble::Mercator );


	marbleWidget->setShowCompass(false);
    marbleWidget->setShowClouds( false );
	marbleWidget->setShowBorders( true );
	marbleWidget->setShowOverviewMap(false);
	marbleWidget->setShowScaleBar(true);
    //marbleWidget->setShowElevationModel(true);
	marbleWidget->setShowTerrain(true);
	marbleWidget->setShowLakes(true);
	marbleWidget->setShowRivers(true);
    marbleWidget->setShowGrid(true);
	marbleWidget->setShowCrosshairs(false);

    marbleWidget->setShowCities(false);
	marbleWidget->setShowOtherPlaces(false);
	//marbleWidget->model()->treeModel()->


    //marbleWidget->model()->pluginManager();

	//mapWidget->zoomVie
	//mapWidget->model()->addPlacemarkFile("/home/ffs/fgx/example.kml");

	//mapWidget->model()->addGeoDataFile(QString("/home/ffs/fgx/example.kml"));

	//mapWidget->setMinimumZoom(100);
	//marbleWidget->

    connect(marbleWidget, SIGNAL(zoomChanged(int)),
            this, SLOT(on_map_zoom_changed(int))
    );

	//
    connect(marbleWidget, SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)),
           this, SLOT(on_map_clicked())
    );

    connect(marbleWidget, SIGNAL(visibleLatLonAltBoxChanged(const GeoDataLatLonAltBox &)),
            this, SLOT(on_map_moved(const GeoDataLatLonAltBox &))
    );
	//connect(marbleWidget, SIGNAL(mouseMoveGeoPosition(QString)), this, SLOT(on_map_move(QString)));


	 //QFile file("/home/ffs/fgx/example.kml");
	 //file.open(QIODevice::ReadOnly);
	 //QString xml = file.readAll();
	 //qDebug() << xml;
	 //file.close();
	// mapWidget->model()->addPlacemarkData(xml, "data");
	// mapWidget->model()->addGeoDataString(xml, "data");
	//mapWidget->fileViewModel()->addMap()


    sliderZoom->setRange(marbleWidget->minimumZoom(), marbleWidget->maximumZoom());


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
	/*
	connect(buttonGroupViewLayers,	SIGNAL(buttonClicked(QAbstractButton*)),
			this,					SLOT(on_display_layer(QAbstractButton*))
	); */
	mainObject->settings->beginGroup("map_display_layers");


	chkViewGridLines = new QCheckBox();
	chkViewGridLines->setText("Grid Lines");
	chkViewGridLines->setProperty("layer","grid_lines");
	chkViewGridLines->setChecked(mainObject->settings->value("grid_lines", "0").toBool());
	statusBar->addPermanentWidget(chkViewGridLines);
	buttonGroupViewLayers->addButton(chkViewGridLines);



	chkViewStands = new QCheckBox();
	chkViewStands->setText("Stands");
	chkViewStands->setProperty("layer","stands");
	statusBar->addPermanentWidget(chkViewStands);
	chkViewStands->setChecked(mainObject->settings->value("stands", "1").toBool());
	buttonGroupViewLayers->addButton(chkViewStands);

	chkViewRunwayLabels = new QCheckBox();
	chkViewRunwayLabels->setText("Runway Labels");
	chkViewRunwayLabels->setProperty("layer","runway_labels");
	statusBar->addPermanentWidget(chkViewRunwayLabels);
	chkViewRunwayLabels->setChecked(mainObject->settings->value("runway_labels", "1").toBool());
	buttonGroupViewLayers->addButton(chkViewRunwayLabels);

	chkViewRunwayLines = new QCheckBox();
	chkViewRunwayLines->setText("Runway Lines");
	chkViewRunwayLines->setProperty("layer","runway_lines");
	statusBar->addPermanentWidget(chkViewRunwayLines);
	chkViewRunwayLines->setChecked(mainObject->settings->value("runway_lines", "1").toBool());
	buttonGroupViewLayers->addButton(chkViewRunwayLines);

	mainObject->settings->endGroup();

	buttDebug = new QToolButton();
	buttDebug->setText("Debug");
	buttDebug->setVisible(mainObject->debug_mode == true);
	statusBar->addPermanentWidget(buttDebug);
	//connect(buttDebug, SIGNAL(clicked()), this, SLOT(on_show_debugger()));



	//============================================================================
	//== Main Settings connection
	//connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	//connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));
	//connect(mainObject, SIGNAL(on_debug_mode(bool)), this, SLOT(on_debug_mode(bool)));

    marbleWidget->zoomView(2000);


}


void MapCoreWidget::on_map_clicked()
{
    //qDebug() << "clicked";

}
void MapCoreWidget::on_map_moved(const Marble::GeoDataLatLonAltBox &region)
{
    //qDebug() << "moved";
    this->txtLat->setText( QString::number(region.center().latitude()) );
    this->txtLon->setText( QString::number(region.center().longitude()) );
}


//= Overide the closeEvent
void MapCoreWidget::closeEvent(QCloseEvent *event)
{
	mainObject->settings->saveWindow(this);
	Q_UNUSED(event);
}


//================================================
// Zoom to point
void MapCoreWidget::on_slider_zoom(int zoom)
{
	marbleWidget->zoomView(zoom);

}

//== Zoom in out buttons
void MapCoreWidget::on_butt_zoom_in(){
	sliderZoom->setValue(sliderZoom->value() + 400);
}
void MapCoreWidget::on_butt_zoom_out(){
	sliderZoom->setValue(sliderZoom->value() - 400);
}


void MapCoreWidget::on_map_zoom_changed(int zoom)
{
	//qDebug() << zoom;
	sliderZoom->setValue(zoom);
	lblZoom->setText(QString::number(zoom));
}




void MapCoreWidget::refresh_map()
{
	qDebug() << "refrsh_radar";
	marbleWidget->update();
}



void MapCoreWidget::map_focus(QString lon, QString lat)
{
	marbleWidget->centerOn(lon.toFloat(), lat.toFloat(), true);
	if(marbleWidget->zoom() < 2000){
		marbleWidget->zoomView(2400);
	}
}


