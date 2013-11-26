
#include <QTimer>
#include <QByteArray>
#include <QFile>
#include <QStringList>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QAction>
#include <QActionGroup>

#include <QToolButton>
#include <QMenu>
#include <QLabel>
#include <QSplitter>

#include <QInputDialog>



#include "xwidgets/toolbargroup.h"

#include "xmarble/mapcorewidget.h"

#include "marble/MarbleModel.h"
#include "marble/PluginInterface.h"


const QString MapCoreWidget::SETTINGS_TAG = QString("map_views");


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
    mainLayout->setSpacing(2);

    //===========
    QHBoxLayout *topBar = new QHBoxLayout();
    mainLayout->addLayout(topBar);

    //===================================================
    //= Map Base Layers and Projection
    ToolBarGroup *tbBaseLayer = new ToolBarGroup(this);
    topBar->addWidget(tbBaseLayer);
    tbBaseLayer->lblTitle->setText("Base Layer");

    //= Base Layer
    buttBaseLayer = new QToolButton();
    buttBaseLayer->setIcon(QIcon(":/micon/base_layer"));
    buttBaseLayer->setToolTip("Change base layer");
    buttBaseLayer->setPopupMode(QToolButton::InstantPopup);
    tbBaseLayer->addWidget(buttBaseLayer);

    QMenu *menuBase = new QMenu(buttBaseLayer);
    buttBaseLayer->setMenu(menuBase);
    this->actGroupBaseLayer = new QActionGroup(this);
    this->actGroupBaseLayer->setExclusive(true);
    connect(this->actGroupBaseLayer, SIGNAL(triggered(QAction*)),
            this, SLOT(on_map_base_layer_action(QAction*))
    );


    QAction *a;
    a = menuBase->addAction("OSM");
    a->setProperty("theme", "earth/openstreetmap/openstreetmap.dgml");
    a->setCheckable(true);
    a->setChecked(true);
    this->actGroupBaseLayer->addAction(a);

    a = menuBase->addAction("Blue Marble");
    a->setProperty("theme", "earth/bluemarble/bluemarble.dgml");
    a->setCheckable(true);
    this->actGroupBaseLayer->addAction(a);

    a = menuBase->addAction("SRTM");
    a->setProperty("theme", "earth/srtm/srtm.dgml");
    a->setCheckable(true);
    this->actGroupBaseLayer->addAction(a);

    a = menuBase->addAction("Plain");
    a->setProperty("theme", "earth/plain/plain.dgml");
    a->setCheckable(true);
    this->actGroupBaseLayer->addAction(a);



    //= Projection
    this->buttProjection = new QToolButton();
    this->buttProjection->setIcon(QIcon(":/micon/projection"));
    this->buttProjection->setToolTip("Change projection");
    this->buttProjection->setPopupMode(QToolButton::InstantPopup);
    tbBaseLayer->addWidget(this->buttProjection);

    QMenu *menuProjection = new QMenu(this->buttProjection);
    this->buttProjection->setMenu(menuProjection);

    this->actGroupProjection = new QActionGroup(this);
    this->actGroupProjection->setExclusive(true);
    connect(this->actGroupProjection, SIGNAL(triggered(QAction*)),
            this, SLOT(on_map_projection_action(QAction*))
    );

    a = menuProjection->addAction("Mercator");
    a->setProperty("proj", Marble::Mercator );
    a->setCheckable(true);
    a->setChecked(true);
    this->actGroupProjection->addAction(a);

    a = menuProjection->addAction("Equirectangular");
    a->setProperty("proj", Marble::Equirectangular );
    a->setCheckable(true);
    this->actGroupProjection->addAction(a);

    a = menuProjection->addAction("Spherical");
    a->setProperty("proj", Marble::Spherical );
    a->setCheckable(true);
    this->actGroupProjection->addAction(a);

    //===================================================
    //= Map View
    ToolBarGroup *tbView = new ToolBarGroup(this);
    topBar->addWidget(tbView);
    tbView->lblTitle->setText("Load / Save");

    // Load view
    QToolButton *buttKSFO = new QToolButton();
    tbView->addWidget(buttKSFO);
    buttKSFO->setIcon(QIcon(":/micon/home"));
    buttKSFO->setToolTip("Center KSFO");
    connect(buttKSFO, SIGNAL(clicked()),
            this, SLOT(center_ksfo())
    );

    // Load view
    this->buttLoadView = new QToolButton();
    this->buttLoadView->setIcon(QIcon(":/micon/load"));
    this->buttLoadView->setToolTip("Load view");
    this->buttLoadView->setPopupMode(QToolButton::InstantPopup);

    QMenu *menu = new QMenu(this->buttLoadView);
    this->buttLoadView->setMenu(menu);
    tbView->addWidget(this->buttLoadView);

    this->actGroupMapViews = new QActionGroup(this);
    this->actGroupMapViews->setExclusive(false);
    connect(this->actGroupMapViews, SIGNAL(triggered(QAction*)),
            this, SLOT(on_map_view_action(QAction*))
    );


    // Save View
    this->buttSaveView = new QToolButton();
    this->buttSaveView->setIcon(QIcon(":/micon/save"));
    this->buttSaveView->setToolTip("Save current view");
    tbView->addWidget(this->buttSaveView);
    connect(this->buttSaveView, SIGNAL(clicked()),
            this, SLOT(on_save_view())
    );

    topBar->addStretch(20);


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
    marbleWidget->setProjection( Marble::Mercator );

    marbleWidget->setShowGrid(false);
    marbleWidget->setShowCrosshairs(false);
	marbleWidget->setShowCompass(false);
    marbleWidget->setShowClouds( false );
    marbleWidget->setShowAtmosphere(false);
	marbleWidget->setShowOverviewMap(false);
	marbleWidget->setShowScaleBar(true);

    //marbleWidget->setShowElevationModel(true);

	marbleWidget->setShowTerrain(true);
    marbleWidget->setShowIceLayer(true);
    marbleWidget->setShowRelief(true);
	marbleWidget->setShowLakes(true);
	marbleWidget->setShowRivers(true);
    marbleWidget->setShowBorders( true );




    marbleWidget->setShowCityLights(false);
    marbleWidget->setShowCities(false);
    marbleWidget->setShowOtherPlaces(false);
	//marbleWidget->model()->treeModel()->


    this->marbleWidget->setCenterLatitude(37.638);
    this->marbleWidget->setCenterLongitude(-122.215);
    this->marbleWidget->setZoom(2000);
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


    //------------------------------------------
    // Test placemark
    //GeoDataPlacemark *pm = new GeoDataPlacemark( "YES" );
    //pm->setCoordinate(30.523333, 50.45,0,  GeoDataCoordinates::Degree);


    // Create Model
    this->docFlights = new GeoDataDocument();
    //this->docFlights->append(pm); // I can se this <<

    this->marbleWidget->model()->treeModel()->addDocument( this->docFlights );

    //------------------------------------------

    connect(this->mainObject->flightsModel, SIGNAL(update_done()),
            this, SLOT(update_flights())
    );


    sliderZoom->setRange(marbleWidget->minimumZoom(), marbleWidget->maximumZoom());

    //================
    //= debug tree
    /*
    QTreeView *treeDebug = new QTreeView();
    midLayout->addWidget(treeDebug);
    treeDebug->setModel(this->marbleWidget->model()->treeModel());
    */

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



    this->load_views();
   // QTimer::singleShot(1000, this, SLOT(center_ksfo()));


}

void MapCoreWidget::center_ksfo()
{
    //= Center on SF Bay
    this->marbleWidget->setCenterLatitude(37.638);
    this->marbleWidget->setCenterLongitude(-122.215);
    this->marbleWidget->setZoom(2000);
    this->marbleWidget->update();
}

void MapCoreWidget::on_map_clicked()
{
    //qDebug() << "clicked";


}
void MapCoreWidget::on_map_moved(const Marble::GeoDataLatLonAltBox &region)
{
    //qDebug() << "moved";
    this->txtLat->setText( QString::number(region.center().latitude(GeoDataCoordinates::Degree)) );
    this->txtLon->setText( QString::number(region.center().longitude(GeoDataCoordinates::Degree)) );
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

//=====================================================
void MapCoreWidget::on_save_view()
{

    bool ok;
    QString txt = QInputDialog::getText(this, QString("Save Map View"),
                                        QString("Enter Label:"), QLineEdit::Normal,
                                        QString(""), &ok);
    if (ok && !txt.isEmpty() ){

        QSettings settings;

        // retrive current index
        int size = settings.beginReadArray( MapCoreWidget::SETTINGS_TAG );
        qDebug() << "write " << size;
        settings.endArray();

        // Write view
        settings.beginWriteArray( MapCoreWidget::SETTINGS_TAG );
            settings.setArrayIndex(size);
            settings.setValue("label", txt);
            settings.setValue("lat", this->marbleWidget->centerLatitude());
            settings.setValue("lon", this->marbleWidget->centerLongitude());
            settings.setValue("zoom", this->marbleWidget->zoom());
        settings.endArray();
    }
}

void MapCoreWidget::on_load_view()
{

}

void MapCoreWidget::load_views()
{

    QMenu *menu = this->buttLoadView->menu();

    QSettings settings;
    int size = settings.beginReadArray( MapCoreWidget::SETTINGS_TAG );
    qDebug() << "load " << size;
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString lbl = settings.value("label").toString();
        if(lbl.isEmpty()){
            lbl.append("- no label -");
        }
        QAction *act = new QAction(menu);
        menu->addAction(act);
        this->actGroupMapViews->addAction(act);
        act->setText(lbl);
        act->setProperty("lat", settings.value("lat"));
        act->setProperty("lon", settings.value("lon"));
        act->setProperty("zoom", settings.value("zoom"));
        //qDebug() << "-----------";
        //qDebug() << settings.value("label").toString();
       // qDebug() << settings.value("lat").toReal();
        //qDebug() << settings.value("lon").toReal();

    }
    settings.endArray();
}

void MapCoreWidget::on_map_view_action(QAction *act)
{
    this->marbleWidget->setCenterLatitude(act->property("lat").toReal());
    this->marbleWidget->setCenterLongitude(act->property("lon").toReal());
    this->marbleWidget->setZoom(act->property("zoom").toInt());
}

void MapCoreWidget::on_map_base_layer_action(QAction *act)
{
    //qDebug() << "base" << act->property("theme").toString();
    this->marbleWidget->setMapThemeId( act->property("theme").toString() );
}

void MapCoreWidget::on_map_projection_action(QAction *act)
{
    //qDebug() << "on_map_projection_action" << act->property("proj").toInt();
    this->marbleWidget->setProjection( act->property("proj").toInt() );
}


void MapCoreWidget::update_flights()
{
    return;

   qDebug() << "update flights";

   // Createing a new doucment and adding here works
   //GeoDataDocument *doco = new GeoDataDocument();

   for(int idx=0; idx < this->mainObject->flightsModel->rowCount(); idx++)
   {

       qDebug() <<  idx << this->mainObject->flightsModel->item(idx, FlightsModel::C_CALLSIGN)->text() << this->mainObject->flightsModel->item(idx, FlightsModel::C_LON)->text().toFloat() << this->mainObject->flightsModel->item(idx, FlightsModel::C_LAT)->text().toFloat();

       GeoDataPlacemark *pm = new GeoDataPlacemark( this->mainObject->flightsModel->item(idx, FlightsModel::C_CALLSIGN)->text() );
       pm->setCoordinate(this->mainObject->flightsModel->item(idx, FlightsModel::C_LON)->text().toFloat(),
                         this->mainObject->flightsModel->item(idx, FlightsModel::C_LAT)->text().toFloat(),
                         0,
                         GeoDataCoordinates::Degree);

       this->docFlights->append(pm);
       //doco->append(pm);
       //this->marbleWidget->model()->treeModel()->updateFeature(pm); << segfaults...


   }
   //this->marbleWidget->model()->treeModel()->addDocument(doco);

   //this->marbleWidget->update(); // Does Nothing aparently

   //this->marbleWidget->model()->treeModel()->update(); //<< segfaults

}
