#ifndef OPENLAYERWIDGET_H
#define OPENLAYERWIDGET_H

#include <QtCore/QVariant>
#include <QtCore/QString>

#include <QtWebKit/QWebView>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkCookieJar>


#include <QtGui/QStatusBar>
#include <QtGui/QProgressBar>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QActionGroup>
#include <QtGui/QToolButton>

#include "xobjects/mainobject.h"
#include "xobjects/latlng.h"

class MainObject;
QT_FORWARD_DECLARE_CLASS(QNetworkDiskCache)

class OpenLayerWidget : public QWidget
{
Q_OBJECT
public:
	explicit OpenLayerWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;
	QString *mpmapFollowCallsign;
	
private:
	
	QProgressBar *progressBar;
	QStatusBar *statusBar;
	QComboBox *comboServer;
	
	QLabel *lblLat;
	QLabel *lblLon;

	QLabel *lblZoom;
	QButtonGroup *groupZoom;
	QToolButton *buttZoom;

	
	QWebView *webView;
	QNetworkDiskCache *networkDiskCache;
	QNetworkCookieJar *networkCookieJar;
	
signals:
	
	public slots:
    void start_progress();
    void update_progress(int progress);
    void end_progress(bool Ok);
	
	void init_xmap();
    void closeEvent(QCloseEvent *event);
	void loadFinished(bool);
	void setLatLon();
	void setZoom();
	void addRunway();

	//====================================
	//== Map Events
	void map_debug(QVariant);
	void map_mouse_move(QVariant lat, QVariant lng);
	void map_click(QVariant lat, QVariant lng);
	void map_right_click(QVariant lat, QVariant lng);


	void map_zoom_changed(QVariant zoom);
	void map_error(QVariant err);

	void marker_clicked(QVariant marker, QVariant mId);
	void marker_unselected(QVariant curr_idx, QVariant mLocationId);



	//void add_airport(QString airport);
	//void add_runway(QString airport, QString rwy1, QString lat1, QString lng1, QString rwy2, QString lat2, QString lng2);
	//void show_airport(QString airport);


};

#endif // OPENLAYERWIDGET
