


#include "mapmainwindow.h"

#include "xobjects/xsettings.h"

//using namespace Marble;


MapMainWindow::MapMainWindow(MainObject *mob, QWidget *parent) :
    QMainWindow(parent)
{

    this->mainObject = mob;

    this->setWindowTitle("FGx - The Utlimate Map ;)");
    this->setWindowIcon(QIcon(":/icon/map_type"));


    this->mapWidget = new MapCoreWidget(this->mainObject);

    //mapWidget->setMapThemeId("earth/bluemarble/bluemarble.dgml");
    //mapWidget->setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
    this->setCentralWidget(this->mapWidget);

    // Enable the cloud cover and enable the country borders
    //mapWidget->setShowClouds( false );
    //mapWidget->setShowBorders( true );

    // Hide the FloatItems: Compass and StatusBar
    //mapWidget->setShowOverviewMap(true);
    //mapWidget->setShowScaleBar(true);
    //mapWidget->setShowCompass(true);


    //================================================================================
    //= Pilots widget
    //================================================================================
    this->dockPilots = new QDockWidget();
    this->addDockWidget(Qt::RightDockWidgetArea, dockPilots);

    this->flightsWidget = new FlightsWidget(mainObject);
    dockPilots->setWidget(this->flightsWidget);
    //this->flightsWidget->set_map_widget(mapWidget);


    XSettings::autosizeWindow(this, 100, 100);
}
