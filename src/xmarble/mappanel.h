#ifndef MAPPANEL_H
#define MAPPANEL_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTabWidget>
#include "xobjects/mainobject.h"

#include "marble/MarbleWidget.h"
#include "marble/MarbleModel.h"
#include "marble/GeoDataTreeModel.h"

#include "xmarble/mapcorewidget.h"
class MapCoreWidget;

#include "navdata/navdatawidget.h"
class NavDataWidget;

#include "mpmap/flightswidget.h"
class FlightsWidget;

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


    QDockWidget *docker;
    QDockWidget *dockSmall;


    QTabWidget *tabWidget;

    NavDataWidget *navDataWidget;
    FlightsWidget *flightsWidget;



signals:

public slots:
    void aircraft_selected(XAero aero);
};

#endif // MAPPANEL_H
