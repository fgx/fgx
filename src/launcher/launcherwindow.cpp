
#include <QDebug>
#include <QtCore/QUrl>
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QProcess>

#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QDesktopServices>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QStyleFactory>
#include <QtGui/QMessageBox>

#include "xwidgets/xgroupboxes.h"


#include "launcher/launcherwindow.h"
#include "utilities/utilities.h"
#include "utilities/messagebox.h"


LauncherWindow::LauncherWindow(MainObject *mainOb, QWidget *parent)
	: QWidget(parent)
{

    mainObject = mainOb;
	
	fgfsflag = false;
	terrasyncflag = false;
	fgcomflag = false;

	setProperty("settings_namespace", QVariant("launcher_window"));
	mainObject->settings->restoreWindow(this);

	setWindowTitle(QCoreApplication::applicationName().append(" - ").append(QCoreApplication::applicationVersion()));
	setWindowIcon(QIcon(":/icon/favicon"));


	//====================================================
	//** Main Central Widget and Layout
	//====================================================

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setContentsMargins(10, 10, 10, 10);
	mainLayout->setSpacing(0);
	setLayout(mainLayout);


	//====================================================
	//** Setup Menus
	//====================================================
	QMenuBar *menuBar = new QMenuBar();
	mainLayout->addWidget(menuBar);

	//** File Menu
	QMenu *menuFile = new QMenu(tr("File"));
	menuBar->addMenu(menuFile);
	QAction *actionQuit = menuFile->addAction(QIcon(":/icon/quit"), tr("Quit"), this, SLOT(on_quit()));
	actionQuit->setIconVisibleInMenu(true);

	//** Style Menu
	QMenu *menuStyle = new QMenu(tr("Style"));
	menuBar->addMenu(menuStyle);
	// TODO make defult style from xplatform.. in mainObject->settings-> (pedro)
	#if defined(Q_OS_MAC)
		QApplication::setStyle( QStyleFactory::create(mainObject->settings->value("gui_style","Macintosh (aqua)").toString()) );
	#else
		QApplication::setStyle( QStyleFactory::create(mainObject->settings->value("gui_style","Cleanlooks").toString()) );
	#endif
	actionGroupStyle = new QActionGroup(this);
	actionGroupStyle->setExclusive(true);
	connect(actionGroupStyle, SIGNAL(triggered(QAction*)), this, SLOT(on_action_style(QAction*)));
	QStringList styles =  QStyleFactory::keys();
	for(int idx=0; idx < styles.count(); idx++){
		QString sty = QString(styles.at(idx));
		QAction *act = menuStyle->addAction( sty );
		actionGroupStyle->addAction( act );
		act->setCheckable(true);
		if(QApplication::style()->objectName() == sty.toLower()){
			act->setChecked(true);
		}
	}

	//** Help Menu
	QMenu *menuHelp = new QMenu(tr("Help"));
	menuBar->addMenu(menuHelp);

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
	
	
	//====================================================
	//** Header Banner
	//====================================================
	QString header_style("padding: 10px 0px 0px 370px; vertical-align: top;	 background-image: url(':/images/fgx-logo'); background-repeat: no-repeat; background-color: white;");
    headerLabel = new QLabel(this);
	headerLabel->setFixedHeight(80);
	headerLabel->setStyleSheet(header_style);
	mainLayout->addWidget(headerLabel, 0);

	mainLayout->addSpacing(10);


	//====================================================
	//** Main TabWidget with Widgets
	//====================================================
    tabWidget = new QTabWidget(this);
	mainLayout->addWidget(tabWidget);


	//* Core Settings
	coreSettingsWidget = new CoreSettingsWidget(mainObject);
	tabWidget->addTab(coreSettingsWidget, tr("Core Settings"));
	connect(coreSettingsWidget->groupBoxTerraSync, SIGNAL(clicked()), this, SLOT(on_group_box_terrasync_clicked()));

	//* Time / Weather Widget
	timeWeatherWidget = new TimeWeatherWidget(mainObject);
	tabWidget->addTab(timeWeatherWidget, tr("Time and Weather"));


	//* Aircraft Widget
	aircraftWidget = new AircraftWidget(mainObject);
	tabWidget->addTab(aircraftWidget, tr("Aircraft"));
	//#connect(aircraftWidget, SIGNAL(set_arg(QString,QString,QString)), this, SLOT(set_arg(QString,QString,QString)));


	//** Airports Tab
	airportsWidget = new AirportsWidget(mainObject);
	tabWidget->addTab(  airportsWidget, tr("Airports"));


	//* Network Tab
	networkWidget = new NetworkWidget(mainObject);
	tabWidget->addTab( networkWidget, tr("Network"));


	//* Advanced Options
	advancedOptionsWidget = new AdvancedOptionsWidget(mainObject);
	tabWidget->addTab( advancedOptionsWidget, tr("Advanced Options"));


	//* Output + Preview
	outputPreviewWidget = new OutputPreviewWidget(mainObject);
	tabWidget->addTab( outputPreviewWidget, tr("Output / Preview"));
	connect(outputPreviewWidget->buttonCommandPreview, SIGNAL(clicked()), this, SLOT(on_command_preview()));
	connect(outputPreviewWidget->buttonCommandHelp, SIGNAL(clicked()), this, SLOT(on_command_help()));

	mainLayout->addSpacing(10);




	//========================================================================================
	//**Bottom Bar
	//========================================================================================
	
	//* Show bottom bar
	QHBoxLayout *bottomActionLayout = new QHBoxLayout();
	mainLayout->addLayout(bottomActionLayout);
	
	
	//* Settings: load/save
	XGroupVBox *grpSettings = new XGroupVBox(tr("Settings"));
	bottomActionLayout->addWidget(grpSettings);
	
	buttonLoadSettings = new QPushButton();
	buttonLoadSettings->setMinimumSize(QSize( 100, 30));
	buttonLoadSettings->setText(tr(" Load "));
	connect(buttonLoadSettings, SIGNAL(clicked()), this, SLOT(load_settings()));
	grpSettings->addWidget(buttonLoadSettings);
	
	bottomActionLayout->addStretch(10);
	
	buttonSaveSettings = new QPushButton();
	buttonSaveSettings->setMinimumSize(QSize( 100, 30));
	buttonSaveSettings->setText(tr(" Save "));
	connect(buttonSaveSettings, SIGNAL(clicked()), this, SLOT(save_settings()));
	grpSettings->addWidget(buttonSaveSettings);
	

	//* FgCom
	exeFgCom = new ExeControls("FgCom", "fgcom");
	
	//* TerraSync
	exeTerraSync = new ExeControls("TerraSync", "terrasync");
	
	//* FlightGear
	exeFgfs = new ExeControls("FlightGear", "fgfs");
	bottomActionLayout->addWidget(exeFgfs);
	connect(	exeFgfs->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_fgfs_clicked())
			);
	connect(	exeFgfs->buttonStop, SIGNAL(clicked()),
			this, SLOT(on_stop_fgfs_clicked())
			);
	//bottomActionLayout->addStretch(10);
	
	// Bottom Statusbar 
	/*statusBar = new StatusBar();
	statusBar->setMinimumHeight(30);
	statusBar->setContentsMargins(10,0,0,0);
	
	mainLayout->addSpacing(10);
	
	mainLayout->addWidget(statusBar);*/


	//=================================================
	//** Restore Last tab
	tabWidget->setCurrentIndex( mainObject->settings->value("launcher_last_tab", 0).toInt() );
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tab_changed(int)));

	//====================================================================================
	//* Problem:  Qt Has no "Show event" for a "widget", so we need to present Widgets first
	//** and then initialise. This is achieved with a timer that triggers in a moment

	// TODO  - disable widget till sane in initialize()
	//centralWidget()->setDisabled(true);
	QTimer::singleShot(300, this, SLOT(initialize()));

}

LauncherWindow::~LauncherWindow()
{
}


//=======================================================================================================================
// initialize and  Setup
//=======================================================================================================================
void LauncherWindow::initialize(){

	//* First load the settings, and check the "paths" to fg_root and fg are sane
	load_settings();
	if(!mainObject->settings->paths_sane()){
		coreSettingsWidget->show_settings_dialog();
	}

	//* Paths are sane so we can initialize;
	//TODO setup wizard to import data first time
	aircraftWidget->initialize();
	airportsWidget->initialize();
	coreSettingsWidget->initialize();
}



//=======================================================================================================================
// Start FlightGear, TerraSync, FGCom
//=======================================================================================================================
void LauncherWindow::on_start_fgfs_clicked() {

	//* Start FlightGear (fgfs)
	
	QString command = mainObject->settings->fgfs_path();

	QString command_line = QString(command).append(" ").append(fg_args());
	outputPreviewWidget->txtPreviewOutput->setPlainText(command_line);

	if(coreSettingsWidget->checkBoxShowMpMap->isChecked()){
		QUrl mapUrl(coreSettingsWidget->comboMpMapServer->itemData(coreSettingsWidget->comboMpMapServer->currentIndex()).toString());
		mapUrl.addQueryItem("follow", coreSettingsWidget->txtCallSign->text());
		QDesktopServices::openUrl(mapUrl);
	}
	exeFgfs->start(command_line);
	fgfsflag = true;

	//* Start TerraSync
	
	if (coreSettingsWidget->groupBoxTerraSync->isChecked()) {
		QStringList terraargs;
		QString terra_sync_path = coreSettingsWidget->txtTerraSyncPath->text();
		terraargs << "-p" << "5505" << "-S" << "-d" << terra_sync_path; 
		//for debugging only: terraargs << "-p" << "5505" << "-S" << "-d" << "/Documents/TerrasyncScenery";
		QString terra_command_line = mainObject->settings->fgfs_path();
		terra_command_line.chop(4);
		terra_command_line.append("terrasync");
		terra_command_line.append(" ").append(terraargs.join(" "));
		qDebug() << terra_command_line;
		exeTerraSync->start(terra_command_line);
		terrasyncflag = true;
	}

	//* Start FGCom

	if (networkWidget->grpFgCom->isChecked()) {
		QStringList fgcomargs;
		QString fgcom_command_line = mainObject->settings->fgcom_exe_path();
		fgcom_command_line.append(" ");
	
	
		QString argPort("-p");
		argPort.append(mainObject->settings->value("fgcom_port").toString());
	
		QString argServer("-S");
		argServer.append(mainObject->settings->value("fgcom_no").toString());
	
		//FGCom default
		fgcomargs << argServer << argPort;
	
		//Echoing for live debugging purposes
		//fgcomargs << argServer << "-f910";
	
		fgcom_command_line.append(fgcomargs.join(" ") );
		qDebug() << command_line;
		exeFgCom->start(fgcom_command_line);
		fgcomflag = true;
	}

}

//=======================================================================================================================
// Stop FlightGear, TerraSync, FGCom
//=======================================================================================================================
void LauncherWindow::on_stop_fgfs_clicked() {
	
	if (fgfsflag == true){
		QString PIDfgfs = QString::number(exeFgfs->get_pid());
		if (PIDfgfs != "0") {
			exeFgfs->killproc();
			outLog("### FlightGear->fgfs PID: " + PIDfgfs + " STOPPED ###");
		}
	}
	
	if (terrasyncflag == true){
		QString PIDterra = QString::number(exeTerraSync->get_pid());
		if (PIDterra != "0") {
			exeTerraSync->killproc();
			outLog("### FlightGear->terrasync PID: " + PIDterra + " STOPPED ###");
		}
	}
	
	if (fgcomflag == true){
		QString PIDfgcom = QString::number(exeFgCom->get_pid());
		if (PIDfgcom != "0") {
			exeFgCom->killproc();
			outLog("### FlightGear->fgcom PID: " + PIDfgcom + " STOPPED ###");
		}
	}
	
}

//=======================================================================================================================
// Command Arguments
//=======================================================================================================================
QString LauncherWindow::fg_args(){


	QStringList args;

	//* fg_root
	args << QString("--fg-root=").append(mainObject->settings->fg_root());


	//* Core Settings
	args << coreSettingsWidget->get_args();

	//* Time and Weather
	args << timeWeatherWidget->get_args();

	//* Aircraft
	args << aircraftWidget->get_args();

	//* Airport, Runway Start pos
	args << airportsWidget->get_args();

	//* Network
	args << networkWidget->get_args();

	//**Advanced Options
	args << advancedOptionsWidget->get_args();
        exeFgfs->user_env = advancedOptionsWidget->get_env();
        exeFgfs->runtime = advancedOptionsWidget->get_runtime();

	//* Ai Traffic TODO
	/*
	if (enableAITraffic->isChecked()) {
		args << QString("--enable-ai-traffic");
	}else{
		args << QString("--disable-ai-traffic");
	}
	*/
	//** Enable AI models ???
	args << QString("--enable-ai-models");

	args.sort();

	//** Create the return string
	QString args_string = args.join(" ");
	return args_string;
}



//================================================================================
// Save Settings
//================================================================================
void LauncherWindow::save_settings()
{

	coreSettingsWidget->save_settings();
	timeWeatherWidget->save_settings();
	aircraftWidget->save_settings();
	airportsWidget->save_settings();
	networkWidget->save_settings();
	advancedOptionsWidget->save_settings();

	mainObject->settings->sync();
	outLog("*** FGx reports: Settings saved ***");
}

//================================================================================
// Load Settings
//================================================================================
void LauncherWindow::load_settings()
{

	coreSettingsWidget->load_settings();
	timeWeatherWidget->load_settings();
	aircraftWidget->load_settings();
	airportsWidget->load_settings();
	networkWidget->load_settings();
	advancedOptionsWidget->load_settings();

	exeTerraSync->setEnabled( mainObject->settings->value("use_terrasync").toBool() );

	//** Weather
	//int weather = mainObject->settings->value("weather").toInt();
	//timeWeatherWidget->buttonGroupMetar->button(weather)->setChecked(true);

	//timeWeatherWidget->txtMetar->setPlainText(mainObject->settings->value("metar").toString());
	//timeWeatherWidget->txtMetar->setEnabled(weather == 2);

	outLog("*** FGx reports: Settings loaded ***");


}




//==============================================
//** View Buttons
void LauncherWindow::on_command_preview(){
	if(!validate()){
		return;
	}
	QString str = QString(mainObject->settings->fgfs_path()).append(" ").append( fg_args());
	outputPreviewWidget->txtPreviewOutput->setPlainText(str);
}

void LauncherWindow::on_command_help(){
	QProcess process;
	QStringList args;
	args << "-h" << "-v" << QString("--fg-root=").append(mainObject->settings->fg_root());
	process.start(mainObject->settings->fgfs_path(), args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		QString ok_result = process.readAllStandardOutput();
		outputPreviewWidget->txtPreviewOutput->setPlainText(ok_result);
	}
}


//=======================================================================================================================
//* Validate
//=======================================================================================================================
bool LauncherWindow::validate(){
	//int TIMEOUT = 5000;
	QString v;

	v = aircraftWidget->validate();
	if(v != ""){
		//tabWidget->setCurrentIndex( tabWidget->indexOf(aircraftWidget));
		messageBox->showWindowMessage("Validation failed:<BR> Please select an Aircraft or check [x] Use Default.");
		return false;
	}
	qDebug() << "aircrafts ok";

	v = airportsWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(airportsWidget));
		messageBox->showWindowMessage("Validation failed:<BR> Please select an Airport.");
		return false;
	}
	qDebug() << "airports ok";

	v = networkWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(networkWidget));
		messageBox->showWindowMessage("Validation failed:<BR> Please check settings on Network Tab!");
		return false;
	}
	qDebug() << "network ok";

	if(coreSettingsWidget->groupBoxTerraSync->isChecked() && coreSettingsWidget->txtTerraSyncPath->text().length() == 0){
		tabWidget->setCurrentIndex( tabWidget->indexOf(coreSettingsWidget) );
		coreSettingsWidget->txtTerraSyncPath->setFocus();
		messageBox->showWindowMessage("Validation failed:<BR> Please set a path vor Terrasync Scenery!");
		return false;
	}
	qDebug() << "ALL VALID";
	return true;
}

//=======================================================================================================================
//* Help Menu Events
//=======================================================================================================================
void LauncherWindow::on_about_fgx(){
	QString txt;
	txt.append("<html><body><p>FGx FlightGear Launcher</b></p>");
	txt.append("<p>&copy; 2011 Yves Sablonier, Pete Morgan, Geoff McLane</p>");
	txt.append("<p><a href='http://www.gnu.org/licenses/gpl-2.0.txt'>GPLv2 and later</a></p>");
	txt.append("<p><a href='http://wiki.flightgear.org'>FlightGear Wiki</a></p>");
	txt.append("</body></html>");
	QMessageBox::about(this, "About FGx", txt);
}

void LauncherWindow::on_about_qt(){
	QMessageBox::aboutQt(this, "About Qt");
}



//=======================================================================================================================
//* Misc Events
//=======================================================================================================================
	
// quit
void LauncherWindow::on_quit(){
	
	save_settings(); // message save needed
	QApplication::quit();

}


void LauncherWindow::on_group_box_terrasync_clicked(){
	mainObject->settings->setValue("use_terrasync", coreSettingsWidget->groupBoxTerraSync->isChecked());
	mainObject->settings->sync();
	exeTerraSync->setEnabled(coreSettingsWidget->groupBoxTerraSync->isChecked());
}

// window close
void LauncherWindow::closeEvent(QCloseEvent *event){
	Q_UNUSED(event);
	save_settings();
	mainObject->settings->saveWindow(this);
	mainObject->settings->sync();
	event->accept();
	mainObject->launcher_flag = false;
	
}

void LauncherWindow::on_action_style(QAction *action){
	mainObject->settings->setValue("gui_style", action->text());
	QApplication::setStyle(QStyleFactory::create(action->text()));
}

void LauncherWindow::on_tab_changed(int idx){
	mainObject->settings->setValue("launcher_last_tab", tabWidget->currentIndex());
	QStringList tablist;
	// put "last used tab" to log, but more human now. To Pete: do we need it at all, or not ?
	tablist << "Coresettings" << "Time and Weather" << "Aircraft" << "Airports" << "Network" << "Advanced Options" << "Output / Preview";
	outLog("*** FGx reports: last tab used = " + tablist[tabWidget->currentIndex()] + " (" + QString::number(tabWidget->currentIndex()) + ") ***");
	if(idx == tabWidget->indexOf(outputPreviewWidget)){
		on_command_preview();
	}
}

void LauncherWindow::on_action_open_url(QAction *action){
	qDebug() << "URL";
	QUrl url(action->property("url").toString());
	QDesktopServices::openUrl(url);
}


