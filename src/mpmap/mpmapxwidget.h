// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // mpmapxwidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef MPMAPXWIDGET_H
#define MPMAPXWIDGET_H

#include <QVariant>
#include <QString>

#include <QWebView>
#include <QNetworkDiskCache>
#include <QNetworkCookieJar>

#include <QSplitter>
#include <QStatusBar>
#include <QProgressBar>
#include <QLabel>
#include <QWidget>
#include <QActionGroup>
#include <QToolButton>


#include "map/openlayerwidget.h"
class OpenLayerWidget;

#include "mpmap/flightswidget.h"
class FlightsWidget;

#include "xobjects/mainobject.h"
class MainObject;

//#include "xobjects/latlng.h"

#include "aircraft/xaero.h"




class MpMapXWidget : public QWidget
{
Q_OBJECT
public:
	explicit MpMapXWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	OpenLayerWidget *mapWidget;
    FlightsWidget *pilotsWidget;

	QSplitter *splitter;

	void closeEvent(QCloseEvent *event);
signals:

public slots:

	void initialize();

	void add_airport(QString airport);
	void add_runway(QString airport, QString rwy1, QString rwy2, QString lat1, QString lng1, QString lat2, QString lng2);
	void show_airport(QString airport);

	void focus_aero(XAero aero);

	//void on_freeze_map(bool freeze);
	void on_splitter_moved();
};

#endif // MPMAPXWIDGET_H
