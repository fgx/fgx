#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include <QtCore/QObject>

#include <QtSql/QSqlDatabase>

#include <QtGui/QSystemTrayIcon>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QWidgetAction>

#include "xobjects/xsettings.h"
//#include "launcher/launcherwindow.h" // << This included "mainobject.h" also !!
#include "mpmap/mpmapwidget.h"

class MpMapWidget;

class MainObject : public QObject
{
Q_OBJECT
public:
    explicit MainObject(QObject *parent = 0);
    ~MainObject();
	
    //** Objects
   // TelnetSlave *telnet;
	QSqlDatabase db;
    XSettings *settings;

	int launcher_flag;

	MpMapWidget *mpMapWidget;
	//LauncherWindow *launcherWindow;
	//PropsTreeWidget *propertiesTree;


    //* Widgets
    QSystemTrayIcon *trayIcon;
    QMenu *popupMenu;
    QWidgetAction *actionCallsign;
    QLabel *lblCallsign;

    QAction *actionLauncher;
    QAction *actionMpMap;
    QAction *actionSettings;

    QAction *actionQuit;

	void db_connect();
    bool db_sanity_check();

	QString runningOS();

signals:
	void show_settings(int);

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

};

#endif // MAINOBJECT_H
