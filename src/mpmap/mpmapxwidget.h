#ifndef MPMAPXWIDGET_H
#define MPMAPXWIDGET_H

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

#include "mpmap/pilotswidget.h"

#include "xobjects/mainobject.h"
#include "xobjects/latlng.h"

class MainObject;
class PilotsWidget;


class MpMapXWidget : public QWidget
{
Q_OBJECT
public:
	explicit MpMapXWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

    QWebView *webView;
	QNetworkDiskCache *networkDiskCache;
	QNetworkCookieJar *networkCookieJar;

	PilotsWidget *pilotsWidget;

	QProgressBar *progressBar;
    QStatusBar *statusBar;
    QLabel *lblLat;
	QLabel *lblLng;
    QLabel *lblZoom;
	QButtonGroup *groupZoom;
    QToolButton *buttZoom;

    QString to_lat(QVariant);

    void execute_js(QString js_string);
    void zoom_to(QString lat, QString lng, QString zoom);

    void add_marker(LatLng latlng, QString label);

    void add_runway(float lat1, float lng1, float lat2, float lng2, QString label);
    void add_runway(QString lat1, QString lng1, QString lat2, QString lng2, QString label);
    void add_runway(LatLng p1, LatLng p2, QString label);


signals:

public slots:
	void initialize();

	 void on_zoom_action(int idx);

	  //== Map Events
      void map_mouse_move(QVariant lat, QVariant lng);
      void map_right_click(QVariant lat, QVariant lng);
      void map_zoom_changed(QVariant zoom);
      void map_error(QVariant err);

      void marker_clicked(QVariant marker, QVariant mId);
      void marker_unselected(QVariant curr_idx, QVariant mLocationId);

	  void start_progress();
	  void update_progress(int progress);
	  void end_progress(bool Ok);


};

#endif // MPMAPXWIDGET_H
