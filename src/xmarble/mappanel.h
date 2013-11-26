#ifndef MAPPANEL_H
#define MAPPANEL_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTabWidget>
#include "xobjects/mainobject.h"



#include "xmarble/mapcorewidget.h"
class MapCoreWidget;

#include "navdata/navdatawidget.h"
class NavDataWidget;

#include "mpmap/flightswidget.h"
class FlightsWidget;



//================================================

class MapPanel : public QMainWindow
{
    Q_OBJECT
public:
    explicit MapPanel(MainObject*, QWidget *parent = 0);

    MainObject *mainObject;

    MapCoreWidget *mapWidget;



    QDockWidget *docker;
    QTabWidget *tabWidget;

    NavDataWidget *navDataWidget;
    FlightsWidget *flightsWidget;



signals:

public slots:

};

#endif // MAPPANEL_H
