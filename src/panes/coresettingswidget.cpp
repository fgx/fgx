

#include <QtDebug>

#include <QtCore/QProcess>

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>

#include "panes/coresettingswidget.h"
//#include "settings/settingsdialog.h"
#include "xwidgets/xgroupboxes.h"


CoreSettingsWidget::CoreSettingsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;

	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);
	

	QVBoxLayout *layoutLeft = new QVBoxLayout();
	mainLayout->addLayout(layoutLeft, 1);


	//==================================================================
	//= Callsign
	XGroupVBox *grpCallsign = new XGroupVBox(tr("Callsign"));
	layoutLeft->addWidget(grpCallsign);

	txtCallSign = new QLineEdit(this);
	txtCallSign->setText("");
	grpCallsign->addWidget(txtCallSign);
	connect(txtCallSign, SIGNAL(textChanged(QString)), this, SLOT(on_callsign_changed(QString)) );


	//==================================================================
	//= Screen Options
	XGroupVBox *grpBoxScreen = new XGroupVBox(tr("Screen Options"));
	layoutLeft->addWidget(grpBoxScreen);

	//= Initial Size
	grpBoxScreen->addWidget(new QLabel("Initial Screen Size"));
	comboScreenSize = new QComboBox();
	comboScreenSize->addItem("-- Default --");
	comboScreenSize->addItem("800x600");
	comboScreenSize->addItem("1024x768");
	comboScreenSize->addItem("1280x1024");
	comboScreenSize->addItem("1600x900");
	grpBoxScreen->addWidget(comboScreenSize);
	
	//= Full Screen
	checkBoxFullScreenStartup = new QCheckBox(tr("Full screen at startup"));
	grpBoxScreen->addWidget(checkBoxFullScreenStartup);

	//= Disable Splash
	checkBoxDisableSplashScreen = new QCheckBox(tr("Disable Splash Screen"));
	grpBoxScreen->addWidget(checkBoxDisableSplashScreen);


	//==================================================================
	//= Controls
	XGroupVBox *grpBoxControls = new XGroupVBox(tr("Controls"));
	layoutLeft->addWidget(grpBoxControls);

	//= Enable Autocordination
	checkBoxEnableAutoCoordination = new QCheckBox("Enable Auto Coordination");
	grpBoxControls->addWidget(checkBoxEnableAutoCoordination);

	//= Joystick
	grpBoxControls->addWidget(new QLabel("Joystick"));
	comboJoystick = new QComboBox();
	grpBoxControls->addWidget(comboJoystick);

	//==================================================================
	//= Map Features
	XGroupVBox *grpMapFeatures = new XGroupVBox("Map Features");
	layoutLeft->addWidget(grpMapFeatures);

	checkBoxShowMpMap = new QCheckBox("Show Map in Browser");
	grpMapFeatures->addWidget(checkBoxShowMpMap);
	connect(checkBoxShowMpMap, SIGNAL(clicked()), this, SLOT(on_checkbox_show_mp_map()));

	comboMpMapServer = new QComboBox();
	comboMpMapServer->addItem("mpmap01.flightgear.org", "http://mpmap01.flightgear.org");
	comboMpMapServer->addItem("mpmap02.flightgear.org", "http://mpmap02.flightgear.org");
	grpMapFeatures->addWidget(comboMpMapServer);


	layoutLeft->addStretch(20);


	//===========================================================================
	//= Paths
	QVBoxLayout *layoutPaths = new QVBoxLayout();
	mainLayout->addLayout(layoutPaths, 2);

	XGroupVBox *grpFgPaths = new XGroupVBox("FlightGear Paths");
	layoutPaths->addWidget(grpFgPaths);

	//----------------------------------------------
	//= FlightGear executable
	XGroupVBox *grpFgFs = new XGroupVBox("FlightGear Executable");
	grpFgPaths->addWidget(grpFgFs);

	labelFgFsInfo = new QLabel(tr("Custom FlightGear Executable:"));
	labelFgFsInfo->setStyleSheet("width: 200px; font-size: 11px;  ");
	grpFgFs->addWidget(labelFgFsInfo, 1);

	labelFgFsPath = new QLabel(" ");
	labelFgFsPath->setStyleSheet("background-image: url(:images/background_yellow);  padding: 3px;");
	grpFgFs->addWidget(labelFgFsPath);


	//----------------------------------------------
	//= FG ROOT Data Directory
	XGroupVBox *grpFgRoot = new XGroupVBox("FlightGear Data");
	grpFgPaths->addWidget(grpFgRoot);

	labelFgRootInfo = new QLabel(tr("Custom Data Directory:"));
	labelFgRootInfo->setStyleSheet("font-size: 11px;");
	grpFgRoot->addWidget(labelFgRootInfo, 1);

	labelFgRootPath = new QLabel(" ");
	labelFgRootPath->setStyleSheet("background-image: url(:images/background_yellow);  padding: 3px;");
	grpFgRoot->addWidget(labelFgRootPath);

	layoutPaths->addStretch(20);
	
	
	//----------------------------------------------
	//= Terrasync Directory
	XGroupVBox *grpTerraSync = new XGroupVBox("TerraSync ");
	grpFgPaths->addWidget(grpTerraSync);

	labelTerraSyncInfo = new QLabel(tr(""));
	labelTerraSyncInfo->setStyleSheet("font-size: 11px;");
	grpTerraSync->addWidget(labelTerraSyncInfo, 1);

	labelTerraSyncPath = new QLabel(" ");
	labelTerraSyncPath->setStyleSheet("background-image: url(:images/background_yellow);  padding: 3px;");
	grpTerraSync->addWidget(labelTerraSyncPath);


	//= Set Paths Wizard Buttons
	QHBoxLayout *boxSetPaths = new QHBoxLayout();
	grpFgPaths->addLayout(boxSetPaths);
	boxSetPaths->addStretch(5);
	QPushButton *buttonShowWizard = new QPushButton();
	buttonShowWizard->setText("Set Paths");
	buttonShowWizard->setIcon(QIcon(":/icon/wizard"));
	boxSetPaths->addWidget(buttonShowWizard, 1);
	connect(buttonShowWizard, SIGNAL(clicked()), this, SLOT(show_setup_wizard()));


	//= Connect Mainobject (after paths wizard)
	connect(mainObject, SIGNAL(reload_paths()), this, SLOT(load_settings()));

}


void CoreSettingsWidget::on_checkbox_show_mp_map(){
	comboMpMapServer->setEnabled(checkBoxShowMpMap->isChecked());
}


//====================================================
//* Load Settings
void CoreSettingsWidget::load_settings(){

	//* Callsign
	txtCallSign->setText( mainObject->settings->value("callsign").toString() );

	//* Sartup screens
	int idx = comboScreenSize->findText(mainObject->settings->value("screen_size").toString());
	comboScreenSize->setCurrentIndex( idx == -1 ? 0 : idx );
	checkBoxFullScreenStartup->setChecked(mainObject->settings->value("screen_full").toBool());
	checkBoxDisableSplashScreen->setChecked(mainObject->settings->value("screen_splash").toBool());

	//* controls
	checkBoxEnableAutoCoordination->setChecked(mainObject->settings->value("enable_auto_coordination", false).toBool());
	// TODO - joystick

	//* mpmap
	checkBoxShowMpMap->setChecked(mainObject->settings->value("show_map_map", false).toBool());
	comboMpMapServer->setCurrentIndex(mainObject->settings->value("mpmap", 0).toInt());
	on_checkbox_show_mp_map();


	if(mainObject->settings->value("use_default_fgfs").toBool()){
		labelFgFsInfo->setText("Using Default Executable Path");
	}else{
		labelFgFsInfo->setText("Using Custom Executable Path");
	}
	labelFgFsPath->setText(mainObject->settings->fgfs_path());

	qDebug() << mainObject->settings->value("use_default_fgroot").toBool() << "=fgroot";
	if(mainObject->settings->value("use_default_fgroot").toBool()){
		labelFgRootInfo->setText("Using Default FG_ROOT");
	}else{
		labelFgRootInfo->setText("Using Custom FG_ROOT Path");
	}
	labelFgRootPath->setText(mainObject->settings->fg_root());


	labelTerraSyncInfo->setText( mainObject->settings->use_terrasync()
								 ? "Using Terrasync" : "Using Default scenery"
								 );
	if (mainObject->settings->value("use_terrasync").toBool()) {
		labelTerraSyncPath->setText( mainObject->settings->value("terrasync_path").toString() );
	} else {
		labelTerraSyncPath->setText("--");
	}

}



//====================================================
//* Save Settings
void CoreSettingsWidget::save_settings(){

	//* Callsign
	mainObject->settings->setValue("callsign", txtCallSign->text());

	//* screen
	mainObject->settings->setValue("screen_size", comboScreenSize->currentText());
	mainObject->settings->setValue("screen_full", checkBoxFullScreenStartup->isChecked());
	mainObject->settings->setValue("screen_splash", checkBoxDisableSplashScreen->isChecked());

	//* Controls
	mainObject->settings->setValue("enable_auto_coordination", checkBoxEnableAutoCoordination->isChecked());

	//* Map
	mainObject->settings->setValue("show_map_map", checkBoxShowMpMap->isChecked());
	mainObject->settings->setValue("mpmap", comboMpMapServer->currentIndex());

}



//====================================================
//* Get Args
QStringList CoreSettingsWidget::get_args(){
	QStringList args;


	if (txtCallSign->text().size()) {
		//  only add a callsign IFF there is an argument
		QString argCallsign;
		argCallsign.append("--callsign=");
		argCallsign.append(txtCallSign->text());
		args << argCallsign;
	}


	//** Startup , Splash, Geometry
	if(comboScreenSize->currentIndex() > 0){ /* zero is "--detault--" */
		args << QString("--geometry=").append( comboScreenSize->currentText() );
		}
	if (checkBoxDisableSplashScreen->isChecked()) {
		args << QString("--disable-splash-screen");
	}
	if (checkBoxFullScreenStartup->isChecked()) {
		args << QString("--enable-fullscreen");
	}

	//** Autocordination with Mouse - is this the same for joysticks ??
	args << QString(checkBoxEnableAutoCoordination->isChecked() ? "--enable-auto-coordination" : "--disable-auto-coordination");



	//** Terrasync/Multiplayermap - send on socket
	if (mainObject->settings->use_terrasync()) {
		args << QString("--fg-scenery=").append(mainObject->settings->terrasync_sync_data_path()).append(":").append(mainObject->settings->scenery_path());
		args << QString("--atlas=socket,out,5,localhost,5505,udp");
	}else{

		/* This is actually not needed because fgroot finds scenery path by default,
		but it is needed in case terrasync scenery is active, then we need both paths with ":" separated, hmpf */
		//args << QString("--fg-scenery=").append(mainObject->settings->scenery_path());
	}
	

	return args;
}



//==============================================
//** Initialize
void CoreSettingsWidget::initialize(){
	load_joysticks();
}


//==============================================
// Joysticks
//==============================================
//** Check js_demo exists
bool CoreSettingsWidget::check_js_demo_exists(){
	QProcess process;
	process.start("which", QStringList() << "js_demo", QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		Q_UNUSED(error_result);
		return ok_result.trimmed().length() != 0;
	}
	return false;
}


//** Load Joysticks
void CoreSettingsWidget::load_joysticks(){
	comboJoystick->clear();
	comboJoystick->addItem("-- None--");
	if(check_js_demo_exists() == false){
		comboJoystick->setDisabled(true);
		return;
	}
	QProcess process;
	process.start("js_demo", QStringList(), QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		Q_UNUSED(error_result);
		//* take result and split into parts
		QStringList entries = ok_result.trimmed().split("\n");
		for(int i=2; i < entries.count(); i++){ //First 2 lines are crap
			comboJoystick->addItem(entries.at(i));
		}
	}
}


void CoreSettingsWidget::on_radio_fg_path(){
	//bool use_custom = buttonGroupPaths->checkedId() == 1;
	//txtFgFs->setEnabled(use_custom);
	//txtFgRoot->setEnabled(use_custom);
}


//================================================================================
//** Path buttons Clicked - Opens the path dialog
/*
void CoreSettingsWidget::on_button_terrasync_path(){

	QString filePath = QFileDialog::getExistingDirectory(this,
														 tr("Select TerraSync Directory"),
														 txtTerraSyncPath->text(),
														 QFileDialog::ShowDirsOnly);
	if(filePath.length() > 0){
		txtTerraSyncPath->setText(filePath);
	}

	// Need to write out the terrasync dir as its used other places ;-(
	mainObject->settings->setValue("terrasync_path", txtTerraSyncPath->text());
	mainObject->settings->sync();
}
*/


/*
void CoreSettingsWidget::on_button_fgfs_path(){
	show_settings_dialog();
}


void CoreSettingsWidget::on_button_fgroot_path(){
	show_settings_dialog();
}
*/

//===============================================================
// Settings Dialog
/*
void CoreSettingsWidget::show_settings_dialog(){
	SettingsDialog *settingsDialog = new SettingsDialog(mainObject);
	if(settingsDialog->exec()){
		txtFgFs->setText(mainObject->settings->fgfs_path());
		txtFgRoot->setText(mainObject->settings->fg_root());
	}
}
*/
//===============================================================
// Settings Wizard
void CoreSettingsWidget::show_setup_wizard(){
	mainObject->show_setup_wizard();
}


//=====================================
// Callsign Changed
void CoreSettingsWidget::on_callsign_changed(QString txt){
	mainObject->lblCallsign->setText(txt);
}
