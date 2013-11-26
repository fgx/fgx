#ifndef MAPCOREWIDGET_H
#define MAPCOREWIDGET_H

#include <QVariant>
#include <QString>



#include <QStatusBar>
#include <QProgressBar>
#include <QLabel>
#include <QWidget>
#include <QActionGroup>
#include <QToolButton>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QToolBar>


#include "marble/MarbleGlobal.h"
#include "marble/MarbleWidget.h"
#include "marble/GeoDataLatLonAltBox.h"
#include "marble/GeoDataDocument.h"
#include "marble/GeoDataPlacemark.h"
#include "marble/GeoDataTreeModel.h"
#include "marble/GeoDataStyle.h"
#include "marble/GeoDataTrack.h"


#include "aircraft/xaero.h"

#include "xmarble/mapselectdialog.h"
class MapSelectDialog;

#include "xobjects/mainobject.h"
class MainObject;

#include "xmarble/xmarblewidget.h"
class XMarbleWidget;

#include "xwidgets/toolbargroup.h"
class ToolBarGroup;

using namespace Marble;



class MapCoreWidget : public QWidget
{
Q_OBJECT
public:

    static const QString SETTINGS_TAG;

	explicit MapCoreWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

    QHash<QString, GeoDataPlacemark*> blips;
    QHash<QString, GeoDataTrack*> tracks;


    QToolButton *buttBaseLayer;
    QToolButton *buttProjection;

    QToolButton *buttLoadView;
    QToolButton *buttSaveView;

	QString *mpmapFollowCallsign;
	QSlider *sliderZoom;
	QLabel *lblZoom;

	QToolBar *toolbarAirports;

	XMarbleWidget *marbleWidget;
    GeoDataDocument *docFlights;
    GeoDataDocument *docTracks;

private:

	QProgressBar *progressBar;
	QStatusBar *statusBar;
	QComboBox *comboServer;

	QLineEdit *txtLat;
	QLineEdit *txtLon;

	QButtonGroup *buttonGroupViewLayers;
	QCheckBox *chkViewStands;
	QCheckBox *chkViewRunwayLabels;
	QCheckBox *chkViewRunwayLines;

	QCheckBox *chkViewGridLines;


	QDoubleSpinBox *spinHeading;


	QToolButton *buttDebug;

    QActionGroup *actGroupMapViews;
    QActionGroup *actGroupBaseLayer;
    QActionGroup *actGroupProjection;


signals:

	void setx(QString option, bool enabled, QString value);

public slots:


    void on_map_view_action(QAction *);
    void on_map_base_layer_action(QAction*);
    void on_map_projection_action(QAction*);

	void on_slider_zoom(int zoom);
	void on_butt_zoom_in();
	void on_butt_zoom_out();
	void on_map_zoom_changed(int);

    void on_map_clicked();
    void on_map_moved(const GeoDataLatLonAltBox &);

	void refresh_map();
	void map_focus(QString lon, QString lat);

	//======================================
	void closeEvent(QCloseEvent *event);

    void on_save_view();
    void on_load_view();
    void load_views();

    void update_flights();
    void center_ksfo();

    void center_on(XAero aero);

    void on_select_map_view();

};

#endif // MAPCOREWIDGET_H
