
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
	lblCallsign->setStyleSheet("color: #0000ff; padding: 0px; font-weight: bold;");
    actionCallsign->setDefaultWidget(lblCallsign);

	popupMenu->addAction(actionCallsign);


	actionLauncher = popupMenu->addAction(tr("Launcher"));
	actionLauncher->setIconVisibleInMenu(true);
	connect(actionLauncher, SIGNAL(triggered()), this, SLOT(on_launcher()) );

	actionMpMap = popupMenu->addAction(tr("Start Map Widget"));
	actionMpMap->setIconVisibleInMenu(true);
    connect(actionMpMap, SIGNAL(triggered()), this, SLOT(on_mpmap()));


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
	
	QSqlDatabase db;
	QSqlQuery query;

	if (settings->value("db_engine").toString() == "QMYSQL"){
		outLog("*** FGX reports: we are using QMYSQL engine ***");
		db = QSqlDatabase::addDatabase("QMYSQL");
		db.setHostName(settings->value("db_host").toString());
		db.setUserName(settings->value("db_user").toString());
		db.setPassword(settings->value("db_pass").toString());
		db.setDatabaseName(settings->value("db_database").toString());

	}else if(settings->value("db_engine").toString() == "QSQLITE"){
		outLog("*** FGX reports: we are using QSQLITE engine ***");
		db = QSqlDatabase::addDatabase("QSQLITE");
		db.setDatabaseName(settings->db_file());
		qDebug() << "dbfile settings: " << settings->db_file();

	}else {
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


