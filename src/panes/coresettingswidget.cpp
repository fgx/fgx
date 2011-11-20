

//#include <QtDebug>

#include <QtCore/QProcess>

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>
#include <QtGui/QWhatsThis>

#include "panes/coresettingswidget.h"
#include "xwidgets/xgroupboxes.h"
#include "utilities/helpers.h"
#include "utilities/utilities.h"


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
	txtCallSign->setMaxLength(7);
	grpCallsign->addWidget(txtCallSign);
	connect(txtCallSign, SIGNAL(textChanged(QString)), this, SLOT(on_callsign_changed(QString)) );


	//==================================================================
	//= Screen Options
	XGroupVBox *grpBoxScreen = new XGroupVBox(tr("Screen Options"));
	layoutLeft->addWidget(grpBoxScreen);
	grpBoxScreen->setWhatsThis(tr("<b>Screen Options</b><br><br>Set prefered size for the FlightGear window."));

	//= Initial Size
	grpBoxScreen->addWidget(new QLabel("Initial Screen Size"));
	comboScreenSize = new QComboBox();
	comboScreenSize->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	comboScreenSize->addItem("-- Default --", "default");
	comboScreenSize->addItem("800 x 600", "800x600");
	comboScreenSize->addItem("1024 x 768", "1024x768");
	comboScreenSize->addItem("1280 x 1024", "1280x1024");
	comboScreenSize->addItem("1600 x 900", "1600x900");
	grpBoxScreen->addWidget(comboScreenSize);
	connect(comboScreenSize, SIGNAL(currentIndexChanged(int)),
			this, SLOT(on_screensize())
	);

	
	//= Full Screen
	checkBoxFullScreenStartup = new QCheckBox(tr("Fullscreen mode"));
	grpBoxScreen->addWidget(checkBoxFullScreenStartup);
	checkBoxFullScreenStartup->setWhatsThis(tr("<b>Full Screen</b><br><br>Will start FlightGear in Full Screen Mode (ESC to cancel mode)."));
	connect(checkBoxFullScreenStartup, SIGNAL(clicked()), this, SLOT(on_screensize()));

	//= Disable Splash
	checkBoxDisableSplashScreen = new QCheckBox(tr("Disable Splash Screen"));
	grpBoxScreen->addWidget(checkBoxDisableSplashScreen);
	connect(checkBoxDisableSplashScreen, SIGNAL(clicked()), this, SLOT(on_checkbox_splash_screen()));
	checkBoxDisableSplashScreen->setWhatsThis(tr("<b>Disable Splash Screen</b><br><br>Disables FlightGear startup screen."));
	
	
	//==================================================================
	//= Map Features
	XGroupVBox *grpMapFeatures = new XGroupVBox("Map Features");
	layoutLeft->addWidget(grpMapFeatures);

	checkBoxShowMpMap = new QCheckBox("Show Map in Browser");
	grpMapFeatures->addWidget(checkBoxShowMpMap);
	connect(checkBoxShowMpMap, SIGNAL(clicked()), this, SLOT(on_show_mp_map()));

	comboMpMapServer = new QComboBox();
	comboMpMapServer->addItem("mpmap01.flightgear.org", "http://mpmap01.flightgear.org");
	comboMpMapServer->addItem("mpmap02.flightgear.org", "http://mpmap02.flightgear.org");
	comboMpMapServer->setCurrentIndex(0);
	grpMapFeatures->addWidget(comboMpMapServer);
	connect(comboMpMapServer, SIGNAL(currentIndexChanged(int)), this, SLOT(on_show_mp_map()));

	layoutLeft->addStretch(20);


	//===========================================================================
	//= Paths
	QVBoxLayout *layoutPaths = new QVBoxLayout();
	mainLayout->addLayout(layoutPaths, 2);

	XGroupVBox *grpFgPaths = new XGroupVBox("FlightGear Paths");
	grpFgPaths->setStyleSheet("XGroupVBox::title { color: #000000; background-color: #eeeeee }");
	layoutPaths->addWidget(grpFgPaths);
	//grpFgPaths->setWhatsThis("<b>FlightGear Paths</b><br><br>Use \"Settings\" button to change this settings."); 

	QString style_paths("font-family: Andale mono, Lucida Console, monospace; font-size: 11px; padding: 3px; background-color: #ffffff; border: 1px solid #dddddd ");

	//----------------------------------------------
	//= FlightGear executable (fgfs)

	labelFgFsInfo = new QLabel(tr("FlightGear Executable (fgfs):"));
	grpFgPaths->addWidget(labelFgFsInfo, 1);

	lineEditFgFsPath = new QLineEdit("");
	lineEditFgFsPath->setStyleSheet(style_paths);
	lineEditFgFsPath->setFixedSize(QSize(300,20));
	grpFgPaths->addWidget(lineEditFgFsPath);
	lineEditFgFsPath->setText( mainObject->X->fgfs_path() );


	//----------------------------------------------
	//= FlightGear Root Data Directory (/fgdata)

	labelFgRootInfo = new QLabel(tr("FlightGear Data Directory (fgdata):"));
	grpFgPaths->addWidget(labelFgRootInfo, 1);

	lineEditFgRootPath = new QLineEdit("");
	lineEditFgRootPath->setStyleSheet(style_paths);
	lineEditFgRootPath->setFixedSize(QSize(300,20));
	grpFgPaths->addWidget(lineEditFgRootPath);
	lineEditFgRootPath->setText( mainObject->X->fgroot() );
	
	//----------------------------------------------
	//= Terrasync Executable (terrasync)
	
	labelTerraSyncExeInfo = new QLabel(tr("Terrasync executable (terrasync):"));
	grpFgPaths->addWidget(labelTerraSyncExeInfo, 1);
	
	lineEditTerraSyncExePath = new QLineEdit("");
	lineEditTerraSyncExePath->setStyleSheet(style_paths);
	lineEditTerraSyncExePath->setFixedSize(QSize(300,20));
	grpFgPaths->addWidget(lineEditTerraSyncExePath);
	
	lineEditTerraSyncExePath->setText( mainObject->X->terrasync_exe_path() );
	
	
	//----------------------------------------------
	//= Terrasync Directory (Custom, but set by default)
	
	labelTerraSyncInfo = new QLabel(tr("Terrasync Data path:"));
	grpFgPaths->addWidget(labelTerraSyncInfo, 1);
	
	lineEditTerraSyncDataPath = new QLineEdit("");
	lineEditTerraSyncDataPath->setStyleSheet(style_paths);
	lineEditTerraSyncDataPath->setFixedSize(QSize(300,20));
	grpFgPaths->addWidget(lineEditTerraSyncDataPath);
	
	lineEditTerraSyncDataPath->setText( mainObject->X->terrasync_data_path() );
	
	
	
	//----------------------------------------------
	//= Custom Scenery Directory (always custom)
	
	labelCustomSceneInfo = new QLabel(tr("Custom scenery path:"));
	grpFgPaths->addWidget(labelCustomSceneInfo, 1);
	
	lineEditCustomScenePath = new QLineEdit("");
	lineEditCustomScenePath->setFixedSize(QSize(300,20));
	lineEditCustomScenePath->setStyleSheet(style_paths);
	grpFgPaths->addWidget(lineEditCustomScenePath);
	
	lineEditCustomScenePath->setText( mainObject->X->custom_scenery_path() );
	
	
	//----------------------------------------------
	//= FGCom Executable (fgcom)
	
	labelFGComExeInfo = new QLabel(tr("FGCom executable (fgcom):"));
	grpFgPaths->addWidget(labelFGComExeInfo, 1);
	
	lineEditFGComExePath = new QLineEdit("");
	lineEditFGComExePath->setStyleSheet(style_paths);
	lineEditFGComExePath->setFixedSize(QSize(300,20));
	grpFgPaths->addWidget(lineEditFGComExePath);
	
	lineEditFGComExePath->setText(mainObject->X->fgcom_exe_path());


	
	//==================================================================
	//= Controls
	
	XGroupVBox *grpBoxControls = new XGroupVBox(tr("FGx Controls Pre-check"));
	grpBoxControls->setStyleSheet("XGroupVBox::title { color: #000000; background-color: #eeeeee }");
	layoutPaths->addWidget(grpBoxControls);
	grpBoxControls->addWidget(new QLabel("Joystick:"));
	
	labelInputs = new QLabel("");
	labelInputs->setStyleSheet(style_paths);
	grpBoxControls->addWidget(labelInputs);
	grpBoxControls->setWhatsThis("<b>Controls</b><br><br>FlightGear use auto-detection to detect your input devices. This is just a pre-check of what FlightGear will see.");
	
	layoutPaths->addStretch(20);

	connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
	connect(mainObject->X, SIGNAL(upx(QString,bool,QString)), this, SLOT(on_upx(QString,bool,QString)));

}


QString CoreSettingsWidget::validate(){
	if(txtCallSign->text().trimmed().length() == 0){
		txtCallSign->setFocus();
		return QString("Callsign is empty");
	}
	return QString("");
}




//==============================================
//== Initialize
void CoreSettingsWidget::initialize(){
	load_joysticks();
}


//==============================================
//== Load Joysticks
void CoreSettingsWidget::load_joysticks(){
	labelInputs->setText("");
	QString find = "Joystick ";
	QString none = "not detected";
	QString results;
	QProcess process;
	int count = 0;
    QString startJSDemoPath;
    QStringList args;
    startJSDemoPath = "";

	// TODO Fix this macro
#ifdef Q_OS_MAC
    startJSDemoPath = mainObject->X->getx("jsdemo_exe_path");
	outLog("*** FGx Joystick Pre-Check jsdemo OSX-path defined: "+startJSDemoPath);
#elif defined(Q_OS_UNIX)
	startJSDemoPath = mainObject->X->getx("jsdemo_exe_path");
	QStringList extra_env = mainObject->X->get_fgfs_env();
    if (extra_env.size()) {
        //= append new env vars
        QStringList env = QProcess::systemEnvironment();
        env << extra_env;
        process.setEnvironment(env);
    }
#endif
    process.start(startJSDemoPath, args, QIODevice::ReadOnly);
    if(process.waitForStarted()){
		process.waitForFinished(3000);
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		outLog("*** FGx jsdemo joystick pre-check result: "+ok_result);
		if (error_result > 0) {
		outLog("*** FGx jsdemo error results: "+error_result);
		}
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
                    if (count == 0)
                        labelInputs->setText(line);
                    count++;
				}
			}
		}
	} else {
		results = "FGx Error: Unable to run 'js_demo' to get Joystick list!\n";
	}
	//outLog("*** FGx reports: Joystick detection results\n"+results+" ***",0); // show results in LOG
	if (count == 0) {
		labelInputs->setEnabled(false);
		labelInputs->setText("No Joystick detected. Check logs");
	}
}



//=====================================
// Callsign Changed
void CoreSettingsWidget::on_callsign_changed(QString txt)
{
	emit( setx("--callsign=", true, txt ));
}


//=====================================
// ScreenSize changed
void CoreSettingsWidget::on_screensize()
{
	if(comboScreenSize->currentIndex() == 0){
		emit setx("--geometry=", false, "# no #");
	}else{
		emit setx( "--geometry=",
			   checkBoxFullScreenStartup->isChecked() == false,
					comboScreenSize->itemData(comboScreenSize->currentIndex()).toString()
			   );
	}
	emit setx( "--full-screen", checkBoxFullScreenStartup->isChecked(), "");
}


//=====================================
// SplashScreen Changed
void CoreSettingsWidget::on_checkbox_splash_screen()
{
	emit setx("--disable-splash-screen", checkBoxDisableSplashScreen->isChecked(), "");
}



//=====================================
// Show Mp Map
void CoreSettingsWidget::on_show_mp_map(){
	comboMpMapServer->setEnabled(checkBoxShowMpMap->isChecked());
	emit setx(	"show_mpmap",
				checkBoxShowMpMap->isChecked(),
				comboMpMapServer->itemData(comboMpMapServer->currentIndex()).toString()
				);
}



//======================================================================
// Update Settings
//======================================================================
void CoreSettingsWidget::on_upx( QString option, bool enabled, QString value)
{
	//qDebug() << "op_upx";
	if(option == "--callsign="){
		txtCallSign->setText(value);

	}else if(option == "--full-screen"){
		checkBoxFullScreenStartup->setChecked(enabled);
		comboScreenSize->setDisabled(enabled);

	}else if(option == "--disable-splash-screen"){
		checkBoxDisableSplashScreen->setChecked(enabled);


	}else if(option == "show_mpmap"){
		checkBoxShowMpMap->setChecked(enabled);
		comboMpMapServer->setCurrentIndex(comboMpMapServer->findData(value));

	//= Fgfs Path
	labelFgFsInfo->setText("Using FlightGear path (fgfs)");
	lineEditFgFsPath->setText(mainObject->X->fgfs_path());

	//= fgroot path
	labelFgRootInfo->setText("Using fgroot path");
	lineEditFgRootPath->setText(mainObject->X->fgroot());
		
	//= terrasync exe path
	lineEditTerraSyncExePath->setText(mainObject->X->terrasync_exe_path());

	//= terrasync data path
	lineEditTerraSyncDataPath->setText(mainObject->X->terrasync_data_path());
		
	//= terrasync data path
	lineEditFGComExePath->setText(mainObject->X->fgcom_exe_path());
	
	
	//= custom scenery
	}else if(option == "custom_scenery_path"){
	
	lineEditCustomScenePath->setText(enabled ? value : mainObject->X->custom_scenery_path());
	}
}
