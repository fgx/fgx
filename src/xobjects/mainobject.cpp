
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

	//TODO Geoff to change this to object even static
	//= init the LOG file
	util_setStdLogFile();

	//= Prederences Object
    settings = new XSettings();
	debug_mode = settings->value("DEBUG_MODE","0").toBool();

	//= Settings Model
	X = new XSettingsModel(this);
	connect(X, SIGNAL(upx(QString,bool,QString)),
			this, SLOT(on_upx(QString,bool,QString))
	);
	

	//================================================================
	//= Processes - the nub..
	//================================================================
	processFgFs  = new XProcess(this, "fgfs");
	processTerraSync  = new XProcess(this, "terrasync");
	processFgCom  = new XProcess(this, "fgcom");



	//====================================
	//= Set GLobal style
	QApplication::setStyle( QStyleFactory::create(settings->style_current()) );
	QApplication::setQuitOnLastWindowClosed(false);



	//=====================================================================================
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
	/*actionMpMap = popupMenu->addAction(QIcon(":icon/mpmap"), tr("Open Map..."));
	actionMpMap->setIconVisibleInMenu(true);
    connect(actionMpMap, SIGNAL(triggered()), this, SLOT(on_mpmap()));*/

	//= MpMapX action
	/*QAction *actionMpXMap = popupMenu->addAction(QIcon(":icon/mpmap"), tr("Open XMap (experimental)..."));
	actionMpXMap->setIconVisibleInMenu(true);
	connect(actionMpXMap, SIGNAL(triggered()), this, SLOT(on_mpxmap()));*/
	//actionMpXMap->setVisible(false);

	//= Settings Action
	QAction *actionSetupWizard= new QAction(this);
	actionSetupWizard->setIcon(QIcon(":/icon/path"));
	actionSetupWizard->setText(tr("Set paths"));
	actionSetupWizard->setIconVisibleInMenu(true);
	popupMenu->addAction(actionSetupWizard);
	connect(actionSetupWizard, SIGNAL(triggered()),
			this, SLOT(show_setup_wizard())
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


	//== View Logs
	actionViewLogs = new QAction(this);
	actionViewLogs->setIcon(QIcon(":/icon/log"));
	actionViewLogs->setText(tr("View Logs"));
	actionViewLogs->setIconVisibleInMenu(true);
	popupMenu->addAction(actionViewLogs);
	connect(actionViewLogs, SIGNAL(triggered()),
			this, SLOT(on_view_logs())
	);

	//== Fgx Debug Logs
	actionViewFgxDebug = new QAction(this);
	actionViewFgxDebug->setIcon(QIcon(":/icon/log"));
	actionViewFgxDebug->setText(tr("FGx Debug"));
	actionViewFgxDebug->setIconVisibleInMenu(true);
	popupMenu->addAction(actionViewFgxDebug);
	connect(actionViewFgxDebug, SIGNAL(triggered()),
			this, SLOT(on_view_fgx_debug())
	);

	//== Debug Enabled
	actionDebugMode = new QAction(this);
	actionDebugMode->setText(tr("Debug Mode"));
	actionDebugMode->setCheckable(true);
	actionDebugMode->setChecked(debug_mode);
	popupMenu->addAction(actionDebugMode);
	connect(actionDebugMode, SIGNAL(triggered()),
			this, SLOT(set_debug_mode())
	);


	popupMenu->addSeparator();


	//== Web Links
	QMenu *menuHelp = new QMenu(tr("Help"));
	popupMenu->addMenu(menuHelp);

	QActionGroup *actionGroupUrls = new QActionGroup(this);
	connect(actionGroupUrls, SIGNAL(triggered(QAction*)), this, SLOT(on_action_open_url(QAction*)));

	QAction *act = menuHelp->addAction(tr("Project Page"));
	act->setProperty("url", "http://code.google.com/p/fgx");
	actionGroupUrls->addAction(act);

	act = menuHelp->addAction(tr("Bugs and Issues"));
	act->setProperty("url", "http://code.google.com/p/fgx/issues/list");
	actionGroupUrls->addAction(act);

	act = menuHelp->addAction(tr("Source Code"));
	act->setProperty("url", "https://gitorious.org/fgx/fgx/");
	actionGroupUrls->addAction(act);

	menuHelp->addSeparator();

	menuHelp->addAction(tr("About FGx"), this, SLOT(on_about_fgx()));
	menuHelp->addAction(tr("About Qt"), this, SLOT(on_about_qt()));




	popupMenu->addSeparator();

	//== Quit
	actionQuit = popupMenu->addAction(QIcon(":/icon/quit"), tr("Quit"));
	actionQuit->setIconVisibleInMenu(true);
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));

	//==================
    trayIcon->show();

	connect(this, SIGNAL(show_settings(int)), this, SLOT(on_settings(int)));

	//== Props tree
	propertiesBrowser = new PropsTreeWidget(this);
	propertiesBrowser->hide();

	//= MP Map Widget
	//mpMapWidget = new MpMapWidget(this);
	//mpMapWidget->hide();

	//= FGx Map Widget
	//mpMapXWidget = new MpMapXWidget(this);
	//mpMapXWidget->hide();
	

	//= Log Viewer is hidden
	viewLogsWidget = new ViewLogsWidget(this);
	viewLogsWidget->hide();


	//= FGx Debug Widget
	fgxDebugWidget = new FgxDebugWidget(this);
	fgxDebugWidget->hide();


	launcherWindow = new LauncherWindow(this);
	launcherWindow->hide();


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
/*void MainObject::on_mpmap(){
	mpMapWidget->show();
	mpMapWidget->setFocus();
}*/

//============================================================================
//** MpMap
/*void MainObject::on_mpxmap(){
	mpMapXWidget->show();
	mpMapXWidget->setFocus();
	mpMapXWidget->initialize();
}*/

//======================================
//== Propertes Browser
void MainObject::on_properties_browser(){
	propertiesBrowser->show();
	propertiesBrowser->setFocus();
}





//=================================================
//== Tray Icon Clicked
void MainObject::on_tray_icon(QSystemTrayIcon::ActivationReason reason){   
	//= Right click will show the context Menu above system tray
	//= Following will popup menu with single click on Top LEFT ??
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
	processFgFs->start(X->get_fgfs_command_string(), X->get_fgfs_env() );
}

//========================================================
//== Stars FGFS
void MainObject::start_terrasync(){

	QStringList terraargs;
	terraargs << "-p" << "5505" << "-S" << "-d" << X->terrasync_sync_data_path();

	QString terra_command_line = X->terrasync_exe_path();
	terra_command_line.append(" ").append( terraargs.join(" ") );

	processTerraSync->start(terra_command_line, QStringList());
}

//========================================================
//== Start FgCom
void MainObject::start_fgcom(){

	QStringList args;
	QString arg;
	QString cmd;

	cmd = settings->value("fgcom_port").toString();
	cmd = cmd.simplified();
		if (cmd.size()) {
			arg = "-p";
			arg.append(cmd);
			args << arg;
		}
  
	cmd = settings->value("fgcom_no").toString();
	cmd = cmd.simplified();
		if (cmd.size()) {
			arg = "-S";
			arg.append(cmd);
			args << arg;
		}

	QString command_line = X->fgcom_exe_path();
	command_line.append(" ").append( args.join(" ") );

	processFgCom->start(command_line, QStringList() );
}


void MainObject::quit(){
	stop_all();
	QApplication::quit();
}


//==========================================================
//== Deug Related
//==========================================================
void MainObject::on_view_fgx_debug(){
	fgxDebugWidget->show();
}

void MainObject::set_debug_mode()
{
	debug_mode = actionDebugMode->isChecked();
	settings->setValue("DEBUG_MODE",debug_mode);
	emit on_debug_mode(debug_mode);
}

//void MainObject::on_debug_mode()

//===========================================================================
//** OS detection
//===========================================================================
/** \brief What OS is running
 *
 * \return a OS enum value
 */
int MainObject::runningOs() {
	#ifdef Q_WS_X11
		return MainObject::LINUX;
	#endif

	#ifdef Q_WS_MAC
		return MainObject::MAC;
	#endif

	#ifdef Q_WS_WIN
		return MainObject::WINDOWS;
	#endif

	return MainObject::UNKNOWN;
}


/** \brief Log File
 *
 * \return Absolute path to log file
 */
QString MainObject::log_file_path(){
	if(runningOs() == MainObject::WINDOWS){
		return temp_dir("/fgx-log.txt");

	}else if(runningOs() == MainObject::MAC){
		return QDir::homePath().append("/Library/Logs/fgx.log");

	}else if(runningOs() == MainObject::LINUX){
		return temp_dir("/fgx.log");

	}else{
		return "UNKNOWN log_file_path()";
	}
}




//===========================================================================
//** temp
//===========================================================================
/** \brief location if temp directoty , os specific
 *
 * Shortcut method for Qt's storageLocation()
 * \return Absolute path
 */
QString MainObject::temp_dir(){
	return QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).absolutePath();
}
/** \brief location if temp directoty , os specific with appended file/path
 *
 * Shortcut method for Qt's storageLocation()
 * \return Absolute path with appended paths.
 */
QString MainObject::temp_dir(QString append_path){
	return temp_dir().append(append_path);
}






//===========================================================================
//** Data File eg airports.txt
//===========================================================================
/** \brief Path to a data file eg data_file("airports.txt")
 *
 * \return Absolute path to the file
 */
QString MainObject::data_file(QString file_name){
	QString storedir = QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).absolutePath();

	// create path is not exist
	if(!QFile::exists(storedir)){
		QDir *dir = new QDir("");
		dir->mkpath(storedir);
	}
	return storedir.append("/").append(file_name);
}


//=======================================================================================================================
//* Help Menu Events
//=======================================================================================================================

void MainObject::on_about_fgx()
{
	QString txt;
	txt.append("<html><body><p>FGx FlightGear Launcher</b></p>");
	txt.append("<p>&copy; 2011 Yves Sablonier, Pete Morgan, Geoff McLane</p>");
	txt.append("<p><a href='http://www.gnu.org/licenses/gpl-2.0.txt'>GPLv2 and later</a></p>");
	txt.append("<p><a href='http://wiki.flightgear.org'>FlightGear Wiki</a></p>");
	txt.append("<pre>rock on and avoid mountauns near Zurich, Near geoff at PARID and pete at EGFF..</pre></body></html>");
	QMessageBox::about(0, "About FGx", txt);
}

void MainObject::on_about_qt()
{
	QMessageBox::aboutQt(0, "About Qt");
}

void MainObject::on_action_open_url(QAction *act)
{
	QUrl url(act->property("url").toString());
	QDesktopServices::openUrl( url );
}


void MainObject::on_upx(QString option, bool enabled, QString value)
{
	Q_UNUSED(enabled);
	if(option == "--callsign="){
		lblCallsign->setText(value);
	}
}
