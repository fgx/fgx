// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // mainobject.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2013
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include <QtCore/QObject>


#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QWidgetAction>


// Forward Declare Marble Map (wtf !)
//#include "marble/MarbleGlobal.h"
#include "marble/MarbleWidget.h"
//using namespace Marble;
//class Marble::MarbleWidget;


#include "xobjects/xsettings.h"

#include "xobjects/xsettingsmodel.h"
class XSettingsModel;

#include "mpmap/flightsmodel.h"
class FlightsModel;

#include "xobjects/xprocess.h"
class XProcess;





#include "launcher/launcherwindow.h"
class LauncherWindow;

#include "xmarble/mapmainwindow.h"
class MapMainWindow;

//#include "mpmap/mpmapxwidget.h"
//class MpMapXWidget;

#include "mpmap/mpmapwidget.h"

#include "logs/viewlogswidget.h"
#include "props/propstreewidget.h"
#include "xwidgets/fgxdebugwidget.h"

//class MpMapWidget;

class ViewLogsWidget;
class PropsTreeWidget;
class FgxDebugWidget;


/*! \class MainObject
 * \brief The MainObject is an extended QObject, and the main "startup and controller" for FGx.
 *
 * On startup, the MainObject is created and called when the app starts in the backgound.
 * It "shows" itself and is made visible via an icon in the QSystemTrayIcon ie TaskBar.
 * All widgets are children to this object, and is passed around as the first argument.
 * @todo: Make MainObject a singleton
 *
 * MainObject also controls the starting and stopping of processes// TBC
 *
 * @author: Peter Morgan
 * @author: Yves Sablonier
 */

class MainObject : public QObject
{
Q_OBJECT
public:
    explicit MainObject(QObject *parent = 0);
    ~MainObject();
	
	static int runningOs();

	bool debug_mode;

	static QString log_file_path();

	QString data_file(QString file_name);
	QString default_file(QString file_name);

	static QString temp_dir();
	static QString temp_dir(QString append_path);

    XSettings *settings;
	XSettingsModel *X;

    FlightsModel *flightsModel;

	LauncherWindow *launcherWindow;
    MapMainWindow  *mapMainWindow;
	//MpMapWidget *mpMapWidget;
    //MpMapXWidget *mpMapXWidget;

	ViewLogsWidget *viewLogsWidget;
	PropsTreeWidget *propertiesBrowser;
	FgxDebugWidget *fgxDebugWidget;

	XProcess *processFgFs;
	XProcess *processTerraSync;
	XProcess *processFgCom;


    QSystemTrayIcon *trayIcon;
    QMenu *popupMenu;

    QWidgetAction *actionCallsign;
    QLabel *lblCallsign;

    QAction *actionLauncher;
    QAction *actionMpMap;
	QAction *actionPropsBrowser;
	QAction *actionSetupWizard;

	QAction *actionViewLogs;
	QAction *actionViewFgxDebug;
	QAction *actionBrowserMap;

    QAction *actionQuit;

	QString get_fgfs_command();
	QStringList get_fgfs_args();
	QStringList get_env();

signals:
	void show_settings(int);
	void reload_paths();
	void on_debug_mode(bool enabled);
	void setx(QString option, bool enabled, QString value);

public slots:

	void initialize();

    void on_tray_icon(QSystemTrayIcon::ActivationReason reason);

    void on_launcher();
    void on_map_main_window();

	void on_browsermap();

	void on_properties_browser();

	void on_view_logs();
	void clear_log(QString log_name);
	void add_log(QString log_name, QString data);

	void start_all();
	void start_fgfs();
	void start_fgcom();
	void start_terrasync();
	void stop_all();

	void on_quit();

	void on_view_fgx_debug();
	void set_debug_mode(bool);

	void on_action_open_url(QAction*);

	void on_about_qt();
	void on_about_fgx();

	void on_upx(QString option, bool enabled, QString value);

};





#endif // MAINOBJECT_H
