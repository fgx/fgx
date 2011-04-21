
#include <QtCore/QProcess>

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>

#include "panes/coresettingswidget.h"

CoreSettingsWidget::CoreSettingsWidget(QWidget *parent) :
    QWidget(parent)
{

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

	XGroupVBox *grpFgPaths = new XGroupVBox("Flight Gear Paths");
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
	groupBoxFgFs = new XGroupHBox(tr("FlightGear Executable"));
	grpFgPaths->addWidget(groupBoxFgFs);

	txtFgFs = new QLineEdit();
	groupBoxFgFs->addWidget(txtFgFs, 4);

	QPushButton *buttonSetFgFs = new QPushButton();
	buttonSetFgFs->setText(tr("Set Path"));
	groupBoxFgFs->addWidget(buttonSetFgFs, 1);


	//----------------------------------------------
	//** Data Directory
	groupBoxFgRoot = new XGroupHBox(tr("Data Directory"));
	grpFgPaths->addWidget(groupBoxFgRoot);

	txtFgRoot = new QLineEdit();
	groupBoxFgRoot->addWidget(txtFgRoot, 4);

	QPushButton *buttonSetFgRoot = new QPushButton();
	buttonSetFgRoot->setText(tr("Set Path"));
	groupBoxFgRoot->addWidget(buttonSetFgRoot, 1);


	layoutPaths->addStretch(20);


	//===========================================================================
	//* TerraSync
	QVBoxLayout *layoutTerraCol = new QVBoxLayout();
	mainLayout->addLayout(layoutTerraCol, 2);

	groupBoxTerraSync = new XGroupHBox(tr("Use TerraSync for Custom Scenery"));
	groupBoxTerraSync->setCheckable(true);
	layoutTerraCol->addWidget(groupBoxTerraSync);

	txtTerraSyncPath = new QLineEdit();
	groupBoxTerraSync->addWidget(txtTerraSyncPath, 4);

	QPushButton *buttonSetTerraPath = new QPushButton();
	buttonSetTerraPath->setText(tr("Set Path"));
	groupBoxTerraSync->addWidget(buttonSetTerraPath, 1);

	layoutTerraCol->addStretch(20);


}


void CoreSettingsWidget::on_checkbox_show_mp_map(){
	comboMpMapServer->setEnabled(checkBoxShowMpMap->isChecked());
}


//====================================================
//* Load Settings
void CoreSettingsWidget::load_settings(){
	//** Sartup screens
	int idx = comboScreenSize->findText(settings.value("screen_size").toString());
	comboScreenSize->setCurrentIndex( idx == -1 ? 0 : idx );
	checkBoxFullScreenStartup->setChecked(settings.value("screen_full").toBool());
	checkBoxDisableSplashScreen->setChecked(settings.value("screen_splash").toBool());

	//* controls
	checkBoxEnableAutoCoordination->setChecked(settings.value("enable_auto_coordination", false).toBool());
	// TODO - joystick

	//* mpmap
	checkBoxShowMpMap->setChecked(settings.value("show_map_map", false).toBool());
	comboMpMapServer->setCurrentIndex(settings.value("mpmap", 0).toInt());
	on_checkbox_show_mp_map();
}



//====================================================
//* Save Settings
void CoreSettingsWidget::save_settings(){
	settings.setValue("screen_size", comboScreenSize->currentText());
	settings.setValue("screen_full", checkBoxFullScreenStartup->isChecked());
	settings.setValue("screen_splash", checkBoxDisableSplashScreen->isChecked());

	settings.setValue("enable_auto_coordination", checkBoxEnableAutoCoordination->isChecked());

	settings.setValue("show_map_map", checkBoxShowMpMap->isChecked());
	settings.setValue("mpmap", comboMpMapServer->currentIndex());
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

	//** Autocoordination with Moouse - is this the same for joysticks ??
	args << QString(checkBoxEnableAutoCoordination->isChecked() ? "--enable-auto-coordination" : "--disable-auto-coordination");

	return args;
}



//==============================================
//** Initialize
void CoreSettingsWidget::initialize(){
	load_joysticks();

}

//==============================================
//** Load Jotsticks
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
	groupBoxFgFs->setEnabled(use_custom);
	groupBoxFgRoot->setEnabled(use_custom);
}
