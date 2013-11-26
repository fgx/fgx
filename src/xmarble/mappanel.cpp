


#include "mappanel.h"

#include "xobjects/xsettings.h"

//using namespace Marble;


MapPanel::MapPanel(MainObject *mob, QWidget *parent) :
    QMainWindow(parent)
{

    this->mainObject = mob;


    //this->flightsModel = new FlightsModel(this);

    //this->setWindowTitle("FGx - The Utlimate Map ;)");
    //this->setWindowIcon(QIcon(":/icon/map_type"));


   // this->tabWidget = new QTabWidget();
   // this->setCentralWidget(this->tabWidget);



    this->mapWidget = new MapCoreWidget(this->mainObject);
    this->setCentralWidget(this->mapWidget);



    //================================================================================
    //= Dock Panel with Tabs
    //================================================================================
    this->docker = new QDockWidget();
    this->addDockWidget(Qt::RightDockWidgetArea, this->docker );
    this->docker->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    this->tabWidget = new QTabWidget();
    this->docker->setWidget(this->tabWidget);

    //================================================================================
    //= Flights widget
    this->flightsWidget = new FlightsWidget(mainObject);
    this->docker->setWidget(this->flightsWidget);

    this->flightsWidget->setMinimumWidth(500);

    //this->flightsWidget->set_map_widget(mapWidget);


    //XSettings::autosizeWindow(this, 300, 300);


}
