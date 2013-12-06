#ifndef MAPPANEL_H
#define MAPPANEL_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTabWidget>


#include "marble/MarbleWidget.h"
#include "marble/MarbleModel.h"
#include "marble/GeoDataTreeModel.h"


#include "xobjects/mainobject.h"
class MainObject;

#include "xmarble/mapcorewidget.h"
class MapCoreWidget;

#include "navdata/navdatawidget.h"
class NavDataWidget;

#include "flights/flightswidget.h"
class FlightsWidget;

#include "xmarble/flightspaintlayer.h"
class FlightsPaintLayer;

using namespace Marble;


//================================================

class MapPanel : public QMainWindow
{
    Q_OBJECT
public:
    explicit MapPanel(MainObject*, QWidget *parent = 0);

    MainObject *mainObject;

    MapCoreWidget *mapWidget;
    MarbleWidget *mapSmall;
    //FlightsPaintLayer *flightsLayer;

    QDockWidget *docker;
    QDockWidget *dockSmall;


    QTabWidget *tabWidget;

    NavDataWidget *navDataWidget;
    FlightsWidget *flightsWidget;

    FlightsPaintLayer *flightsMiniLayer;


signals:

public slots:
    void aircraft_selected(XAero aero);
};

#endif // MAPPANEL_H
