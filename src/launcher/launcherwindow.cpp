
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


#include "launcher/launcherwindow.h"


LauncherWindow::LauncherWindow(MainObject *mainOb, QWidget *parent)
	: QWidget(parent)
{

    mainObject = mainOb;

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
	QApplication::setStyle( QStyleFactory::create(mainObject->settings->value("gui_style","Macintosh (aqua)").toString()) );
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
	menuHelp->addAction(tr("About FGX"), this, SLOT(on_about_fgx()));
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

	//* FgCom
	exeFgCom = new ExeControls("FgCom", "fgcom");
	bottomActionLayout->addWidget(exeFgCom);
	connect(	exeFgCom->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_fgcom_clicked())
			);
	bottomActionLayout->addStretch(10);
	
	//* TerraSync
	exeTerraSync = new ExeControls("TerraSync", "terrasync");
	bottomActionLayout->addWidget(exeTerraSync);
	connect(	exeTerraSync->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_terrasync_clicked())
			);
	bottomActionLayout->addStretch(10);
	
	//* FlightGear
	exeFgfs = new ExeControls("FlightGear", "fgfs");
	bottomActionLayout->addWidget(exeFgfs);
	connect(	exeFgfs->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_fgfs_clicked())
			);
	bottomActionLayout->addStretch(10);

	//* Settings: load/save
	
	QVBoxLayout *buttlay = new QVBoxLayout();
	bottomActionLayout->addLayout(buttlay);
	
	QLabel *settingsLabel = new QLabel(this);
	settingsLabel->setText("Settings:");
	buttlay->addWidget(settingsLabel, 0);
	
	buttonLoadSettings = new QPushButton();
	buttonLoadSettings->setText(tr(" Load "));
	buttonLoadSettings->setIcon(QIcon(":/icon/refresh"));
	buttlay->addWidget(buttonLoadSettings);
	connect(buttonLoadSettings, SIGNAL(clicked()), this, SLOT(load_settings()));
	
	bottomActionLayout->addStretch(10);

	buttonSaveSettings = new QPushButton();
	buttonSaveSettings->setText(tr(" Save "));
	buttonSaveSettings->setIcon(QIcon(":/icon/save"));
	buttlay->addWidget(buttonSaveSettings);
	connect(buttonSaveSettings, SIGNAL(clicked()), this, SLOT(save_settings()));
	
	
	// Bottom Statusbar 
	statusBar = new XStatusBar();
	statusBar->setMinimumHeight(30);
	statusBar->setContentsMargins(10,0,0,0);
	
	mainLayout->addSpacing(10);
	
	mainLayout->addWidget(statusBar);


	//=================================================
	//** Restore Last tab
	tabWidget->setCurrentIndex( mainObject->settings->value("launcher_last_tab", 0).toInt() );
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tab_changed(int)));

	//====================================================================================
	//* Problem:  Qt Has no "Show event" for a "widget", so we need to present Widgets first
	//** and then initialise. This is achieved with a timer that triggers in a moment

	// TODO  - disable widget till sane in initialize()
	//centralWidget()->setDisabled(true);
	QTimer::singleShot(500, this, SLOT(initialize()));

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
// Start FlightGear
//=======================================================================================================================
void LauncherWindow::on_start_fgfs_clicked() {

	QString command = mainObject->settings->fgfs_path();

	QString command_line = QString(command).append(" ").append(fg_args());
	outputPreviewWidget->txtPreviewOutput->setPlainText(command_line);

	if(coreSettingsWidget->checkBoxShowMpMap->isChecked()){
		QUrl mapUrl(coreSettingsWidget->comboMpMapServer->itemData(coreSettingsWidget->comboMpMapServer->currentIndex()).toString());
		mapUrl.addQueryItem("follow", networkWidget->txtCallSign->text());
		QDesktopServices::openUrl(mapUrl);
	}
	exeFgfs->start(command_line);
}



//=======================================================================================================================
// Start Terrasync
//=======================================================================================================================
void LauncherWindow::on_start_terrasync_clicked(){

	QStringList args;
	args << "-p" << "5505" << "-S" << "-d" << mainObject->settings->terrasync_sync_path(); 
	QString command_line = mainObject->settings->terrasync_exe_path();
	command_line.append(" ").append(args.join(" "));
	qDebug() << command_line;
	
	exeTerraSync->start(command_line);
	
}



//=======================================================================================================================
// Start FGCom
//=======================================================================================================================
void LauncherWindow::on_start_fgcom_clicked(){
	QString command_line = mainObject->settings->fgcom_exe_path();
	command_line.append(" ");
	QStringList args;
	//args << mainObject->settings->value("fgcom_no").toString() << mainObject->settings->value("fgcom_port").toString();
	args << mainObject->settings->value("fgcom_no").toString() << "-f910";
	command_line.append( args.join(" ") );
	qDebug() << command_line;
	exeFgCom->start(command_line);
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
	statusBar->showMessage("Settings Saved", 6000);
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

	statusBar->showMessage("Settings Loaded", 6000);


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
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		outputPreviewWidget->txtPreviewOutput->setPlainText(ok_result);
	}
}


//=======================================================================================================================
//* Validate
//=======================================================================================================================
bool LauncherWindow::validate(){
	int TIMEOUT = 5000;
	QString v;

	v = aircraftWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(aircraftWidget));
		statusBar->showError(v, TIMEOUT);
		return false;
	}
	qDebug() << "aircrafts ok";

	v = airportsWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(airportsWidget));
		statusBar->showError(v, TIMEOUT);
		return false;
	}
	qDebug() << "airports ok";

	v = networkWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(networkWidget));
		statusBar->showError(v, TIMEOUT);
		return false;
	}
	qDebug() << "network ok";

	if(coreSettingsWidget->groupBoxTerraSync->isChecked() && coreSettingsWidget->txtTerraSyncPath->text().length() == 0){
		tabWidget->setCurrentIndex( tabWidget->indexOf(coreSettingsWidget) );
		coreSettingsWidget->txtTerraSyncPath->setFocus();
		statusBar->showError("Need a Terrasync directory", TIMEOUT);
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
	txt.append("<html><body><p>FGX FlightGear Launcher</b></p>");
	txt.append("<p>&copy; 2011 Yves Sablonier, Pete Morgan, Geoff McLane</p>");
	txt.append("<p><a href='http://www.gnu.org/licenses/gpl-2.0.txt'>GPLv2 and later</a></p>");
	txt.append("<p><a href='http://wiki.flightgear.org'>FlightGear Wiki</a></p>");
	txt.append("</body></html>");
	QMessageBox::about(this, "About FGX", txt);
}

void LauncherWindow::on_about_qt(){
	QMessageBox::aboutQt(this, "About Qt");
}



//=======================================================================================================================
//* Misc Events
//=======================================================================================================================
	
// quit
void LauncherWindow::on_quit(){
	
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
	qDebug() << "last=" << tabWidget->currentIndex();
	if(idx == tabWidget->indexOf(outputPreviewWidget)){
		on_command_preview();
	}
}

void LauncherWindow::on_action_open_url(QAction *action){
	qDebug() << "URL";
	QUrl url(action->property("url").toString());
	QDesktopServices::openUrl(url);
}
