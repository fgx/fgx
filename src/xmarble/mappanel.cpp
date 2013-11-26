


#include "mappanel.h"



MapPanel::MapPanel(MainObject *mob, QWidget *parent) :
    QMainWindow(parent)
{

    this->mainObject = mob;


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
    this->docker->setWidget(this->flightsWidget);

    this->flightsWidget->setMinimumWidth(500);



}
