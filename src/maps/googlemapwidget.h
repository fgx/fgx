#ifndef GOOGLEMAPWIDGET_H
#define GOOGLEMAPWIDGET_H

#include <QtCore/QVariant>
#include <QtCore/QString>

#include <QtGui/QStatusBar>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QActionGroup>
#include <QtGui/QToolButton>

#include <QtWebKit/QWebView>

#include "xobjects/mainobject.h"
#include "xobjects/latlng.h"

class MainObject;


class GoogleMapWidget : public QWidget
{
Q_OBJECT
public:
	explicit GoogleMapWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

    QWebView *webView;
    QStatusBar *statusBar;
    QLabel *lblLat;
    QLabel *lblLng;
    QLabel *lblZoom;
    QActionGroup *groupZoom;
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

     void on_zoom_action(QAction *);

      //** Map Events
      void map_mouse_move(QVariant lat, QVariant lng);
      void map_right_click(QVariant lat, QVariant lng);
      void map_zoom_changed(QVariant zoom);
      void map_error(QVariant err);

      void marker_clicked(QVariant marker, QVariant mId);
      void marker_unselected(QVariant curr_idx, QVariant mLocationId);



};

#endif // GOOGLEMAPWIDGET_H
