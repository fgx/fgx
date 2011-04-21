
#include <QDebug>
#include <QCoreApplication>
#include <QtGui/QDesktopServices>

#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QCursor>


#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "xobjects/mainobject.h"
#include "launcher/launcherwindow.h"

//#include "mp/mpmapwidget.h"
//#include "map/googlemapwidget.h"

//#include "settings/settingswidget.h"


MainObject::MainObject(QObject *parent) :
    QObject(parent)
{

    //**********************************************************************
    //** Settings connection
    settings = new XSettings();

    //********************************************************************
    //** Telnet connection
    //TODO ? problem
    // I dont want to "create this object yet, as a socket might not be required
	// Hoewever I need a trick to create this object as required?? dont know how to do that yet..
    //telnet = NULL;
	//telnet = new TelnetSlave(this);
    // Somehow move this somewhere else and the first call is gonna be available based on a flag

	//connect(telnet, SIGNAL(telnet_connected(bool)), this, SLOT(on_telnet_connected(bool)));
	//    //void telnet_connected(bool);

    //********************************************************************
    //** SQL database problem
	// I dont want to connect until required
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db = QSqlDatabase::addDatabase("QMYSQL");
    //db.setHostName("localhost");
    //db.setUserName("root");
    //db.setPassword("mash");
    //db.setDatabaseName("ffs-desktop");
	qDebug() << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    db.setDatabaseName(QDesktopServices::storageLocation(QDesktopServices::DataLocation).append("ffs-desktop.sqlite"));
    if( !db.open() ){
        qDebug() << db.lastError();
        return;
    }
    if(!db_sanity_check()){
        qDebug() << "DB error";
        return;
    }

    //***********************************
    //** Tray Icon
	trayIcon = new QSystemTrayIcon(QIcon(":/icon/favicon"), this);
    trayIcon->setToolTip("FlightGear Launcher");
    trayIcon->setVisible(true);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(on_tray_icon(QSystemTrayIcon::ActivationReason)));


    //***********************************
    //** Menu and actions
    popupMenu = new QMenu();
    trayIcon->setContextMenu(popupMenu);

    actionCallsign = new QWidgetAction(this);
    lblCallsign = new QLabel();
    lblCallsign->setText("CALLSIGN");
    lblCallsign->setStyleSheet("font-family: monospace; background-color: black; color: white; padding: 5px; font-weight: bold;");
    actionCallsign->setDefaultWidget(lblCallsign);

    popupMenu->addAction(actionCallsign); //QIcon(":/icons/mpmap"), tr("Multiplayer Map"));


	actionLauncher = popupMenu->addAction(QIcon(":/icons/launcher"), tr("Launcher"));
	actionLauncher->setIconVisibleInMenu(true);
	connect(actionLauncher, SIGNAL(triggered()), this, SLOT(on_launcher()) );

    actionMpMap = popupMenu->addAction(QIcon(":/icons/mpmap"), tr("Multiplayer Map"));
	actionMpMap->setIconVisibleInMenu(true);
    connect(actionMpMap, SIGNAL(triggered()), this, SLOT(on_mpmap()));

    //**** Telnet Menu
    QMenu *actionTelnetMenu = new QMenu();    
    popupMenu->addMenu(actionTelnetMenu);
    actionTelnetMenu->setTitle("Telnet");

    actionTelnetConnect = new QAction(actionTelnetMenu);
    actionTelnetMenu->addAction(actionTelnetConnect);
    actionTelnetConnect->setText(tr("Connect"));
    actionTelnetConnect->setIcon(QIcon(":/icons/connect"));
	actionTelnetConnect->setIconVisibleInMenu(true);
    connect(actionTelnetConnect, SIGNAL(triggered()),
            this, SLOT(on_telnet_connect_action())
    );

    actionTelnetDisconnect = new QAction(actionTelnetMenu);
    actionTelnetMenu->addAction(actionTelnetDisconnect);
    actionTelnetDisconnect->setText(tr("Disconnect"));
    actionTelnetDisconnect->setIcon(QIcon(":/icons/disconnect"));
	actionTelnetDisconnect->setIconVisibleInMenu(true);
    actionTelnetDisconnect->setDisabled(true);
    connect(actionTelnetDisconnect, SIGNAL(triggered()),
            this, SLOT(on_telnet_disconnect_action())
    );

	//*** Properties
	QAction *actionPropsBrowser= new QAction(this);
	actionPropsBrowser->setIcon(QIcon(":/icons/properties_browser"));
	actionPropsBrowser->setText(tr("Properties Browser"));
	actionPropsBrowser->setIconVisibleInMenu(true);
	popupMenu->addAction(actionPropsBrowser);
	connect(actionPropsBrowser, SIGNAL(triggered()),
			this, SLOT(on_properties_browser())
	);


    //*** Settings
    QAction *actionSettings= new QAction(this);
    actionSettings->setIcon(QIcon(":/icons/settings"));
    actionSettings->setText(tr("Settings"));
	actionSettings->setIconVisibleInMenu(true);
    popupMenu->addAction(actionSettings);
    connect(actionSettings, SIGNAL(triggered()),
            this, SLOT(on_settings())
    );

    popupMenu->addSeparator();



    //*** Quit
    actionQuit = popupMenu->addAction(QIcon(":/icons/quit"), tr("Quit"));
	actionQuit->setIconVisibleInMenu(true);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(on_quit()));

    //** Setup
    trayIcon->show();

	//==========================================
	//***** Initial check if FG_ROOT is set
	qDebug() << settings->fg_root() << settings->fg_root().length();
	if(settings->fg_root().length() == 0){

	}
	//on_mpmap();
	on_launcher();
    //on_map();

} /* constructor */


//*****************************************************************************
//** Shows the Launcher window
void MainObject::on_launcher(){
	qDebug() << launcherWindow;
	launcherWindow = new LauncherWindow(this);
    launcherWindow->show();
}

//****************************************************************************
//** Settings Dialog
void MainObject::on_settings(){
	//SettingsWidget *sWidget = new SettingsWidget(this);
   // sWidget->show();
}

//****************************************************************************
//** Map
void MainObject::on_map(){
	//GoogleMapWidget *gmapWidget = new GoogleMapWidget();
   // gmapWidget->show();
}

//****************************************************************************
//** MpMap
void MainObject::on_mpmap(){
	//MpMapWidget *mpMapWidget = new MpMapWidget(this);
   // mpMapWidget->show();
}

//****************************************************************************
//** MpMap
void MainObject::on_properties_browser(){
	//propertiesTree = new PropsTreeWidget(this);
	//propertiesTree->show()
	//mpMapWidget->show();
}


//****************************************************************************
//** Database Sanity Check - created database and tables if not exist
bool MainObject::db_sanity_check(){
    QSqlQuery query;
    query.prepare("select version from db_version;");
    if(!query.exec()){
        qDebug() << "Sanity=" << query.lastError();
        QStringList queries;
        queries.append("CREATE TABLE IF NOT EXISTS db_version( `version` varchar(20) );");
        queries.append("INSERT INTO db_version ( `version` )VALUES( 0.1 );");
        queries.append("CREATE TABLE IF NOT EXISTS airport_favs( airport varchar(20) );");
        for(int i = 0; i < queries.size(); ++i){
            qDebug() << queries.at(i);
            QSqlQuery q;
            //query.prepare(queries.at(i));
            if(!q.exec(queries.at(i))){
                qDebug() << "OOps=" << q.lastError();
                return false;
            }
        }
        return true;
    }
    return true;
}



//****************************************************************************
//** Quit
void MainObject::on_quit(){
    QCoreApplication::instance()->quit();
}


//****************************************************************************
//** Tray Icon
void MainObject::on_tray_icon(QSystemTrayIcon::ActivationReason reason){   
    //* Right click will show the context Menu above system tray
    //* Following will popup menu with single click on Top LEFT ??
    if(reason == QSystemTrayIcon::Trigger){
        QPoint p = QCursor::pos();
        trayIcon->contextMenu()->popup(p);
    }
}

//*******************************************************************************************
//*********************************
//** Telnet Action  click events
void MainObject::on_telnet_connect_action(){
     qDebug() << "on_telnet_connect_action(@)";
	//telnet->fg_connect();
}
void MainObject::on_telnet_disconnect_action(){
    qDebug() << "on_telnet_disconnect_action(@)";
	//telnet->fg_disconnect();
}

//*************************************************
//** Telnet Events
void MainObject::on_telnet_connected(bool state){
    //telnet->fg_connect();
    qDebug() << "on_telnet_connected SIGNAL" << state;
    //qDebug() << state;
    //statusBarTelnet->showMessage(state ? tr("Connected") : tr("Disconnected"), 4000);
    //buttTelnetConnect->setDisabled(state);
    //buttTelnetDisconnect->setDisabled(!state);
    trayIcon->setIcon(QIcon(state ? ":/icons/connect" : ":/icons/disconnect"));
    actionTelnetConnect->setDisabled(state);
    actionTelnetDisconnect->setDisabled(!state);
}

//=====================================
QString MainObject::runningOS() {
	#ifdef Q_WS_X11
	return QString("linux");
	#endif

	#ifdef Q_WS_MAC
	return QString("mac");
	#endif

	#ifdef Q_WS_WIN
	return QString("windows");
	#endif

	return QString("unknown");
}
