#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include <QtCore/QObject>

#include <QtSql/QSqlDatabase>

#include <QtGui/QSystemTrayIcon>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QLabel>
#include <QtGui/QWidgetAction>

//#include "props/telnetslave.h"
#include "xobjects/xsettings.h"
//#include "props/propstreewidget.h"

class MainObject : public QObject
{
Q_OBJECT
public:
    explicit MainObject(QObject *parent = 0);

    //** Objects
   // TelnetSlave *telnet;
   // QSqlDatabase db;
    XSettings *settings;

	//PropsTreeWidget *propertiesTree;

    QAction *actionTelnetConnect;
    QAction *actionTelnetDisconnect;

    //* Widgets
    QSystemTrayIcon *trayIcon;
    QMenu *popupMenu;
    QWidgetAction *actionCallsign;
    QLabel *lblCallsign;

    QAction *actionLauncher;
    QAction *actionMpMap;
    QAction *actionSettings;

    QAction *actionQuit;

    bool db_sanity_check();

	QString runningOS();

signals:

public slots:
    void on_tray_icon(QSystemTrayIcon::ActivationReason reason);

    void on_telnet_connected(bool);
    void on_telnet_connect_action();
    void on_telnet_disconnect_action();


    void on_launcher();
    void on_mpmap();
    void on_map();
    void on_settings();
	void on_properties_browser();
    void on_quit();
};

#endif // MAINOBJECT_H
