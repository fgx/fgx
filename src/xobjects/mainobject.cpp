
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
	QApplication::setQuitOnLastWindowClosed(false);


	//= Processes
	processFgFs  = new XProcess(this, "fgfs");
	processTerraSync  = new XProcess(this, "terrasync");
	processFgCom  = new XProcess(this, "fgcom");

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
	lblCallsign->setStyleSheet("color: #000099; padding: 4px; font-weight: bold; background-color: white; border: 1px outset #cccccc;");
    actionCallsign->setDefaultWidget(lblCallsign);
	popupMenu->addAction(actionCallsign);

	//= Launcher Action
	actionLauncher = popupMenu->addAction(QIcon(":icon/favicon"), tr("Open Launcher..."));
	actionLauncher->setIconVisibleInMenu(true);
	connect(actionLauncher, SIGNAL(triggered()), this, SLOT(on_launcher()) );

	//= MpMap action
	actionMpMap = popupMenu->addAction(QIcon(":icon/mpmap"), tr("Open Map..."));
	actionMpMap->setIconVisibleInMenu(true);
    connect(actionMpMap, SIGNAL(triggered()), this, SLOT(on_mpmap()));

	//= MpMapX action
	QAction *actionMpXMap = popupMenu->addAction(QIcon(":icon/mpmap"), tr("Open XMap (experimental)..."));
	actionMpXMap->setIconVisibleInMenu(true);
	connect(actionMpXMap, SIGNAL(triggered()), this, SLOT(on_mpxmap()));

	//= Settings Action
	QAction *actionSetupWizard= new QAction(this);
	actionSetupWizard->setIcon(QIcon(":/icon/wizard"));
	actionSetupWizard->setText(tr("Setup..."));
	actionSetupWizard->setIconVisibleInMenu(true);
	popupMenu->addAction(actionSetupWizard);
	connect(actionSetupWizard, SIGNAL(triggered()),
			this, SLOT(show_setup_wizard())
    );

	//== View Logs
	QAction *actionViewLogs = new QAction(this);
	actionViewLogs->setIcon(QIcon(":/icon/log"));
	actionViewLogs->setText(tr("View Logs..."));
	actionViewLogs->setIconVisibleInMenu(true);
	popupMenu->addAction(actionViewLogs);
	connect(actionViewLogs, SIGNAL(triggered()),
			this, SLOT(on_view_logs())
	);

	//== Properties browseer
	actionPropsBrowser = new QAction(this);
	actionPropsBrowser->setIcon(QIcon(":/icon/props"));
	actionPropsBrowser->setText(tr("Properties Browser..."));
	actionPropsBrowser->setIconVisibleInMenu(true);
	popupMenu->addAction(actionPropsBrowser);
	connect(actionPropsBrowser, SIGNAL(triggered()),
			this, SLOT(on_properties_browser())
	);

    popupMenu->addSeparator();



	//== Quit
	actionQuit = popupMenu->addAction(QIcon(":/icon/quit"), tr("Quit"));
	actionQuit->setIconVisibleInMenu(true);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(on_quit()));

	//==================
    trayIcon->show();

	connect(this, SIGNAL(show_settings(int)), this, SLOT(on_settings(int)));

	//== Props tree
	propertiesBrowser = new PropsTreeWidget(this);
	propertiesBrowser->hide();

	//= MP Map Widget
	mpMapWidget = new MpMapWidget(this);
	mpMapWidget->hide();

	//= MP Map Widget
	mpMapXWidget = new MpMapXWidget(this);
	mpMapXWidget->hide();


	//= Log Viewer is hidden
	viewLogsWidget = new ViewLogsWidget(this);
	viewLogsWidget->hide();

	launcherWindow = new LauncherWindow(this);

	//== initialise after initial show so UI dont look frozen while cache loading etc
	QTimer::singleShot(300, this, SLOT(initialize()));

}

MainObject::~MainObject()
{
    outLog(util_getDateTimestg()+" - Application close");
}

//============================================================================
//= Initialize
void MainObject::initialize(){
	on_launcher();
}

//============================================================================
//=  Launcher window
void MainObject::on_launcher(){
	launcherWindow->show();
	launcherWindow->raise();

}


//============================================================================
//** Settings Dialog - THESE ARE DEAD FOR NOW
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
void MainObject::on_mpmap(){
	mpMapWidget->show();
	mpMapWidget->setFocus();
}

//============================================================================
//** MpMap
void MainObject::on_mpxmap(){
	mpMapXWidget->show();
	mpMapXWidget->setFocus();
}

//======================================
//== Propertes Browser
void MainObject::on_properties_browser(){
	propertiesBrowser->show();
	propertiesBrowser->setFocus();
	//propertiesBrowser->raise();
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


//=========================================================================
//== FgFs Start args = command
//=========================================================================
QStringList MainObject::get_fgfs_args(){

	QStringList args;

	args << QString("--fg-root=").append(settings->fgroot());

	//=== Callsign
	args << QString ("--callsign=").append(settings->value("callsign").toString());


	//=== Screen
	if(settings->value("screen_size").toString() != "default"){
		args << QString ("--geometry=").append(settings->value("screen_size").toString());
	}
	if(settings->value("screen_splash").toBool()){
		args << QString ("--disable-splash-screen");
	}
	if( settings->value("screen_full").toBool()){
		args << QString ("--enable-fullscreen");
	}



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


	//== Multiplayer
	if(settings->value("enable_mp").toBool()){
		//= In
		if(settings->value("in").toBool()){
			args << QString("--multiplay=in,%1,%2,%3"
								).arg(	settings->value("in_hz").toString()
								).arg(	settings->value("in_address").toString()
								).arg(	settings->value("in_port").toString()
							);
		}
		//= Out
		if(settings->value("out").toBool()){
			args << QString("--multiplay=out,%1,%2,%3"
							).arg(	settings->value("out_hz").toString()
							).arg( 	settings->value("mpserver").toString()
							).arg( settings->value("out_port").toString()
						);
		}
	}

	//== Servers
	//= Http
	if(settings->value("http").toBool()){
		args << QString("--httpd=%1").arg( settings->value("http_port").toString() );
	}
	//= Telnet
	if(settings->value("telnet").toBool()){
		args << QString("--telnet=%1").arg( settings->value("telnet_port").toString() );
	}
	//= ScreenShot
	if(settings->value("screenshot").toBool()){
		// BUG - this reports unknow option ??
		//args << QString("--jpg-httpd=%1").arg( settings->value("screenshot_port").toString() );
	}


	//=== FgCom
	if(settings->value("fgcom").toBool()){
		args << QString("--generic=socket,out,10,localhost,%1,udp,fgcom"
						).arg( settings->value("fgcom_port").toString()
						);
	}



	//=============================================================
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
	if(settings->value("aircraft").toString().length() > 0){
		args << QString("--aircraft=").append(settings->value("aircraft").toString());
	}

	//== Navigation Radio
	QStringList navkeys;
	navkeys << "nav1" << "nav2" << "com1" << "com2" << "adf";
	for(int nidx=0; nidx < navkeys.size(); nidx++){
		if(settings->value(navkeys.at(nidx)).toString().length() > 0){
			args << QString("--%1=%2").arg(navkeys.at(nidx)).arg(settings->value(navkeys.at(nidx)).toString());
		}
	}

	//=== Airports and Startup Position
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
	//qDebug() << args;
	args.sort();
	return args;
}

QString MainObject::get_fgfs_command(){
	QString command = settings->fgfs_path();
	command.append(" ").append(get_fgfs_args().join(" "));
	return command;
}

//========================================================
//** Get Enviroment
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


//========================================================
//== Start All
void MainObject::start_all(){
	start_fgfs();
	start_terrasync();
	start_fgcom();
}

//========================================================
//== Stop All
void MainObject::stop_all(){
	processFgFs->stop();
	processFgCom->stop();
	processTerraSync->stop();
}



//========================================================
//== Start FGFS
void MainObject::start_fgfs(){
	//qDebug() << get_fgfs_command();
	processFgFs->start(get_fgfs_command(), get_env() );
}

//========================================================
//== Stars FGFS
void MainObject::start_terrasync(){
	qDebug() << "Start";
	QStringList terraargs;
	terraargs << "-p" << "5505" << "-S" << "-d" << settings->terrasync_sync_data_path();

	QString terra_command_line = settings->terrasync_exe_path();
	terra_command_line.append(" ").append( terraargs.join(" ") );

	//qDebug() << terra_command_line;
	processTerraSync->start(terra_command_line, QStringList());
}

//========================================================
//== Start FgCom
void MainObject::start_fgcom(){

	QStringList args;

	args << QString("-p");
	args << settings->value("fgcom_port").toString();

	args << QString("-S");
	args << settings->value("fgcom_no").toString();

	QString command_line = settings->fgcom_exe_path();
	command_line.append(" ").append( args.join(" ") );
	//qDebug() << command_line;
	processFgCom->start(command_line, QStringList() );
}


void MainObject::quit(){
	QApplication::quit();
}



