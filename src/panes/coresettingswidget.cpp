
#include <QtCore/QProcess>

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>

#include "panes/coresettingswidget.h"
#include "settings/settingsdialog.h"

CoreSettingsWidget::CoreSettingsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;

	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);
	

	QVBoxLayout *layoutLeft = new QVBoxLayout();
	mainLayout->addLayout(layoutLeft, 1);

	//==================================================================
	//* Screen Options
	XGroupVBox *grpBoxScreen = new XGroupVBox(tr("Screen Options"));
	layoutLeft->addWidget(grpBoxScreen);

	grpBoxScreen->addWidget(new QLabel("Initial Screen Size"));
	comboScreenSize = new QComboBox();
	comboScreenSize->addItem("800x600");
	comboScreenSize->addItem("1024x768");
	comboScreenSize->addItem("1280x1024");
	comboScreenSize->addItem("1600x900");
	grpBoxScreen->addWidget(comboScreenSize);
	

	checkBoxFullScreenStartup = new QCheckBox(tr("Full screen at startup"));
	grpBoxScreen->addWidget(checkBoxFullScreenStartup);

	checkBoxDisableSplashScreen = new QCheckBox(tr("Disable Splash Screen"));

	grpBoxScreen->addWidget(checkBoxDisableSplashScreen);


	//==================================================================
	//* Controls
	XGroupVBox *grpBoxControls = new XGroupVBox(tr("Controls"));
	layoutLeft->addWidget(grpBoxControls);

	checkBoxEnableAutoCoordination = new QCheckBox("Enable Auto Coordination");
	grpBoxControls->addWidget(checkBoxEnableAutoCoordination);

	grpBoxControls->addWidget(new QLabel("Joystick"));
	comboJoystick = new QComboBox();
	grpBoxControls->addWidget(comboJoystick);

	//==================================================================
	//* Map Features
	XGroupVBox *grpMapFeatures = new XGroupVBox("Map Features");
	layoutLeft->addWidget(grpMapFeatures);

	checkBoxShowMpMap = new QCheckBox("Show Multiplayer Map");
	grpMapFeatures->addWidget(checkBoxShowMpMap);
	connect(checkBoxShowMpMap, SIGNAL(clicked()), this, SLOT(on_checkbox_show_mp_map()));

	comboMpMapServer = new QComboBox();
	comboMpMapServer->addItem("mpmap01.flightgear.org", "http://mpmap01.flightgear.org");
	comboMpMapServer->addItem("mpmap02.flightgear.org", "http://mpmap02.flightgear.org");
	grpMapFeatures->addWidget(comboMpMapServer);


	layoutLeft->addStretch(20);


	//===========================================================================
	//* Paths
	QVBoxLayout *layoutPaths = new QVBoxLayout();
	mainLayout->addLayout(layoutPaths, 2);

	XGroupVBox *grpFgPaths = new XGroupVBox("FlightGear Paths");
	layoutPaths->addWidget(grpFgPaths);

	buttonGroupPaths = new QButtonGroup(this);
	buttonGroupPaths->setExclusive(true);
	connect(buttonGroupPaths, SIGNAL(buttonClicked(int)), this, SLOT(on_radio_fg_path()));

	QRadioButton *radioFgUseDefault = new QRadioButton(tr("Use Default"));
	grpFgPaths->addWidget(radioFgUseDefault);
	buttonGroupPaths->addButton(radioFgUseDefault, 0);

	QRadioButton *radioFgUseCustom = new QRadioButton(tr("Use Custom"));
	grpFgPaths->addWidget(radioFgUseCustom);
	buttonGroupPaths->addButton(radioFgUseCustom, 1);


	//----------------------------------------------
	//** FlightGear executable
	
	QLabel *FgFsPathLabel = new QLabel(tr("Custom FlightGear Executable:"));
	FgFsPathLabel->setStyleSheet("width: 200px; font-size: 11px; text-transform: uppercase; margin-top: 15px;");
	grpFgPaths->addWidget(FgFsPathLabel, 1);
	txtFgFs = new QLineEdit();
	//txtFgFs->setMargin(4);
	txtFgFs->setStyleSheet("background-image: url(:images/background_yellow); font-size: 10px;");
	grpFgPaths->addWidget(txtFgFs);


	//----------------------------------------------
	//** Data Directory
	
	QLabel *FgRootPathLabel = new QLabel(tr("Custom Data Directory:"));
	FgRootPathLabel->setStyleSheet("font-size: 11px; text-transform: uppercase; margin-top: 15px;");
	grpFgPaths->addWidget(FgRootPathLabel, 1);
	txtFgRoot = new QLineEdit();
	//txtFgRoot->setMargin(4);
	txtFgRoot->setStyleSheet("background-image: url(:images/background_yellow); font-size: 10px;");
	grpFgPaths->addWidget(txtFgRoot);

	layoutPaths->addStretch(20);
	
	
	//----------------------------------------------
	//** Set Path Button
	QPushButton *buttonSetPaths = new QPushButton();
	buttonSetPaths->setText(tr("Set Paths"));
	grpFgPaths->addWidget(buttonSetPaths, 1);
	connect(buttonSetPaths, SIGNAL(clicked()), this, SLOT(show_settings_dialog()));


	//===========================================================================
	//* TerraSync
	QVBoxLayout *layoutTerraCol = new QVBoxLayout();
	mainLayout->addLayout(layoutTerraCol, 2);

	groupBoxTerraSync = new XGroupHBox(tr("Download scenery via Terrasync"));
	groupBoxTerraSync->setCheckable(true);
	layoutTerraCol->addWidget(groupBoxTerraSync);

	txtTerraSyncPath = new QLineEdit();
	groupBoxTerraSync->addWidget(txtTerraSyncPath, 4);

	QPushButton *buttonSetTerraPath = new QPushButton();
	buttonSetTerraPath->setText(tr("Set Path"));
	groupBoxTerraSync->addWidget(buttonSetTerraPath, 1);
	connect(buttonSetTerraPath, SIGNAL(clicked()), this, SLOT(on_button_terrasync_path()));
	layoutTerraCol->addStretch(20);


}


void CoreSettingsWidget::on_checkbox_show_mp_map(){
	comboMpMapServer->setEnabled(checkBoxShowMpMap->isChecked());
}


//====================================================
//* Load Settings
void CoreSettingsWidget::load_settings(){
	//** Sartup screens
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

	//* Paths
	txtFgFs->setText(mainObject->settings->fgfs_path());
	txtFgRoot->setText(mainObject->settings->fg_root());


	groupBoxTerraSync->setChecked(mainObject->settings->value("use_terrasync").toBool());
	txtTerraSyncPath->setText( mainObject->settings->value("terrasync_sync_path").toString() );

}



//====================================================
//* Save Settings
void CoreSettingsWidget::save_settings(){

	//* screen
	mainObject->settings->setValue("screen_size", comboScreenSize->currentText());
	mainObject->settings->setValue("screen_full", checkBoxFullScreenStartup->isChecked());
	mainObject->settings->setValue("screen_splash", checkBoxDisableSplashScreen->isChecked());

	//* Controls
	mainObject->settings->setValue("enable_auto_coordination", checkBoxEnableAutoCoordination->isChecked());
	//TODO joystick

	//* Map
	mainObject->settings->setValue("show_map_map", checkBoxShowMpMap->isChecked());
	mainObject->settings->setValue("mpmap", comboMpMapServer->currentIndex());

	//* Paths
	mainObject->settings->setValue("use_terrasync", groupBoxTerraSync->isChecked());
	mainObject->settings->setValue("terrasync_sync_path", txtTerraSyncPath->text());

}



//====================================================
//* Get Args
QStringList CoreSettingsWidget::get_args(){
	QStringList args;

	//** Startup , Splash, Geometry
	args << QString("--geometry=").append( comboScreenSize->currentText() );
	if (checkBoxDisableSplashScreen->isChecked()) {
		args << QString("--disable-splash-screen");
	}
	if (checkBoxFullScreenStartup->isChecked()) {
		args << QString("--enable-fullscreen");
	}

	//** Autocordination with Mouse - is this the same for joysticks ??
	args << QString(checkBoxEnableAutoCoordination->isChecked() ? "--enable-auto-coordination" : "--disable-auto-coordination");



	//** Terrasync - send on socket
	if (groupBoxTerraSync->isChecked()) {
		args << QString("--atlas=socket,out,5,localhost,5505,udp");
		args << QString("--fg-scenery=").append(txtTerraSyncPath->text()).append(":").append(mainObject->settings->scenery_path());
	}
	
	
	/* This is actually not needed because fgroot finds scenery path by default, 
	but it is needed in case terrasync scenery is active, then we need both paths with ":" separated, hmpf */
	else{
		args << QString("--fg-scenery=").append(mainObject->settings->scenery_path());
	}

	return args;
}



//==============================================
//** Initialize
void CoreSettingsWidget::initialize(){
	load_joysticks();

}

//==============================================
//** Load Joysticks
void CoreSettingsWidget::load_joysticks(){
	comboJoystick->clear();
	comboJoystick->addItem("-- None--");
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
	bool use_custom = buttonGroupPaths->checkedId() == 1;
	txtFgFs->setEnabled(use_custom);
	txtFgRoot->setEnabled(use_custom);
}


//================================================================================
//** Path buttons Clicked - Opens the path dialog
void CoreSettingsWidget::on_button_terrasync_path(){

	QString filePath = QFileDialog::getExistingDirectory(this,
														 tr("Select TerraSync Directory"),
														 txtTerraSyncPath->text(),
														 QFileDialog::ShowDirsOnly);
	if(filePath.length() > 0){
		txtTerraSyncPath->setText(filePath);
	}

	//* Need to write out the terrasync dir as its used other places ;-(
	mainObject->settings->setValue("terrasync_path", txtTerraSyncPath->text());
	mainObject->settings->sync();
}




void CoreSettingsWidget::on_button_fgfs_path(){
	show_settings_dialog();
}


void CoreSettingsWidget::on_button_fgroot_path(){
	show_settings_dialog();
}


//===============================================================
// Settings Dialog
void CoreSettingsWidget::show_settings_dialog(){
	SettingsDialog *settingsDialog = new SettingsDialog(mainObject);
	if(settingsDialog->exec()){
		txtFgFs->setText(mainObject->settings->fgfs_path());
		txtFgRoot->setText(mainObject->settings->fg_root());
	}
}

