#ifndef MAPPANEL_H
#define MAPPANEL_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTabWidget>
#include "xobjects/mainobject.h"

#include "mpmap/flightswidget.h"
class FlightsWidget;

//#include "mpmap/flightsmodel.h"
//class FlightsModel;

//#include "marble/MarbleGlobal.h"
//#include "marble/MarbleWidget.h"

#include "xmarble/mapcorewidget.h"
class MapCoreWidget;



class MapPanel : public QMainWindow
{
    Q_OBJECT
public:
    explicit MapPanel(MainObject*, QWidget *parent = 0);

    MainObject *mainObject;

    MapCoreWidget *mapWidget;



    QDockWidget *docker;
    QTabWidget *tabWidget;

    FlightsWidget *flightsWidget;
    //FlightsModel *flightsModel;


signals:

public slots:

};

#endif // MAPPANEL_H
