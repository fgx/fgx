
//#include <QDebug>

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
#include <QtGui/QToolBar>

#include "xwidgets/xgroupboxes.h"

#include "launcher/launcherwindow.h"
#include "utilities/utilities.h"
#include "utilities/messagebox.h"

#include "setupwizard/setupwizard.h"


LauncherWindow::LauncherWindow(MainObject *mainOb, QWidget *parent)
	: QWidget(parent)
{
	
	
	//#####= Set to true to show all exec controls
	bool show_all_exe_controls = false;


	initializing = true;
    mainObject = mainOb;
	

	setProperty("settings_namespace", QVariant("launcher_window"));
	mainObject->settings->restoreWindow(this);

	setWindowTitle(QCoreApplication::applicationName().append(" - ").append(QCoreApplication::applicationVersion()));
	setWindowIcon(QIcon(":/icon/favicon"));


	//====================================================
	//== Outer Layout
	//====================================================
	QVBoxLayout *outerContainer = new QVBoxLayout();
	outerContainer->setContentsMargins(0, 0, 0, 0);
	outerContainer->setSpacing(0);
	setLayout(outerContainer);	
	
	//====================================================
	//== Header Banner
	//====================================================
	QString header_style("padding: 10px 0px 0px 0px; vertical-align: top");
    headerLabel = new QLabel(this);
	headerLabel->setFixedHeight(10);
	headerLabel->setStyleSheet(header_style);
	outerContainer->addWidget(headerLabel, 0);



	//=============================================
	//=== Top toolbar
	QHBoxLayout *toolbarLayout =  new QHBoxLayout();
	toolbarLayout->setContentsMargins(10, 0, 10, 0);
	outerContainer->addLayout(toolbarLayout);
	
	//== Message Label
	
	messageLabel = new XMessageLabel(this);
	messageLabel->setStyleSheet("{ font-size: 16px; }");
	messageLabel->showMessage("Welcome [Callsign]");
	toolbarLayout->addWidget(messageLabel,1);


	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setContentsMargins(10, 0, 10, 10);
	mainLayout->setSpacing(0);
	outerContainer->addLayout(mainLayout);


	//====================================================
	//** Main TabWidget with Widgets
	//====================================================
    tabWidget = new QTabWidget(this);
	mainLayout->addWidget(tabWidget);
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tab_changed(int)));


	//* Core Settings
	coreSettingsWidget = new CoreSettingsWidget(mainObject);
	tabWidget->addTab(coreSettingsWidget, tr("Core Settings"));
	//connect(coreSettingsWidget->groupBoxTerraSync, SIGNAL(clicked()), this, SLOT(on_group_box_terrasync_clicked()));

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


	//* Expert Options
	expertOptionsWidget = new ExpertOptionsWidget(mainObject);
	tabWidget->addTab( expertOptionsWidget, tr("Expert Options"));
	//connect(outputPreviewWidget->buttonCommandPreview, SIGNAL(clicked()), this, SLOT(on_command_preview()));


	mainLayout->addSpacing(10);
	mainLayout->setAlignment( Qt::AlignTop );



	//========================================================================================
	//**Bottom Bar
	//========================================================================================
	
	//* Show bottom bar
	QHBoxLayout *bottomActionLayout = new QHBoxLayout();
	bottomActionLayout->setAlignment( Qt::AlignTop );
	mainLayout->addLayout(bottomActionLayout);
	
	XGroupHBox *toolBox = new XGroupHBox(tr("Settings"));
	bottomActionLayout->addWidget(toolBox);
	
	toolBox->setStyleSheet("XGroupHBox::title { color: #000000; background-color: yellow }");
	
	//== Show setup wizard
	QToolButton *buttonShowWizard = new QToolButton();
	buttonShowWizard->setText("Set Paths");
	buttonShowWizard->setAutoRaise(true);
	buttonShowWizard->setIcon(QIcon(":/icon/path"));
	buttonShowWizard->setStyleSheet("padding: 0px;");
	buttonShowWizard->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	toolBox->addWidget(buttonShowWizard);
	connect(buttonShowWizard, SIGNAL(clicked()), mainObject, SLOT(show_setup_wizard()));
	
	//== Show Profiles
	QToolButton *buttonShowProfiles = new QToolButton();
	buttonShowProfiles->setText("Profiles");
	buttonShowProfiles->setAutoRaise(true);
	buttonShowProfiles->setIcon(QIcon(":/icon/profile"));
	buttonShowProfiles->setStyleSheet("padding: 0px;");
	buttonShowProfiles->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonShowProfiles->setEnabled(false);
	toolBox->addWidget(buttonShowProfiles);
	//connect(buttonShowProfiles, SIGNAL(clicked()), mainObject, SLOT(show_setup_profiles()));
	
	//= Show Log
	/*QToolButton *buttonShowLogs = new QToolButton(this);
	buttonShowLogs->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonShowLogs->setText("View Logs");
	buttonShowLogs->setAutoRaise(true);
	buttonShowLogs->setIcon(QIcon(":/icon/log"));
	buttonShowLogs->setStyleSheet("padding: 0px;");
	toolBox->addWidget(buttonShowLogs);
	connect(buttonShowLogs, SIGNAL(clicked()),
			mainObject, SLOT(on_view_logs())
			);*/
	
	//= Load Settings
	QToolButton *buttonLoadSettings = new QToolButton(this);
	buttonLoadSettings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonLoadSettings->setText("Load Settings");
	buttonLoadSettings->setAutoRaise(true);
	buttonLoadSettings->setIcon(QIcon(":/icon/load"));
	buttonLoadSettings->setStyleSheet("padding: 0px;");
	toolBox->addWidget(buttonLoadSettings);
	connect(buttonLoadSettings, SIGNAL(clicked()),
			this, SLOT(load_settings())
			);
	
	//= Save Settings
	QToolButton *buttonSaveSettings = new QToolButton(this);
	buttonSaveSettings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonSaveSettings->setText("Save Settings");
	buttonSaveSettings->setAutoRaise(true);
	buttonSaveSettings->setIcon(QIcon(":/icon/save"));
	buttonSaveSettings->setStyleSheet("padding: 0px;");
	toolBox->addWidget(buttonSaveSettings);
	connect(buttonSaveSettings, SIGNAL(clicked()),
			this, SLOT(save_settings())
			);
	
	bottomActionLayout->addStretch(20);

	//=============================================================
	//== Start Stop ==
	//=============================================================


	//= FgCom
	exeFgCom = new ExeControls("FgCom");
	bottomActionLayout->addWidget(exeFgCom);
	connect(exeFgCom->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_fgcom_clicked())
			);

	connect(mainObject->processFgCom, SIGNAL(running(bool)), exeFgCom, SLOT(set_running(bool)));
	connect(exeFgCom, SIGNAL(stop()), mainObject->processFgCom, SLOT(stop()));
	exeFgCom->setVisible(show_all_exe_controls);

	//= TerraSync
	exeTerraSync = new ExeControls("TerraSync");
	bottomActionLayout->addWidget(exeTerraSync);
	connect(exeTerraSync->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_terrasync_clicked())
			);

	connect(mainObject->processTerraSync, SIGNAL(running(bool)), exeTerraSync, SLOT(set_running(bool)));
	connect(exeTerraSync, SIGNAL(stop()), mainObject->processTerraSync, SLOT(stop()));
	exeTerraSync->setVisible(show_all_exe_controls);

	//= FlightGear
	exeFgfs = new ExeControls("FgFs");
	bottomActionLayout->addWidget(exeFgfs);
	connect(	exeFgfs->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_fgfs_clicked())
			);
	connect(mainObject->processFgFs, SIGNAL(running(bool)), exeFgfs, SLOT(set_running(bool)));
	connect(exeFgfs, SIGNAL(stop()), mainObject->processFgFs, SLOT(stop()));
	exeFgfs->setVisible(show_all_exe_controls);

	//= All
	exeAll = new ExeControls("FlightGear");
	bottomActionLayout->addWidget(exeAll);
	connect(	exeAll->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_all_clicked())
			);
	connect(mainObject->processFgFs, SIGNAL(running(bool)), exeAll, SLOT(set_running(bool)));
	connect(exeAll, SIGNAL(stop()), mainObject, SLOT(stop_all()) );

	//====================================================================================
	//* Problem:  Qt Has no "Show event" for a "widget", so we need to present Widgets first
	//** and then initialise. This is achieved with a timer that triggers in a moment

	// TODO  - disable widget till sane in initialize()
	//centralWidget()->setDisabled(true);

	initializing = false;
	QTimer::singleShot(300, this, SLOT(initialize()));

}

LauncherWindow::~LauncherWindow()
{
}


//=======================================================================================================================
// initialize and  Setup
//=======================================================================================================================
void LauncherWindow::initialize(){

	//= retorelast tab
	tabWidget->setCurrentIndex( mainObject->settings->value("launcher_last_tab", 0).toInt() );

	//= First load the settings
	load_settings();

	//= check paths are same
	if(!mainObject->settings->paths_sane()){
		mainObject->show_setup_wizard();
	}

	//* Paths are sane so we can initialize;
	//TODO setup wizard to import data first time
	aircraftWidget->initialize();
	airportsWidget->initialize();
	coreSettingsWidget->initialize();
}

//=======================================================================================================================
// Start Commands
//=======================================================================================================================

//= Start All
void LauncherWindow::on_start_all_clicked() {
	if(!validate()){
		return;
	}
	save_settings();
	mainObject->start_all();
}

//= Start FgFs
void LauncherWindow::on_start_fgfs_clicked() {
	if(!validate()){
		return;
	}
	save_settings();
	mainObject->start_fgfs();
}

//= Start Terrasync
void LauncherWindow::on_start_terrasync_clicked() {
	if(!validate()){
		return;
	}
	save_settings();
	mainObject->start_terrasync();
}

void LauncherWindow::on_start_fgcom_clicked() {
	if(!validate()){
		return;
	}
	save_settings();
	mainObject->start_fgcom();
}


//================================================================================
// Save Settings
//================================================================================
void LauncherWindow::save_settings()
{
	QString message("Settings saved.");
	messageLabel->showMessage(message);

	coreSettingsWidget->save_settings();
	timeWeatherWidget->save_settings();
	aircraftWidget->save_settings();
	airportsWidget->save_settings();
	networkWidget->save_settings();
	expertOptionsWidget->save_settings();
    mainObject->settings->saveWindow(this);
	mainObject->settings->sync();
	outLog("FGx: LauncherWindow::save_settings() saved ***");
	
	
	
}

//================================================================================
// Load Settings
//================================================================================
void LauncherWindow::load_settings()
{
	
	QString message("Settings loaded.");
	messageLabel->showMessage(message);
	
	coreSettingsWidget->load_settings();
	timeWeatherWidget->load_settings();
	aircraftWidget->load_settings();
	airportsWidget->load_settings();
	networkWidget->load_settings();
	expertOptionsWidget->load_settings();
	exeTerraSync->setEnabled( mainObject->settings->terrasync_enabled() );
	outLog("FGx: Settings loaded in LauncherWIndow::load_settings()");
	

}




//==============================================
//== View Buttons
void LauncherWindow::on_command_preview(){
	if(!validate()){
		return;
	}
	save_settings();
	//outputPreviewWidget->preview();
}


//=======================================================================================================================
//* Validate
//=======================================================================================================================
bool LauncherWindow::validate(){

	QString v;

	v = coreSettingsWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(coreSettingsWidget));
		messageLabel->showMessage(v);
		return false;
	}

	v = aircraftWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(aircraftWidget));
		messageLabel->showMessage(v);
		return false;
	}
	outLog("*** FGx reports: Aircraft settings ok. ***");

	v = airportsWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(airportsWidget));
		messageLabel->showMessage(v);
		return false;
	}
	outLog("*** FGx reports: Airport settings ok. ***");

	v = networkWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(networkWidget));
		messageLabel->showMessage(v);
		return false;
	}
	v = timeWeatherWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(timeWeatherWidget));
		messageLabel->showMessage(v);
		return false;
	}
	outLog("*** FGx reports: Network settings ok. ***");

	outLog("*** FGx reports: ALL SETTINGS VALID. ***");
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
	
//= quit
void LauncherWindow::on_quit(){
	save_settings(); // message save needed
	mainObject->quit();
}



//= window close
void LauncherWindow::closeEvent(QCloseEvent *event){
	if(mainObject->settings->value("first_launcher_close", "").toBool() == false){
		QMessageBox::information(this, "Minimize Notice",
								 "Fgx does not quit when this window closes, instead minimize to taskbar. You can open this window again.",
								 QMessageBox::Ok);
		mainObject->settings->setValue("first_launcher_close", true);
	}

	save_settings();
	mainObject->settings->saveWindow(this);
	mainObject->settings->sync();
	event->accept();
}

//=== Style Selected
void LauncherWindow::on_action_style(QAction *action){
	mainObject->settings->setValue("gui_style", action->text());
	QApplication::setStyle(QStyleFactory::create(action->text()));
}

//=== Tab Changes
void LauncherWindow::on_tab_changed(int tab_idx){
	if(initializing){
		return;
	}


	if(tab_idx == tabWidget->indexOf(expertOptionsWidget)){
		on_command_preview();
	}else{
		//= we dont want to restore to output preview cos it validates and will throw popup
		mainObject->settings->setValue("launcher_last_tab", tabWidget->currentIndex());
	}
}

void LauncherWindow::on_action_open_url(QAction *action){
	QUrl url(action->property("url").toString());
	QDesktopServices::openUrl(url);
}


