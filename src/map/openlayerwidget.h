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

	//= browser progress
    void start_progress();
    void update_progress(int progress);
    void end_progress(bool Ok);
	
	//= map calls
	void init_map();
	void execute_js(QString js_string);




	//# To map markers and features
	void add_runway(QString apt, QString lat1, QString lon1, QString lat2, QString lon2);
	void zoom_to_airport(QString apt);
	void zoom_to(QString lat, QString lon, int zoom);

	void show_aircraft(QString callsign, QString lat, QString lon, QString heading, QString altitude);
	void focus_aircraft(QString callsign);

	//====================================
	//@<< From Map Events
	void map_debug(QVariant);
	void map_mouse_move(QVariant lat, QVariant lon);
	void map_click(QVariant lat, QVariant lon);
	void map_right_click(QVariant lat, QVariant lon);


	void map_zoom_changed(QVariant zoom);
	void map_error(QVariant err);

	void marker_clicked(QVariant marker, QVariant mId);
	void marker_unselected(QVariant curr_idx, QVariant mLocationId);

	//======================================
	void closeEvent(QCloseEvent *event);

};

#endif // OPENLAYERWIDGET
