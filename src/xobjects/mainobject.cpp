
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


#include <QtGui/QDesktopServices>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QCursor>


#include "xobjects/mainobject.h"
#include "launcher/launcherwindow.h"

//#include "mpmap/mpmapwidget.h"
//#include "map/googlemapwidget.h"

#include "settings/settingsdialog.h"
#include "utilities/utilities.h"

MainObject::MainObject(QObject *parent) :
    QObject(parent)
{
	
    util_setStdLogFile(); // init the LOG file

    //**********************************************************************
    //** Settings connection
    settings = new XSettings();

	launcher_flag = false;

	mpMapWidget = 0;

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
    lblCallsign->setText("(your callsign)");
    lblCallsign->setStyleSheet("color: #ff0000; padding: 0px; font-weight: bold;");
    actionCallsign->setDefaultWidget(lblCallsign);

	popupMenu->addAction(actionCallsign);


	actionLauncher = popupMenu->addAction(tr("Launcher"));
	actionLauncher->setIconVisibleInMenu(true);
	connect(actionLauncher, SIGNAL(triggered()), this, SLOT(on_launcher()) );

	actionMpMap = popupMenu->addAction(tr("Start Map Widget"));
	actionMpMap->setIconVisibleInMenu(true);
    connect(actionMpMap, SIGNAL(triggered()), this, SLOT(on_mpmap()));

    //**** Telnet Menu
    //QMenu *actionTelnetMenu = new QMenu();    
    //popupMenu->addMenu(actionTelnetMenu);
    //actionTelnetMenu->setTitle("Telnet");

    //actionTelnetConnect = new QAction(actionTelnetMenu);
    //actionTelnetMenu->addAction(actionTelnetConnect);
    //actionTelnetConnect->setText(tr("Connect"));
    //actionTelnetConnect->setIcon(QIcon(":/icons/connect"));
	//actionTelnetConnect->setIconVisibleInMenu(true);
    //connect(actionTelnetConnect, SIGNAL(triggered()),
    //        this, SLOT(on_telnet_connect_action())
    //);

    //actionTelnetDisconnect = new QAction(actionTelnetMenu);
    //actionTelnetMenu->addAction(actionTelnetDisconnect);
    //actionTelnetDisconnect->setText(tr("Disconnect"));
    //actionTelnetDisconnect->setIcon(QIcon(":/icons/disconnect"));
	//actionTelnetDisconnect->setIconVisibleInMenu(true);
    //actionTelnetDisconnect->setDisabled(true);
    //connect(actionTelnetDisconnect, SIGNAL(triggered()),
    //        this, SLOT(on_telnet_disconnect_action())
    //);

	//*** Properties
	//QAction *actionPropsBrowser= new QAction(this);
	//actionPropsBrowser->setIcon(QIcon(":/icons/properties_browser"));
	//actionPropsBrowser->setText(tr("Properties Browser"));
	//actionPropsBrowser->setIconVisibleInMenu(true);
	//popupMenu->addAction(actionPropsBrowser);
	//connect(actionPropsBrowser, SIGNAL(triggered()),
	//		this, SLOT(on_properties_browser())
	//);


    //*** Settings
    QAction *actionSettings= new QAction(this);
    actionSettings->setIcon(QIcon(":/icons/settings"));
    actionSettings->setText(tr("Path and database settings"));
	actionSettings->setIconVisibleInMenu(true);
    popupMenu->addAction(actionSettings);
    connect(actionSettings, SIGNAL(triggered()),
            this, SLOT(on_settings())
    );

    popupMenu->addSeparator();



    //*** Quit
	actionQuit = popupMenu->addAction(QIcon(":/icon/quit"), tr("Quit"));
	actionQuit->setIconVisibleInMenu(true);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(on_quit()));

    //** Setup
    trayIcon->show();

	//==========================================
	//***** Initial check if FG_ROOT is set
	//qDebug() << settings->fg_root() << settings->fg_root().length();
	//if(settings->fg_root().length() == 0){

	//}

	//if(!settings->value("initial_setup").toBool()){
	//	on_settings();
	//}

	//on_settings();
	//on_launcher();
    //on_map();
	connect(this, SIGNAL(show_settings(int)), this, SLOT(on_settings(int)));

	QTimer::singleShot(300, this, SLOT(initialize()));

}

MainObject::~MainObject()
{
    outLog(util_getDateTimestg()+" - Application close");
}

//============================================================================
//** Shows the Launcher window
void MainObject::initialize(){
	db_connect();
	on_launcher();
}

//============================================================================
//**  Launcher window
void MainObject::on_launcher(){
	//TODO there HAS to be a better was than using a flag
	if(launcher_flag == true){
		return;
	}
	LauncherWindow *launcherWindow = new LauncherWindow(this);
    launcherWindow->show();
	launcher_flag = true;
}


//============================================================================
//** Settings Dialog
void MainObject::on_settings(){
	on_settings(0);
}
void MainObject::on_settings(int idx){
	SettingsDialog *dialog = new SettingsDialog(this);
	dialog->tabWidget->setCurrentIndex(idx);
	dialog->exec();
}


//****************************************************************************
//** Map
void MainObject::on_map(){
	//GoogleMapWidget *gmapWidget = new GoogleMapWidget();
   // gmapWidget->show();
}

//============================================================================
//** MpMap
void MainObject::on_mpmap(){
	if(mpMapWidget == 0){
		//qDebug() << "NO mpmap";
		mpMapWidget = new MpMapWidget(this);

	}else{
		//qDebug() << "exists";
	}
	mpMapWidget->show();
	mpMapWidget->setFocus();
	//TODO need to set focus here.
}

//======================================
//** Propertes Browser
void MainObject::on_properties_browser(){
	//propertiesTree = new PropsTreeWidget(this);
	//propertiesTree->show()
	//mpMapWidget->show();
}

//======================================
//** DB Connect
void MainObject::db_connect(){

	trayIcon->showMessage("DB", "Opening DB");

	if (settings->value("db_engine").toString() == "QMYSQL"){
		//** Setup MySql
		db = QSqlDatabase::addDatabase("QMYSQL");
		db.setHostName(settings->value("db_host").toString());
		db.setUserName(settings->value("db_user").toString());
		db.setPassword(settings->value("db_pass").toString());
		db.setDatabaseName(settings->value("db_database").toString());

	}else if(settings->value("db_engine").toString() == "QSQLITE"){
		db = QSqlDatabase::addDatabase("QSQLITE");
		db.setDatabaseName(settings->db_file());

	}else{
		trayIcon->showMessage("DB Error", "Could not open DB", QSystemTrayIcon::Critical, 5000);
		emit(show_settings(1));
		qDebug() << "NO ENGINE";
		return;
	}

	//* Catch DB Open error
	if(!db.open()){
		qDebug() << db.lastError().text();
		trayIcon->showMessage("DB Error", "Could not open DB", QSystemTrayIcon::Critical, 5000);
		db.close();
		emit(show_settings(1));
		return;
	}

	//* Database connected
	trayIcon->showMessage("DB", "Connected", QSystemTrayIcon::Information, 3000);
	//bool foo = db_sanity_check();

}


//======================================
//** Database Sanity Check - creates database and tables if not exist
bool MainObject::db_sanity_check(){
    QSqlQuery query;
    query.prepare("select version from db_version;");
    if(!query.exec()){
        qDebug() << "Sanity=" << query.lastError();
        QStringList queries;
        queries.append("CREATE TABLE IF NOT EXISTS db_version( `version` varchar(20) );");
        queries.append("INSERT INTO db_version ( `version` )VALUES( 0.1 );");

		//* Aircraft
		queries.append("CREATE TABLE IF NOT EXISTS aircraft(  `aero` varchar(20), `directory` varchar(20), `xml_file` varchar(30), `description` varchar(100), `fdm` varchar(20), `author` varchar(255), PRIMARY KEY(aero) );");
		queries.append("CREATE INDEX directory ON aircraft(directory);");
		queries.append("CREATE INDEX xml_file ON aircraft(xml_file);");
		queries.append("CREATE INDEX fdm ON aircraft(fdm);");
		queries.append("CREATE INDEX description ON aircraft(description);");

		//queries.append("CREATE TABLE airports(code varchar(10) NOT NULL PRIMARY KEY, name varchar(50) NULL, elevation int, tower tinyint NULL);");
		//queries.append("CREATE TABLE runways(airport varchar(10) NOT NULL, runway varchar(15), width numeric(2,2), length int, lat float, lng float, heading float )");
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
	
	// delete pid files on quit ?
	// system("rm -f /tmp/terrasync.pid");
	
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
