
//#include <QDebug>

#include <QCoreApplication>
#include <QApplication>
#include <QTimer>

#include <QStyleFactory>

#include <QtGui/QDesktopServices>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QCursor>


#include "xobjects/mainobject.h"
#include "launcher/launcherwindow.h"

#include "settings/settingsdialog.h"
#include "utilities/utilities.h"
#include "setupwizard/setupwizard.h"



MainObject::MainObject(QObject *parent) :
    QObject(parent)
{

	//= init the LOG file
	util_setStdLogFile();

	//= XSettings Object
    settings = new XSettings();

	//= Set GLobal style
	QApplication::setStyle( QStyleFactory::create(settings->style_current()) );

	launcher_flag = false;

	mpMapWidget = 0;

	//= Log Viewer is hidden
	viewLogsWidget = new ViewLogsWidget(this);
	viewLogsWidget->hide();

	//= Processes
	processFgFs  = new XProcess(this, "fgfs");

	//============================
	//== Tray Icon
	trayIcon = new QSystemTrayIcon(QIcon(":/icon/favicon"), this);
    trayIcon->setToolTip("FlightGear Launcher");
    trayIcon->setVisible(true);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(on_tray_icon(QSystemTrayIcon::ActivationReason)));


	//== Tray Menu and actions
    popupMenu = new QMenu();
    trayIcon->setContextMenu(popupMenu);

	//== Callsign - label in a widget..
    actionCallsign = new QWidgetAction(this);
    lblCallsign = new QLabel();
    lblCallsign->setText("(your callsign)");
	lblCallsign->setStyleSheet("color: #0000ff; padding: 0px; font-weight: bold;");
    actionCallsign->setDefaultWidget(lblCallsign);
	popupMenu->addAction(actionCallsign);

	//= Launcher Action
	actionLauncher = popupMenu->addAction(tr("Launcher"));
	actionLauncher->setIconVisibleInMenu(true);
	connect(actionLauncher, SIGNAL(triggered()), this, SLOT(on_launcher()) );

	//= MpMap action
	actionMpMap = popupMenu->addAction(tr("Start Map Widget"));
	actionMpMap->setIconVisibleInMenu(true);
    connect(actionMpMap, SIGNAL(triggered()), this, SLOT(on_mpmap()));


	//= Settings Action
	QAction *actionSetupWizard= new QAction(this);
	actionSetupWizard->setIcon(QIcon(":/icon/wizard"));
	actionSetupWizard->setText(tr("Setup Wizard..."));
	actionSetupWizard->setIconVisibleInMenu(true);
	popupMenu->addAction(actionSetupWizard);
	connect(actionSetupWizard, SIGNAL(triggered()),
			this, SLOT(show_setup_wizard())
    );

	//== View Logs
	QAction *actionViewLogs = new QAction(this);
	actionViewLogs->setIcon(QIcon(":/icon/log"));
	actionViewLogs->setText(tr("View Log..."));
	actionViewLogs->setIconVisibleInMenu(true);
	popupMenu->addAction(actionViewLogs);
	connect(actionViewLogs, SIGNAL(triggered()),
			this, SLOT(on_view_logs())
	);

    popupMenu->addSeparator();



	//== Quit
	actionQuit = popupMenu->addAction(QIcon(":/icon/quit"), tr("Quit"));
	actionQuit->setIconVisibleInMenu(true);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(on_quit()));

	//==================
    trayIcon->show();

	connect(this, SIGNAL(show_settings(int)), this, SLOT(on_settings(int)));

	//== initialise after initial show so UI dont look frozen while cache loading etc
	QTimer::singleShot(300, this, SLOT(initialize()));

}

MainObject::~MainObject()
{
    outLog(util_getDateTimestg()+" - Application close");
}

//============================================================================
//** Shows the Launcher window
void MainObject::initialize(){

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
//== Propertes Browser
void MainObject::on_properties_browser(){
	//propertiesTree = new PropsTreeWidget(this);
	//propertiesTree->show()
	//mpMapWidget->show();
}



//=====================================
//== Quit
void MainObject::on_quit(){
    QCoreApplication::instance()->quit();
}


//=================================================
//== Tray Icon Clicked
void MainObject::on_tray_icon(QSystemTrayIcon::ActivationReason reason){   
    //* Right click will show the context Menu above system tray
    //* Following will popup menu with single click on Top LEFT ??
    if(reason == QSystemTrayIcon::Trigger){
        QPoint p = QCursor::pos();
        trayIcon->contextMenu()->popup(p);
    }
}



//=================================================
//== Show Setup Wizard
void MainObject::show_setup_wizard(){
	SetupWizard *setupWizard = new SetupWizard(this);
	if(setupWizard->exec()){
		qDebug() << "closed";
		emit(reload_paths());
	}
}

//=============================================================================
//== Logs
//=============================================================================
void MainObject::on_view_logs(){
	viewLogsWidget->show();
}

void MainObject::clear_log(QString log_name){
	viewLogsWidget->clear_log(log_name);
}

void MainObject::add_log(QString log_name, QString data){
	viewLogsWidget->add_log(log_name, data);
}


QString MainObject::get_fgfs_command(){
	QStringList args;

	args << QString("--fg-root=").append(settings->fgroot());

	//* Core Settings
	//args << coreSettingsWidget->get_args();

	//=== Callsign
	args << QString ("--callsign=").append(settings->value("callsign").toString());

	//=== Screen
	if(settings->value("screen_size").toString().length())
		args << QString ("--geometry=").append(settings->value("screen_size").toString());

	if(settings->value("screen_size").toBool())
		args << QString ("-disable-splash-screen");

	if( settings->value("screen_size").toBool())
		args << QString ("--enable-fullscreen");



	//=== Weather
	QString weather_method = settings->value("weather").toString();
	if(weather_method == "live") {
		//= real weather
		args << QString("--enable-real-weather-fetch");

	}else if(weather_method == "custom"){
		//= custom metar
		args << QString("--metar=").append("\"").append(settings->value("metar").toString()).append("\"");

	}else{
		//= no weather
		args << QString("--disable-real-weather-fetch");
	}


	//=== Time of Day
	QString timeofday = settings->value("timeofday").toString();
	if (timeofday != "real") {
		args << QString("--timeofday=").append(timeofday);
	}

	//=== Season
	//args << QString("--season=").append(mainObject->settings->value("season").toString());




	//== Startup , Splash, Geometry
	if(settings->value("screen_size").toString() != "default"){
		args << QString("--geometry=").append( settings->value("screen_size").toString() );
	}
	if (settings->value("screen_splash").toBool()){
		args << QString("--disable-splash-screen");
	}
	if (settings->value("screen_full").toBool()){
		args << QString("--enable-fullscreen");
	}

	//== AutoCordination
	if(settings->value("enable_auto_coordination").toBool()){
		args << QString("--enable-auto-coordination");
	}
	//+ TODO joystick


	//== Terrasync
	if (settings->terrasync_enabled()) {
		args << QString("--fg-scenery=").append(settings->terrasync_sync_data_path()).append(":").append(settings->scenery_path());
		args << QString("--atlas=socket,out,5,localhost,5505,udp");
	}


	//=== Extra Args
	QString extra = settings->value("extra_args").toString().trimmed();
	if (extra.length() > 0) {
		QStringList parts = extra.split("\n");
		if(parts.count() > 0){
			for(int i=0; i < parts.count(); i++){
				QString part = parts.at(i).trimmed();
				if(part.length() > 0){
					args << part;
				}
			}
		}
	}

	//= Log Level
	if(settings->value("log_level").toString() != "none"){
		args << QString("--log-level=").append(settings->value("log_level").toString());
	}

	//* Aircraft
	//args << aircraftWidget->get_args();


	if(settings->value("airport").toString().length() > 0){
		args << QString("--airport=").append(settings->value("airport").toString());

		QString runway_or_stand = settings->value("runway_or_stand").toString().trimmed();
		if(runway_or_stand.length() > 0){
			if(runway_or_stand == "runway"){
				args << QString("--runway=").append(settings->value("startup_position").toString());

			}else if(runway_or_stand == "stand"){
				args << QString("--parkpos=").append(settings->value("startup_position").toString());
			}
		}
	}
	//* Network
	//args << networkWidget->get_args();

	//**Advanced Options
	//args << advancedOptionsWidget->get_args();
		//exeFgfs->user_env = advancedOptionsWidget->get_env();
	   // exeFgfs->runtime = advancedOptionsWidget->get_runtime();

	//* Ai Traffic TODO
	/*
	if (enableAITraffic->isChecked()) {
		args << QString("--enable-ai-traffic");
	}else{
		args << QString("--disable-ai-traffic");
	}
	*/
	//** Enable AI models ???
	//args << QString("--enable-ai-models");
	qDebug() << args;

	return QString("");
}


//========================================================
//** Get Engiroment
QStringList MainObject::get_env(){

	QStringList args;

	QString extra = settings->value("extra_env").toString().trimmed();
	if (extra.length() > 0) {
		QStringList parts = extra.split("\n");
		if(parts.count() > 0){
			for(int i=0; i < parts.count(); i++){
				QString part = parts.at(i).trimmed();
				if(part.length() > 0){
					args << part;
				}
			}
		}
	}
	return args;
}

