#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include <QtCore/QObject>

//#include <QtSql/QSqlDatabase>

#include <QtGui/QSystemTrayIcon>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QWidgetAction>

#include "xobjects/xsettings.h"
#include "launcher/launcherwindow.h"
#include "maps/mpmapwidget.h"
#include "maps/googlemapwidget.h"
#include "logs/viewlogswidget.h"

#include "xobjects/xprocess.h"


//= Forward Declarations
class LauncherWindow;
class MpMapWidget;
class GoogleMapWidget;
class ViewLogsWidget;
class XProcess;



/*! \class MainObject
 * \brief The MainObject is an extended QObject, and the main "controller" for FGx
 *
 * This is the main object created and called when the app starts in the backgound
 * It manifest itself and is made visible via an icon in the QSystemTrayIcon ie TaskBar
 * All widgets are children to this object and are hidden (at the moment)
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
	

    XSettings *settings;

	LauncherWindow *launcherWindow;
	MpMapWidget *mpMapWidget;
	GoogleMapWidget *googleMapWidget;

	ViewLogsWidget *viewLogsWidget;


    QSystemTrayIcon *trayIcon;
    QMenu *popupMenu;
    QWidgetAction *actionCallsign;
    QLabel *lblCallsign;

    QAction *actionLauncher;
    QAction *actionMpMap;
	QAction *actionSetupWizard;
	QAction *actionViewLog;

    QAction *actionQuit;

	XProcess *processFgFs;
	XProcess *processTerraSync;
	XProcess *processFgCom;

	QString get_fgfs_command();
	QStringList get_env();



signals:
	void show_settings(int);
	void reload_paths();

public slots:

	void initialize();

    void on_tray_icon(QSystemTrayIcon::ActivationReason reason);

    void on_launcher();
    void on_mpmap();
    void on_map();
    void on_settings();
	void on_settings(int);
	void on_properties_browser();
    void on_quit();
	void show_setup_wizard();

	void on_view_logs();
	void clear_log(QString log_name);
	void add_log(QString log_name, QString data);

	void start_all();
	void start_fgfs();
	void start_fgcom();
	void start_terrasync();
	void stop_all();

	void quit();

};


#endif // MAINOBJECT_H
