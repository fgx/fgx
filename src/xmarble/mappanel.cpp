


#include "mappanel.h"



MapPanel::MapPanel(MainObject *mob, QWidget *parent) :
    QMainWindow(parent)
{

    this->mainObject = mob;


    //=======================================================
    //= Map Widget in center
    this->mapWidget = new MapCoreWidget(this->mainObject);
    this->setCentralWidget(this->mapWidget);



    //================================================================================
    //= Dock Panel with Tabs
    //================================================================================
    this->docker = new QDockWidget();
    this->addDockWidget(Qt::RightDockWidgetArea, this->docker );
    this->docker->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    this->docker->setFloating(false);

    this->tabWidget = new QTabWidget();
    this->docker->setWidget(this->tabWidget);

    this->tabWidget->setMinimumWidth(400);

    //================================================================================
    //= Flights widget
    this->flightsWidget = new FlightsWidget(mainObject);
    this->tabWidget->addTab(this->flightsWidget, QIcon(":/micon/flights"), "Flights");

    this->connect(this->flightsWidget, SIGNAL(aircraft_selected(XAero)),
                  this, SLOT(aircraft_selected(XAero))
    );


    //================================================================================
    //= Navdata widget
    this->navDataWidget = new NavDataWidget();
    this->tabWidget->addTab(this->navDataWidget, QIcon(":/micon/navdata"), "Nav Data");


    //================================================================================
    //= Map Model Debug
    QTreeView *treeDebug = new QTreeView();
    this->tabWidget->addTab(treeDebug, QIcon(":/micon/navdata"), "Map Debug");
    treeDebug->setModel( this->mapWidget->marbleWidget->model()->treeModel() );


    //================================================================================
    //= Dock for Small Map

    this->dockSmall = new QDockWidget();
    this->dockSmall->setMaximumHeight(300);
    this->addDockWidget(Qt::RightDockWidgetArea, this->dockSmall );
    this->dockSmall->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    this->dockSmall->setFloating(false);

    //= Small Map
    this->mapSmall = new MarbleWidget();
    this->dockSmall->setWidget(this->mapSmall);
    this->mapSmall->setMapThemeId("earth/plain/plain.dgml");
    this->mapSmall->setProjection( Marble::Mercator );

    this->mapSmall->setShowGrid(false);
    this->mapSmall->setShowCompass(false);
    this->mapSmall->setShowCities(false);
    this->mapSmall->setShowCrosshairs(false);
    this->mapSmall->setShowOverviewMap(false);
    this->mapSmall->setShowScaleBar(false);

    this->mapSmall->floatItem("navigation")->hide();

    this->mapSmall->setShowBorders(false);
    this->mapSmall->setShowRelief(true);

    this->mapSmall->setShowPlaces(false);
    this->mapSmall->setShowCities(false);
    this->mapSmall->setShowOtherPlaces(false);




    this->mapSmall->setShowBackground(false);

    this->mapSmall->zoomView(400);


    this->flightsMiniLayer = new FlightsPaintLayer(this->mapSmall, FlightsPaintLayer::MINI_MODE);
    this->mapSmall->addLayer(this->flightsMiniLayer);
    this->mapSmall->installEventFilter(this->flightsMiniLayer);
    // TODO - there's got to be a more elegant way to do this
    this->flightsMiniLayer->register_flights_model(this->mainObject->flightsModel);

}

void MapPanel::aircraft_selected(XAero aero)
{
    this->mapWidget->center_on(aero);
}
