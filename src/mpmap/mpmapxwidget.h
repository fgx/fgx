#ifndef MPMAPXWIDGET_H
#define MPMAPXWIDGET_H

#include <QtCore/QVariant>
#include <QtCore/QString>

#include <QtWebKit/QWebView>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkCookieJar>

#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QProgressBar>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QActionGroup>
#include <QtGui/QToolButton>


#include "map/openlayerwidget.h"
class OpenLayerWidget;

#include "mpmap/pilotswidget.h"
class PilotsWidget;

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
	PilotsWidget *pilotsWidget;

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
