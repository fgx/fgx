// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // openlayerwidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef OPENLAYERWIDGET_H
#define OPENLAYERWIDGET_H

#include <QtCore/QVariant>
#include <QtCore/QString>

#include <QtWebKitWidgets/QWebView>
#include <QWebInspector>
#include <QtNetwork/QNetworkDiskCache>


#include <QStatusBar>
#include <QProgressBar>
#include <QLabel>
#include <QWidget>
#include <QActionGroup>
#include <QToolButton>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QToolBar>
#include <QDial>

#include "xobjects/mainobject.h"


class MainObject;
QT_FORWARD_DECLARE_CLASS(QNetworkDiskCache)

class OpenLayerWidget : public QWidget
{
Q_OBJECT
public:
    explicit OpenLayerWidget(MainObject *mob, QWidget *parent = 0);

    MainObject *mainObject;
    QString *mpmapFollowCallsign;
    QSlider *sliderZoom;
    QLabel *lblZoom;

    QString map_type;
    QToolBar *toolbarAirports;

    QWebView *webView;

private:

    QProgressBar *progressBar;
    QStatusBar *statusBar;
    QComboBox *comboServer;

    QLineEdit *txtLat;
    QLineEdit *txtLon;
    QSpinBox *spinHeading;
    QDial *dialHeading;


    QButtonGroup *buttonGroupViewLayers;
    QCheckBox *chkViewStands;
    QCheckBox *chkViewRunwayLabels;
    QCheckBox *chkViewRunwayLines;

    QCheckBox *chkViewGridLines;


    QToolButton *buttDebug;
    QWebInspector *webInspector;

    QNetworkDiskCache *networkDiskCache;
    //QNetworkCookieJar *networkCookieJar;

signals:

    void setx(QString option, bool enabled, QString value);
    void setv(QString option, QString value);
    void map_double_clicked(QString lat, QString lon, QString heading);

public slots:

    //= browser progress
    void start_progress();
    void update_progress(int progress);
    void end_progress(bool Ok);

    //= map calls
    void load_map(QString map_type);
    void execute_js(QString js_string);


    //# To map markers and features
    void clear_airport(QString apt);
    void clear_map();

    void add_runway(QString apt, QString rwy1, QString rwy2, QString lat1, QString lon1, QString lat2, QString lon2);
    void add_stand(QString apt, QString name, QString lat, QString lon);
    void add_tower(QString apt, QString lat, QString lon);

    void show_aircraft(QString callsign, QString lat, QString lon, QString heading, QString altitude);
    void focus_aircraft(QString callsign);

    void show_radar(QString callsign, QString lat, QString lon, QString heading, QString altitude, bool is_tower);
    //void display_radar_layer( int viz);
    void clear_radar();


    void zoom_to_airport(QString apt);
    void zoom_to_latlon(QString lat, QString lon, int zoom);
    void zoom_to(int zoom);

    void on_zoom_in();
    void on_zoom_out();


    //====================================
    //@<< From Map Events
    void map_initialised();

    void map_debug(QVariant);

    //void map_mouse_move(QVariant lat, QVariant lon);
    void map_click(QVariant lat, QVariant lon);
    void map_right_click(QVariant lat, QVariant lon);

    void map_set_coords(QVariant lat, QVariant lon);
    void on_lat_changed(QString);
    void on_lon_changed(QString);


    void map_zoom_changed(QVariant zoom);
    void map_error(QVariant err);

    void marker_clicked(QVariant marker, QVariant mId);
    void marker_unselected(QVariant curr_idx, QVariant mLocationId);

    //======================================
    void closeEvent(QCloseEvent *event);

    void on_upx( QString option, bool enabled, QString value);

    void on_display_layer(QAbstractButton*);

    void on_dial(int val);
    void on_spin(int val);
    void on_show_debugger();
    void on_debug_mode(bool);
};

#endif // OPENLAYERWIDGET
