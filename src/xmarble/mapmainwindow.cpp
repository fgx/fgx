


#include "mapmainwindow.h"

#include "xobjects/xsettings.h"

//using namespace Marble;


MapMainWindow::MapMainWindow(MainObject *mob, QWidget *parent) :
    QMainWindow(parent)
{

    this->mainObject = mob;


    //this->flightsModel = new FlightsModel(this);

    this->setWindowTitle("FGx - The Utlimate Map ;)");
    this->setWindowIcon(QIcon(":/icon/map_type"));


    this->tabWidget = new QTabWidget();
    this->setCentralWidget(this->tabWidget);



    MapPanel *mapPanel = new MapPanel(this->mainObject);

    //mapWidget->setMapThemeId("earth/bluemarble/bluemarble.dgml");
    //mapWidget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
    //this->setCentralWidget(this->mapWidget);
    this->tabWidget->addTab(mapPanel, QIcon(":/icon/map_type"), "MAP");

    // Enable the cloud cover and enable the country borders
    //mapWidget->setShowClouds( false );
    //mapWidget->setShowBorders( true );

    // Hide the FloatItems: Compass and StatusBar
    //mapWidget->setShowOverviewMap(true);
    //mapWidget->setShowScaleBar(true);
    //mapWidget->setShowCompass(true);



    XSettings::autosizeWindow(this, 100, 200);

    this->mainObject->flightsModel->timer->start();
}
