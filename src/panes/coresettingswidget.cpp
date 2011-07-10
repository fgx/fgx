

//#include <QtDebug>

#include <QtCore/QProcess>

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>

#include "panes/coresettingswidget.h"
#include "xwidgets/xgroupboxes.h"
#include "utilities/helpers.h"


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
	comboScreenSize->addItem("-- Default --", "default");
	comboScreenSize->addItem("800 x 600", "800x600");
	comboScreenSize->addItem("1024 x 768", "1024x768");
	comboScreenSize->addItem("1280 x 1024", "1280x1024");
	comboScreenSize->addItem("1600 x 900", "1600x900");
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

	QString style_paths("font-family: Andale mono, Lucida Console, monospace; font-size: 12px; padding: 3px; background-color: #eeeeee; border: 1px solid #dddddd ");

	//----------------------------------------------
	//= FlightGear executable

	labelFgFsInfo = new QLabel(tr("Custom FlightGear Executable (fgfs):"));
	grpFgPaths->addWidget(labelFgFsInfo, 1);

	labelFgFsPath = new QLabel(" ");
	labelFgFsPath->setStyleSheet(style_paths);
	grpFgPaths->addWidget(labelFgFsPath);


	//----------------------------------------------
	//= FG ROOT Data Directory

	labelFgRootInfo = new QLabel(tr("Custom FlightGear Data Directory (fgdata):"));
	grpFgPaths->addWidget(labelFgRootInfo, 1);

	labelFgRootPath = new QLabel(" ");
	labelFgRootPath->setStyleSheet(style_paths);
	grpFgPaths->addWidget(labelFgRootPath);

	layoutPaths->addStretch(20);
	
	
	//----------------------------------------------
	//= Terrasync Directory

	labelTerraSyncInfo = new QLabel(tr(""));
	grpFgPaths->addWidget(labelTerraSyncInfo, 1);

	labelTerraSyncDataPath = new QLabel("");
	labelTerraSyncDataPath->setStyleSheet(style_paths);
	grpFgPaths->addWidget(labelTerraSyncDataPath);

	//= Connect Mainobject (after paths wizard)
	connect(mainObject, SIGNAL(reload_paths()), this, SLOT(load_settings()));

}


void CoreSettingsWidget::on_checkbox_show_mp_map(){
	comboMpMapServer->setEnabled(checkBoxShowMpMap->isChecked());
}


//====================================================
//* Load Settings
void CoreSettingsWidget::load_settings(){

	//= Callsign
	txtCallSign->setText( mainObject->settings->value("callsign").toString() );

	//= Sartup screen
	Helpers::select_combo(comboScreenSize, mainObject->settings->value("screen_size").toString());
	checkBoxFullScreenStartup->setChecked(mainObject->settings->value("screen_full").toBool());
	checkBoxDisableSplashScreen->setChecked(mainObject->settings->value("screen_splash").toBool());

	//= controls
	checkBoxEnableAutoCoordination->setChecked(mainObject->settings->value("enable_auto_coordination", false).toBool());
	// TODO - joystick

	//* mpmap
	checkBoxShowMpMap->setChecked(mainObject->settings->value("show_map_map", false).toBool());
	comboMpMapServer->setCurrentIndex(mainObject->settings->value("mpmap", 0).toInt());
	on_checkbox_show_mp_map();


	if(mainObject->settings->value("fgfs_use_default").toBool()){
		labelFgFsInfo->setText("Using Default Executable Path:");
	}else{
		labelFgFsInfo->setText("Using Custom Executable Path:");
	}
	labelFgFsPath->setText(mainObject->settings->fgfs_path());

	if(mainObject->settings->value("fgroot_use_default").toBool()){
		labelFgRootInfo->setText("Using Default Data Path (FG_ROOT):");
	}else{
		labelFgRootInfo->setText("Using Custom Data Path (FG_ROOT):");
	}
	labelFgRootPath->setText(mainObject->settings->fgroot());


	labelTerraSyncInfo->setText( mainObject->settings->terrasync_enabled()
								 ? "Using Terrasync to directory below" : "Using default scenery at path below"
								 );
	if (mainObject->settings->value("terrasync_enabled").toBool()) {
		labelTerraSyncDataPath->setText( mainObject->settings->terrasync_sync_data_path());
	} else {
		labelTerraSyncDataPath->setText("kein true");
	}

}



QString CoreSettingsWidget::validate(){
	if(txtCallSign->text().trimmed().length() == 0){
		txtCallSign->setFocus();
		return QString("Callsign is empty");
	}
	return QString("");
}

//====================================================
//= Save Settings
void CoreSettingsWidget::save_settings(){

	//= Callsign
	mainObject->settings->setValue("callsign", txtCallSign->text());

	//= screen
	mainObject->settings->setValue("screen_size", comboScreenSize->itemData(comboScreenSize->currentIndex()));
	mainObject->settings->setValue("screen_full", checkBoxFullScreenStartup->isChecked());
	mainObject->settings->setValue("screen_splash", checkBoxDisableSplashScreen->isChecked());

	//= Controls
	mainObject->settings->setValue("enable_auto_coordination", checkBoxEnableAutoCoordination->isChecked());

	//= Map
	mainObject->settings->setValue("show_map_map", checkBoxShowMpMap->isChecked());
	mainObject->settings->setValue("mpmap", comboMpMapServer->currentIndex());

}




//==============================================
//== Initialize
void CoreSettingsWidget::initialize(){
	load_joysticks();
}


//==============================================
//== Joysticks
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


//==============================================
//== Load Joysticks
void CoreSettingsWidget::load_joysticks(){
	comboJoystick->clear();
	comboJoystick->addItem("-- None--");
	QString find = "Joystick ";
	QString none = "not detected";
	QString results;
	QProcess process;
	int count = 0;
	process.start("js_demo", QStringList(), QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished(3000);
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		Q_UNUSED(error_result);
		//* take result and split into parts
		QStringList entries = ok_result.trimmed().split("\n");
		for(int i=2; i < entries.count(); i++){ //First 2 lines are crap
			QString line = entries.at(i).trimmed();
			if (line.indexOf(find) == 0) {
				line = line.mid(find.size()+2).trimmed();
				if (line.at(0) == QChar('"'))
					line = line.mid(1);
				if (line.at(line.size()-1) == QChar('"'))
					line = line.mid(0,line.size()-1);
				if (line != none) {
					results += "Found \""+line+"\"\n";
					count++;
				}
				// *TBD* Maybe NOT add 'not detected' entries???
				comboJoystick->addItem(line);
			}
		}
	} else {
		results = "Unable to run 'js_demo' to get Joystick list!\n";
	}
	//outLog("Joystick detection results\n"+results,0); // show results in LOG
	// *TBD* Maybe if count == 0, disable or hide the comboJoystick
	if (count == 0) {
		// comboJoystick->hide(); // *TBD* Choice to be made
	}
}


void CoreSettingsWidget::on_radio_fg_path(){
	//bool use_custom = buttonGroupPaths->checkedId() == 1;
	//txtFgFs->setEnabled(use_custom);
	//txtFgRoot->setEnabled(use_custom);
}




//=====================================
// Callsign Changed
void CoreSettingsWidget::on_callsign_changed(QString txt){
	Q_UNUSED(txt);
	save_settings();
	mainObject->set_callsign();
}
