


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



}

void MapPanel::aircraft_selected(XAero aero)
{
    qDebug() << "map panel sel";
    this->mapWidget->center_on(aero);
}
