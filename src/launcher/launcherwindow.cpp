
//#include <QDebug>

#include <QtCore/QUrl>
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtGui/QWhatsThis>


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
	//outerContainer->setSpacing(0);
	setLayout(outerContainer);	
	
	
	//== Message Label
	
	headerWidget = new HeaderWidget(mainObject);
	outerContainer->addWidget(headerWidget,0);
	
	
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setContentsMargins(10, 0, 10, 10);
	outerContainer->addLayout(mainLayout);


	//====================================================
	//** Main TabWidget with Widgets
	//====================================================
    tabWidget = new QTabWidget(this);
	tabWidget->setObjectName("launcher_tabs");
	mainLayout->addWidget(tabWidget);
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tab_changed(int)));


	//* Core Settings
	coreSettingsWidget = new CoreSettingsWidget(mainObject);
	tabWidget->addTab(coreSettingsWidget, tr("Core Settings"));
	//connect(coreSettingsWidget->groupBoxTerraSync, SIGNAL(clicked()), this, SLOT(on_group_box_terrasync_clicked()));


	//* Aircraft Widget
	aircraftWidget = new AircraftWidget(mainObject);
	tabWidget->addTab(aircraftWidget, tr("Aircraft"));
	//#connect(aircraftWidget, SIGNAL(set_arg(QString,QString,QString)), this, SLOT(set_arg(QString,QString,QString)));

	//** Position Tab
	airportsWidget = new AirportsWidget(mainObject);
	tabWidget->addTab(  airportsWidget, tr("Position"));
	
	//* Time / Weather Widget
	timeWeatherWidget = new TimeWeatherWidget(mainObject);
	tabWidget->addTab(timeWeatherWidget, tr("Time and Weather"));


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
	
	//= Load Settings
	QToolButton *buttonLoadSettings = new QToolButton(this);
	buttonLoadSettings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonLoadSettings->setText("Load Profile");
	buttonLoadSettings->setAutoRaise(true);
	buttonLoadSettings->setIcon(QIcon(":/icon/load"));
	buttonLoadSettings->setStyleSheet("padding: 0px;");
	toolBox->addWidget(buttonLoadSettings);
	connect(buttonLoadSettings, SIGNAL(clicked()),
			this, SLOT(load_settings())
			);
	connect(buttonLoadSettings, SIGNAL(clicked()),
			mainObject->X, SLOT(read_ini())
	);

	//= Save Settings
	QToolButton *buttonSaveSettings = new QToolButton(this);
	buttonSaveSettings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonSaveSettings->setText("Save Profile");
	buttonSaveSettings->setAutoRaise(true);
	buttonSaveSettings->setIcon(QIcon(":/icon/save"));
	buttonSaveSettings->setStyleSheet("padding: 0px;");
	toolBox->addWidget(buttonSaveSettings);
	connect(buttonSaveSettings, SIGNAL(clicked()),
			this, SLOT(save_settings())
			);
	connect(buttonSaveSettings, SIGNAL(clicked()),
			mainObject->X, SLOT(write_ini())
	);

	bottomActionLayout->addStretch(20);
	
	
	//== Help Box
	XGroupHBox *helpBox = new XGroupHBox(tr("Help"));
	helpBox->setStyleSheet("XGroupHBox::title { color: #000000; background-color: #ffff00 }");
	bottomActionLayout->addWidget(helpBox);
	
	//= Whats this button
	buttonWhatsThis = new QToolButton();
	buttonWhatsThis->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonWhatsThis->setText("Whats this");
	buttonWhatsThis->setAutoRaise(true);
	buttonWhatsThis->setIcon(QIcon(":/icon/help"));
	buttonWhatsThis->setStyleSheet("padding: 0px;");
	helpBox->addWidget(buttonWhatsThis);
	connect(buttonWhatsThis, SIGNAL(clicked()), this, SLOT(on_whats_this()));
	
	bottomActionLayout->addStretch(100);

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

	//= TerraSync
	exeTerraSync = new ExeControls("TerraSync");
	bottomActionLayout->addWidget(exeTerraSync);
	connect(exeTerraSync->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_terrasync_clicked())
			);

	connect(mainObject->processTerraSync, SIGNAL(running(bool)), exeTerraSync, SLOT(set_running(bool)));
	connect(exeTerraSync, SIGNAL(stop()), mainObject->processTerraSync, SLOT(stop()));

	//= FlightGear
	exeFgfs = new ExeControls("FgFs");
	bottomActionLayout->addWidget(exeFgfs);
	connect(	exeFgfs->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_fgfs_clicked())
			);
	connect(mainObject->processFgFs, SIGNAL(running(bool)), exeFgfs, SLOT(set_running(bool)));
	connect(exeFgfs, SIGNAL(stop()), mainObject->processFgFs, SLOT(stop()));

	//= All
	exeAll = new ExeControls("FlightGear");
	exeAll->setStyleSheet("QGroupBox::title { color: #ffffff; background-color: #007eff }");
	bottomActionLayout->addWidget(exeAll);
	connect(	exeAll->buttonStart, SIGNAL(clicked()),
			this, SLOT(on_start_all_clicked())
			);
	connect(mainObject->processFgFs, SIGNAL(running(bool)), exeAll, SLOT(set_running(bool)));
	connect(exeAll, SIGNAL(stop()), mainObject, SLOT(stop_all()) );

	on_debug_mode(); //= Show hide exe widgets

	//====================================================================================
	//* Problem:  Qt Has no "Show event" for a "widget", so we need to present Widgets first
	//** and then initialise. This is achieved with a timer that triggers in a moment

	// TODO  - disable widget till sane in initialize()
	//centralWidget()->setDisabled(true);

	initializing = false;
	QTimer::singleShot(300, this, SLOT(initialize()));

	//headerWidget->setText("Callsign - KSFO - AIRPORT");
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));
	connect(mainObject, SIGNAL(on_debug_mode(bool)), this, SLOT(on_debug_mode()));

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
	mainObject->X->read_ini();

	//= check paths are sane
	if(!mainObject->X->paths_sane()){
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
	qDebug() << "pre validate";
	if(!validate()){
		return;
	}
	qDebug() << "save";
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
	headerWidget->showMessage(message);

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
	headerWidget->showMessage(message);
	
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
		headerWidget->showMessage(v);
		return false;
	}

	v = aircraftWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(aircraftWidget));
		headerWidget->showMessage(v);
		return false;
	}
	outLog("*** FGx reports: Aircraft settings ok. ***");

	v = airportsWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(airportsWidget));
		headerWidget->showMessage(v);
		return false;
	}
	outLog("*** FGx reports: Airport settings ok. ***");

	v = networkWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(networkWidget));
		headerWidget->showMessage(v);
		return false;
	}
	v = timeWeatherWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(timeWeatherWidget));
		headerWidget->showMessage(v);
		return false;
	}
	outLog("*** FGx reports: Network settings ok. ***");

	outLog("*** FGx reports: ALL SETTINGS VALID. ***");
	return true;
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


//=== Tab Changes
void LauncherWindow::on_tab_changed(int tab_idx){
	if(initializing){
		return;
	}


	if(tab_idx == tabWidget->indexOf(expertOptionsWidget)){
		//on_command_preview();
	}else{
		//= we dont want to restore to output preview cos it validates and will throw popup
		mainObject->settings->setValue("launcher_last_tab", tabWidget->currentIndex());
	}
}

void LauncherWindow::on_action_open_url(QAction *act){
	QUrl url(act->property("url").toString());
	QDesktopServices::openUrl(url);
}

void LauncherWindow::on_whats_this() {
	QWhatsThis::enterWhatsThisMode();
}

void LauncherWindow::on_upx(QString option, bool enabled, QString value)
{
	Q_UNUSED(enabled);
	Q_UNUSED(value);	
	
	if(option == "--callsign=" || option == "--airport=" || option == "--aircraft="){
		QString header = QString("<font color=#ff0000>%1</font> with Aircraft <b>%2</b> at Airport <b>%3</b>").arg( mainObject->X->getx("--callsign=")
									).arg( mainObject->X->getx("--aircraft=")
									).arg( mainObject->X->getx("--airport="));
		headerWidget->setText( header );
	}
}


void LauncherWindow::on_debug_mode()
{
	exeFgfs->setVisible(mainObject->debug_mode == true);
	exeFgCom->setVisible(mainObject->debug_mode == true);
	exeTerraSync->setVisible(mainObject->debug_mode == true);
	exeAll->setVisible(mainObject->debug_mode == false);
}
