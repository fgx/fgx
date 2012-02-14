/*
 *  coresettingswidget.cpp
 *  FGx
 *
 *  © 2011 --- GPL2
 *
 */

#include "app_config.h"
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
#include "aircraft/aircraftwidget.h"

CoreSettingsWidget::CoreSettingsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;
	
	fgrootcheck = false;

	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);
	
	QVBoxLayout *layoutLeft = new QVBoxLayout();
	mainLayout->addLayout(layoutLeft, 1);
	
	QVBoxLayout *layoutMiddle = new QVBoxLayout();
	mainLayout->addLayout(layoutMiddle, 1);
	
	QVBoxLayout *layoutRight = new QVBoxLayout();
	mainLayout->addLayout(layoutRight, 1);
	
	
	//==================================================================
	//= Image
	
	QHBoxLayout *imgBox = new QHBoxLayout();
	imgBox->setAlignment(Qt::AlignBottom|Qt::AlignCenter);
	layoutLeft->addLayout(imgBox);
	
	
	imageLabel =  new QLabel(this);
	//imageLabel->setScaledContents(true);
	imageLabel->resize(300,225);
	imageLabel->setStyleSheet("background: #5b5a56");
	imageLabel->setMargin(5);
	
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());
	// There are only nine images at the moment, see resources /images
	// and images.qrc
	int randomValue = randInt(1,9);
	imageLabel->setPixmap(QPixmap(":/images/"+QString::number(randomValue)));
	
	//connect(imageLabel, SIGNAL(clicked), this, SLOT(on_imagelabel_clicked()) );
	imgBox->addWidget(imageLabel);
	imageLabel->setWhatsThis(tr("<b>Just an image</b><br>Here you see some randomly selected images. \
								Just for getting some impression from what FlightGear is bringing to you. \
								This does not reflect you aircraft selection."));
	

	//==================================================================
	//= Callsign
	XGroupVBox *grpCallsign = new XGroupVBox(tr("Your Callsign"));
	layoutLeft->addWidget(grpCallsign);

	txtCallSign = new QLineEdit(this);
	txtCallSign->setText("");
	txtCallSign->setMaxLength(7);
	grpCallsign->addWidget(txtCallSign);
	connect(txtCallSign, SIGNAL(textChanged(QString)), this, SLOT(on_callsign_changed(QString)) );
	grpCallsign->setWhatsThis(tr("<b>Your Callsign</b><br>Edit your callsign (max. 7 letters). This will be your uniqe callsign \
								 used by FlightGear. For local offline flight or in multiplayer mode."));


	//==================================================================
	//= Screen Options
	XGroupVBox *grpBoxScreen = new XGroupVBox(tr("Screen Options"));
	layoutLeft->addWidget(grpBoxScreen);
	grpBoxScreen->setWhatsThis(tr("<b>Screen Options</b><br><br>Set prefered size for the FlightGear window."));

	//= Initial Size
	grpBoxScreen->addWidget(new QLabel("Initial Screen Size"));
	comboScreenSize = new QComboBox();
	comboScreenSize->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	comboScreenSize->addItem("800 x 600 (4:3)", "800x600");
	comboScreenSize->addItem("1024 x 768 (4:3)", "1024x768");
	comboScreenSize->addItem("1280 x 720 (16:9)", "1280x720");
	comboScreenSize->addItem("1280 x 800 (16:10)", "1280x800");
	comboScreenSize->addItem("1280 x 960 (4:3)", "1280x960");
	comboScreenSize->addItem("1280 x 1024 (5:4)", "1280x1024");
	comboScreenSize->addItem("1366 x 768 (16:9)", "1366x768");
	comboScreenSize->addItem("1440 x 900 (16:10)", "1440x900");
	comboScreenSize->addItem("1600 x 900 (16:9)", "1600x900");
	comboScreenSize->addItem("1680 x 1050 (16:10)", "1680x1050");
	comboScreenSize->addItem("1920 x 1200 (16:10)", "1920x1200");
	grpBoxScreen->addWidget(comboScreenSize);
	connect(comboScreenSize, SIGNAL(currentIndexChanged(int)),
			this, SLOT(on_screensize())
	);
	
	// set screen size manually, set input mask to 4 digits and numbers only
	QHBoxLayout *screenSizeBox = new QHBoxLayout();
	lineEditScreenSizeWLabel = new QLabel("Width: ");
	lineEditScreenSizeW = new QLineEdit(this);
	lineEditScreenSizeW->setText("");
	lineEditScreenSizeW->setMaxLength(4);
	lineEditScreenSizeW->setInputMask("9999");
	connect(lineEditScreenSizeW, SIGNAL(textChanged(QString)), this, SLOT(on_screensize_changed(QString)) );
	
	lineEditScreenSizeHLabel = new QLabel("Height: ");
	lineEditScreenSizeH = new QLineEdit(this);
	lineEditScreenSizeH->setText("");
	lineEditScreenSizeH->setMaxLength(4);
	lineEditScreenSizeH->setInputMask("9999");
	connect(lineEditScreenSizeH, SIGNAL(textChanged(QString)), this, SLOT(on_screensize_changed(QString)) );
	
	screenSizeBox->addWidget(lineEditScreenSizeWLabel, 1);
	screenSizeBox->addWidget(lineEditScreenSizeW, 1);
	screenSizeBox->addWidget(lineEditScreenSizeHLabel, 1);
	screenSizeBox->addWidget(lineEditScreenSizeH, 1);
	grpBoxScreen->addLayout(screenSizeBox);
	
	//= Full Screen
	checkBoxFullScreenStartup = new QCheckBox(tr("Fullscreen mode"));
	grpBoxScreen->addWidget(checkBoxFullScreenStartup);
	checkBoxFullScreenStartup->setWhatsThis(tr("<b>Full Screen</b><br><br>Will start FlightGear in Full Screen Mode (ESC to cancel mode)."));
	connect(checkBoxFullScreenStartup, SIGNAL(clicked()), this, SLOT(on_fullscreen_changed()) );

	//= Disable Splash
	checkBoxDisableSplashScreen = new QCheckBox(tr("Disable Splash Screen"));
	grpBoxScreen->addWidget(checkBoxDisableSplashScreen);
	connect(checkBoxDisableSplashScreen, SIGNAL(clicked()), this, SLOT(on_checkbox_splash_screen()));
	checkBoxDisableSplashScreen->setWhatsThis(tr("<b>Disable Splash Screen</b><br><br>Disables FlightGear startup screen."));
	


	//===========================================================================
	//= Paths
	QVBoxLayout *layoutPaths = new QVBoxLayout();
	layoutMiddle->addLayout(layoutPaths);
	
	//----------------------------------------------
	//= FlightGear box
	
	XGroupVBox *grpFgfs = new XGroupVBox("FlightGear program and data");
	layoutPaths->addWidget(grpFgfs,2);
	
	//----------------------------------------------
	//= FlightGear executable (fgfs)
	
	labelFgfsProgram = new QLabel("Path to FlightGear program (fgfs): ");
	labelFgfsCheck = new QLabel("");
	lineEditFgFsPath = new QLineEdit("");
	lineEditFgFsPath->setFixedSize(QSize(240,20));
	buttonSetFgfsPath = new QToolButton();
	buttonSetFgfsPath->setFixedSize(20,20);
	buttonSetFgfsPath->setIcon(QIcon(":/icon/path"));
	grpFgfs->addWidget(labelFgfsProgram);
	
	QHBoxLayout *fgfsPathBox = new QHBoxLayout();
	fgfsPathBox->addWidget(lineEditFgFsPath);
	fgfsPathBox->addWidget(labelFgfsCheck);
	fgfsPathBox->addWidget(buttonSetFgfsPath);
	grpFgfs->addLayout(fgfsPathBox);
	lineEditFgFsPath->setText( mainObject->X->fgfs_path() );
	
	// "Set" clicked
	connect( buttonSetFgfsPath, SIGNAL(clicked()),this, SLOT(on_select_fgfsbutton()) );
	
	//Check if path exists, set pixmap, emit setting
	connect(lineEditFgFsPath, SIGNAL(textChanged(QString)), this, SLOT(fgfs_check_path()));
	connect(lineEditFgFsPath, SIGNAL(textChanged(QString)), this, SLOT(on_fgfs_path(QString)));
	connect(buttonSetFgfsPath, SIGNAL(clicked()), this, SLOT(fgfs_set_path()));

	//----------------------------------------------
	//= FlightGear Root Data Directory (/fgdata)

	labelFgRootData = new QLabel("Path to FlightGear data directory (fgdata): ");
	labelFgRootCheck = new QLabel("");
	lineEditFgRootPath = new QLineEdit("");
	lineEditFgRootPath->setFixedSize(QSize(240,20));
	buttonSetFgRootPath = new QToolButton();
	buttonSetFgRootPath->setFixedSize(20,20);
	buttonSetFgRootPath->setIcon(QIcon(":/icon/path"));
	grpFgfs->addWidget(labelFgRootData);
	
	QHBoxLayout *fgfsRootBox = new QHBoxLayout();
	fgfsRootBox->addWidget(lineEditFgRootPath);
	fgfsRootBox->addWidget(labelFgRootCheck);
	fgfsRootBox->addWidget(buttonSetFgRootPath);
	grpFgfs->addLayout(fgfsRootBox);
	lineEditFgRootPath->setText( mainObject->X->fgroot() );
	
	// "Set" clicked
	connect( buttonSetFgRootPath, SIGNAL(clicked()),this, SLOT(on_select_fgrootbutton()) );
	
	//Check if path exists and set pixmap
	connect(lineEditFgRootPath, SIGNAL(textChanged(QString)), this, SLOT(fgroot_check_path()));
	connect(lineEditFgRootPath, SIGNAL(textChanged(QString)), this, SLOT(on_fgroot_path(QString)));
	connect(buttonSetFgRootPath, SIGNAL(clicked()), this, SLOT(fgroot_set_path()));
	
	
	//----------------------------------------------
	//= Scenery box
	
	XGroupVBox *grpScene = new XGroupVBox("Scenery program and data");
	layoutPaths->addWidget(grpScene,2);
	
	//----------------------------------------------
	//= Terrasync Executable (terrasync)
	
	checkBoxUseTerrasync = new QCheckBox("Use online scenery download tool (Terrasync)");
	grpScene->addWidget(checkBoxUseTerrasync);
	connect(checkBoxUseTerrasync, SIGNAL(clicked()), this, SLOT(on_terrasync_enabled()));
	
	labelTerrasyncCheck = new QLabel("");
	labelTerrasyncCheck->setEnabled(false);
	lineEditTerraSyncExePath = new QLineEdit("");
	lineEditTerraSyncExePath->setFixedSize(QSize(240,20));
	lineEditTerraSyncExePath->setEnabled(false);
	buttonSetTerrasyncExePath = new QToolButton();
	buttonSetTerrasyncExePath->setFixedSize(20,20);
	buttonSetTerrasyncExePath->setIcon(QIcon(":/icon/path"));
	buttonSetTerrasyncExePath->setEnabled(false);
	
	QHBoxLayout *terraProgramBox = new QHBoxLayout();
	terraProgramBox->addWidget(lineEditTerraSyncExePath);
	terraProgramBox->addWidget(labelTerrasyncCheck);
	terraProgramBox->addWidget(buttonSetTerrasyncExePath);
	grpScene->addLayout(terraProgramBox);
	lineEditTerraSyncExePath->setText( mainObject->X->terrasync_exe_path() );
	
	// "Set" clicked
	connect( buttonSetTerrasyncExePath, SIGNAL(clicked()),this, SLOT(on_select_terrasyncexebutton()) );
	
	//Check if path exists and set pixmap
	connect(lineEditTerraSyncExePath, SIGNAL(textChanged(QString)), this, SLOT(terrasync_exe_check_path()));
	connect(lineEditTerraSyncExePath, SIGNAL(textChanged(QString)), this, SLOT(on_terrasync_path(QString)));
    // connect(buttonSetTerrasyncExePath, SIGNAL(textChanged(QString)), this, SLOT(terrasyncexe_set_path()));
	
	
	//----------------------------------------------
	//= Terrasync Directory (custom, but set by default)
	
	labelTerrasyncData = new QLabel("Path to terrasync scenery: ");
	labelTerrasyncDataCheck = new QLabel("");
	labelTerrasyncDataCheck->setEnabled(false);
	lineEditTerraSyncDataPath = new QLineEdit("");
	lineEditTerraSyncDataPath->setFixedSize(QSize(240,20));
	lineEditTerraSyncDataPath->setEnabled(false);
	buttonSetTerrasyncDataPath = new QToolButton();
	buttonSetTerrasyncDataPath->setFixedSize(20,20);
	buttonSetTerrasyncDataPath->setIcon(QIcon(":/icon/path"));
	buttonSetTerrasyncDataPath->setEnabled(false);
	grpScene->addWidget(labelTerrasyncData);
	
	QHBoxLayout *terraDataBox = new QHBoxLayout();
	terraDataBox->addWidget(lineEditTerraSyncDataPath);
	terraDataBox->addWidget(labelTerrasyncDataCheck);
	terraDataBox->addWidget(buttonSetTerrasyncDataPath);
	grpScene->addLayout(terraDataBox);
	
	lineEditTerraSyncDataPath->setText( mainObject->X->terrasync_data_path() );
	
	// "Set" clicked
	connect( buttonSetTerrasyncDataPath, SIGNAL(clicked()),this, SLOT(on_select_terrasyncdatabutton()) );
	
	//Check if path exists and set pixmap
	connect(lineEditTerraSyncDataPath, SIGNAL(textChanged(QString)), this, SLOT(terrasync_data_check_path()));
	connect(lineEditTerraSyncDataPath, SIGNAL(textChanged(QString)), this, SLOT(on_terrasync_data_path(QString)));
    // connect(buttonSetTerrasyncDataPath, SIGNAL(textChanged(QString)), this, SLOT(terrasyncdata_set_path()));
	
	layoutMiddle->addStretch(20);
	
	
	//----------------------------------------------
	//= Custom Scenery Directory (always custom)
	
	checkBoxUseCustomScenery = new QCheckBox("Use custom scenery");
	grpScene->addWidget(checkBoxUseCustomScenery);
	connect(checkBoxUseCustomScenery, SIGNAL(clicked()), this, SLOT(on_custom_scenery_enabled()));
	
	labelCustomScene = new QLabel("Path to custom scenery directory:");
	labelCustomSceneCheck = new QLabel("");
	labelCustomSceneCheck->setEnabled(false);
	lineEditCustomScenePath = new QLineEdit("");
	lineEditCustomScenePath->setFixedSize(QSize(240,20));
	lineEditCustomScenePath->setEnabled(false);
	buttonSetCustomSceneryPath = new QToolButton();
	buttonSetCustomSceneryPath->setFixedSize(20,20);
	buttonSetCustomSceneryPath->setIcon(QIcon(":/icon/path"));
	buttonSetCustomSceneryPath->setEnabled(false);
	grpScene->addWidget(labelCustomScene);
	
	QHBoxLayout *customSceneBox = new QHBoxLayout();
	customSceneBox->addWidget(lineEditCustomScenePath);
	customSceneBox->addWidget(labelCustomSceneCheck);
	customSceneBox->addWidget(buttonSetCustomSceneryPath);
	grpScene->addLayout(customSceneBox);
	
	lineEditCustomScenePath->setText( mainObject->X->custom_scenery_path() );
	
	// "Set" clicked
	connect( buttonSetCustomSceneryPath, SIGNAL(clicked()),this, SLOT(on_select_customscenerybutton()) );
	
	//Check if path exists and set pixmap
	connect(lineEditCustomScenePath, SIGNAL(textChanged(QString)), this, SLOT(custom_scenery_check_path()));
	connect(lineEditCustomScenePath, SIGNAL(textChanged(QString)), this, SLOT(on_custom_scenery_path(QString)));
    // connect(buttonSetCustomSceneryPath, SIGNAL(textChanged(QString)), this, SLOT(customscenery_set_path()));
	
	
	//===========================================================================
	//= Input
	QVBoxLayout *layoutInput = new QVBoxLayout();
	layoutRight->addLayout(layoutInput);
	
	//==================================================================
	//= Map Features
	XGroupVBox *grpMapFeatures = new XGroupVBox("Map Features");
	layoutRight->addWidget(grpMapFeatures);
	
	checkBoxShowMpMap = new QCheckBox("Show Map in Browser");
	grpMapFeatures->addWidget(checkBoxShowMpMap);
	connect(checkBoxShowMpMap, SIGNAL(clicked()), this, SLOT(on_show_mp_map()));
	
	comboMpMapServer = new QComboBox();
	comboMpMapServer->addItem("mpmap01.flightgear.org", "http://mpmap01.flightgear.org");
	comboMpMapServer->addItem("mpmap02.flightgear.org", "http://mpmap02.flightgear.org");
	comboMpMapServer->setCurrentIndex(0);
	grpMapFeatures->addWidget(comboMpMapServer);
	connect(comboMpMapServer, SIGNAL(currentIndexChanged(int)), this, SLOT(on_show_mp_map()));
	layoutRight->addStretch(20);
	
	//----------------------------------------------
	//= FGCom Executable (fgcom)
	
	//labelFGComExeInfo = new QLabel(tr("FGCom executable (fgcom):"));
	//grpFgPaths->addWidget(labelFGComExeInfo, 1);
	
	//lineEditFGComExePath = new QLineEdit("");
	//lineEditFGComExePath->setStyleSheet(style_paths);
	//lineEditFGComExePath->setFixedSize(QSize(280,20));
	//grpFgPaths->addWidget(lineEditFGComExePath);
	
	//lineEditFGComExePath->setText(mainObject->X->fgcom_exe_path());


	
	//==================================================================
	//= Controls
	
	XGroupVBox *grpBoxControls = new XGroupVBox(tr("FGx Controls Pre-check"));
	grpBoxControls->setStyleSheet("XGroupVBox::title { color: #000000; background-color: #eeeeee }");
	layoutInput->addWidget(grpBoxControls);
	grpBoxControls->addWidget(new QLabel("Joystick:"));
	
	labelInputs = new QLabel("");
	//labelInputs->setStyleSheet(style_paths);
	grpBoxControls->addWidget(labelInputs);
	grpBoxControls->setWhatsThis("<b>Controls</b><br><br>FlightGear use auto-detection to detect your input devices. This is just a pre-check of what FlightGear will see.");
	
	layoutInput->addStretch(20);

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
    startJSDemoPath = mainObject->X->getx("jsdemo_exe_path");
	// TODO Fix this macro
#ifdef Q_OS_MAC
	outLog("*** FGx Joystick Pre-Check jsdemo OSX-path defined: "+startJSDemoPath);
#else // #elif defined(Q_OS_UNIX)
    // TODO Check maybe this could be used for ALL ports?
    if (startJSDemoPath.length() == 0) {
        // see if it can be found...
        startJSDemoPath = mainObject->X->fgfs_path();
        if (startJSDemoPath.length()) { // we have a path to 'fgfs'
            startJSDemoPath = util_getBasePath(startJSDemoPath);
            startJSDemoPath.append("js_demo");
#ifdef Q_OS_WIN
            startJSDemoPath.append(".exe"); // add the windows thing
#endif // Q_OS_WIN
            if (QFile::exists(startJSDemoPath)) {   // if file exists,
                emit setx("jsdemo_exe_path",true,startJSDemoPath);  // update profile
            }
        } else {
            startJSDemoPath = "js_demo";
        }
    }
    // This will do NOTHING is no 'environment' defined
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
        //Q_UNUSED(error_result);
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
        results = "FGx Error: Unable to run '"+startJSDemoPath+"' to get Joystick list!\n";
	}
    outLog("*** FGx reports: Joystick detection results\n"+results+" ***"); // show results in LOG
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
	mainObject->launcherWindow->on_upx("--callsign=", true, txt); // change text in headerwidget
}


//=====================================
// ScreenSize combobox changed
void CoreSettingsWidget::on_screensize()
{	
	QString value(""), valueLeft(""), valueRight("");
	value = comboScreenSize->itemData(comboScreenSize->currentIndex()).toString();
	valueLeft = value.section('x', 0, 0);
	valueRight = value.section('x', 1, 1);
	lineEditScreenSizeW->setText(valueLeft);
	lineEditScreenSizeH->setText(valueRight);
}

//=====================================
// ScreenSize changed
void CoreSettingsWidget::on_screensize_changed(QString)
{
	emit setx( "--geometry=",
				true,
				lineEditScreenSizeW->text().append("x").append(lineEditScreenSizeH->text()));
	
	//emit setx( "--enable-fullscreen", checkBoxFullScreenStartup->isChecked(), "");
}

//=====================================
// FullScreen changed
void CoreSettingsWidget::on_fullscreen_changed()
{	
	emit setx( "--enable-fullscreen", checkBoxFullScreenStartup->isChecked(), "");
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

//=====================================
// Emit fgfs path
void CoreSettingsWidget::on_fgfs_path(QString txt)
{
	emit( mainObject->X->set_option("fgfs_path", true, txt));
}

//=====================================
// Emit fgroot path
void CoreSettingsWidget::on_fgroot_path(QString txt)
{
	emit( setx("fgroot_path", true, txt));
}

//=====================================
// Emit terrasync path
void CoreSettingsWidget::on_terrasync_path(QString txt)
{
	emit( setx("terrasync_exe_path", true, txt));
}

//=====================================
// Emit terrasync data path
void CoreSettingsWidget::on_terrasync_data_path(QString txt)
{
	emit( setx("terrasync_data_path", true, txt));
}

//=====================================
// Emit Custom Scenery path
void CoreSettingsWidget::on_custom_scenery_path(QString txt)
{
	emit( setx("custom_scenery_path", true, txt));
}


//=====================================
// Terrasync enabled
void CoreSettingsWidget::on_terrasync_enabled()
{
	emit( setx("terrasync_enabled", checkBoxUseTerrasync->isChecked(), ""));
	terrasync_enabled_checkstate();
}

void CoreSettingsWidget::terrasync_enabled_checkstate()
{
	lineEditTerraSyncExePath->setEnabled(checkBoxUseTerrasync->checkState());
	lineEditTerraSyncDataPath->setEnabled(checkBoxUseTerrasync->checkState());
	buttonSetTerrasyncExePath->setEnabled(checkBoxUseTerrasync->checkState());
	buttonSetTerrasyncDataPath->setEnabled(checkBoxUseTerrasync->checkState());
	labelTerrasyncCheck->setEnabled(checkBoxUseTerrasync->checkState());
	labelTerrasyncDataCheck->setEnabled(checkBoxUseTerrasync->checkState());
}

//=====================================
// Custom Scenery enabled
void CoreSettingsWidget::on_custom_scenery_enabled()
{
	emit( setx("custom_scenery_enabled", checkBoxUseCustomScenery->isChecked(), ""));
	custom_scenery_enabled_checkstate();
}

void CoreSettingsWidget::custom_scenery_enabled_checkstate()
{
	lineEditCustomScenePath->setEnabled(checkBoxUseCustomScenery->checkState());
	buttonSetCustomSceneryPath->setEnabled(checkBoxUseCustomScenery->checkState());
	labelCustomSceneCheck->setEnabled(checkBoxUseCustomScenery->checkState());
}



//======================================================================
// Update Settings
//======================================================================

void CoreSettingsWidget::fgfs_set_path() {
	emit setx("fgfs_path", true, lineEditFgFsPath->text());
}

void CoreSettingsWidget::fgroot_set_path() {
	emit setx("fgroot_path", true, lineEditFgRootPath->text());
}

void CoreSettingsWidget::terrasyncexe_set_path() {
	emit setx("terrasync_exe_path", true, lineEditTerraSyncExePath->text());
}

void CoreSettingsWidget::terrasyncdata_set_path() {
	emit setx("terrasync_data_path", true, lineEditTerraSyncDataPath->text());
}

void CoreSettingsWidget::customscenery_set_path() {
	emit setx("custom_scenery_path", true, lineEditCustomScenePath->text());
}


void CoreSettingsWidget::on_upx( QString option, bool enabled, QString value)
{
	QString valueLeft(""), valueRight("");
	
	if(option == "--callsign="){
		txtCallSign->setText(value);

	}else if(option == "--full-screen"){
		checkBoxFullScreenStartup->setChecked(enabled);
		comboScreenSize->setDisabled(enabled);
		
	}else if(option == "--geometry="){
		comboScreenSize->setCurrentIndex(comboScreenSize->findData(value));
		valueLeft = value.section('x', 0, 0);
		valueRight = value.section('x', 1, 1);
		lineEditScreenSizeW->setText(valueLeft);
		lineEditScreenSizeH->setText(valueRight);

	}else if(option == "--disable-splash-screen"){
		checkBoxDisableSplashScreen->setChecked(enabled);

	}else if(option == "show_mpmap"){
		checkBoxShowMpMap->setChecked(enabled);
		comboMpMapServer->setCurrentIndex(comboMpMapServer->findData(value));
	
	}else if(option == "fgfs_path"){
		lineEditFgFsPath->setText(mainObject->X->fgfs_path());
	
	}else if(option == "fgroot_path"){
		lineEditFgRootPath->setText(mainObject->X->fgroot());
		
	}else if (option == "terrasync_enabled") {
		checkBoxUseTerrasync->setChecked(enabled);
		terrasync_enabled_checkstate();
	
	}else if (option == "custom_scenery_enabled") {
		checkBoxUseCustomScenery->setChecked(enabled);
		custom_scenery_enabled_checkstate();
		
	}else if(option == "terrasync_exe_path"){
		lineEditTerraSyncExePath->setText(mainObject->X->terrasync_exe_path());

	}else if(option == "terrasync_data_path"){
		lineEditTerraSyncDataPath->setText(mainObject->X->terrasync_data_path());
	
	}else if(option == "custom_scenery_path"){
		lineEditCustomScenePath->setText(enabled ? value : mainObject->X->custom_scenery_path());
	}
}


//======================================================================
// Check paths and give some feedback
//======================================================================

void CoreSettingsWidget::fgfs_check_path()
{
	bool fgfs_exists = QFile::exists(lineEditFgFsPath->text());
	if (fgfs_exists) {
		labelFgfsCheck->setPixmap(QPixmap(":/icon/ok"));
	} else {
		labelFgfsCheck->setPixmap(QPixmap(":/icon/not-ok"));
	}
	
}

void CoreSettingsWidget::fgroot_check_path()
{
	// NEW: check if "version" file exists in folder.
	// Without this file also fgfs won’t start
	// Needs a version check later ...
	bool fgroot_exists = QFile::exists(lineEditFgRootPath->text()+"/version");
	if (fgroot_exists) {
		labelFgRootCheck->setPixmap(QPixmap(":/icon/ok"));
		fgrootcheck = true;
	} else {
		labelFgRootCheck->setPixmap(QPixmap(":/icon/not-ok"));
		fgrootcheck = false;
	}

}

void CoreSettingsWidget::terrasync_exe_check_path()
{
	bool terrasync_exe_exists = QFile::exists(lineEditTerraSyncExePath->text());
	if (terrasync_exe_exists) {
		labelTerrasyncCheck->setPixmap(QPixmap(":/icon/ok"));
	} else {
		labelTerrasyncCheck->setPixmap(QPixmap(":/icon/not-ok"));
	}
	
}


// Path check now NOT special case...
void CoreSettingsWidget::terrasync_data_check_path()
{
    QDir dir(lineEditTerraSyncDataPath->text());
    QString valid = ":/icon/not-ok";
    if (dir.exists())
        valid = ":/icon/ok";
    labelTerrasyncDataCheck->setPixmap(QPixmap(valid));
}

void CoreSettingsWidget::custom_scenery_check_path()
{	
	QDir custom_scenery_dir(lineEditCustomScenePath->text());
	bool custom_scenery_exists = custom_scenery_dir.exists();
	if (custom_scenery_exists) {
		labelCustomSceneCheck->setPixmap(QPixmap(":/icon/ok"));
	} else {
		labelCustomSceneCheck->setPixmap(QPixmap(":/icon/not-ok"));
	}
	
}

//======================================================================
// Set paths with buttons
//======================================================================

void CoreSettingsWidget::on_select_fgfsbutton()

{
#ifdef USE_ALTERNATE_GETFILE
    QString filePathFgfs = util_getFileName((QWidget *)this, tr("Select FlightGear binary (fgfs)"),
                                            lineEditFgFsPath->text());
#else // !#ifdef USE_ALTERNATE_GETFILE
    QString filePathFgfs = QFileDialog::getOpenFileName(this, tr("Select FlightGear binary (fgfs)"),
    lineEditFgFsPath->text());
#endif // #ifdef USE_ALTERNATE_GETFILE y/n
        if(filePathFgfs.length() > 0){
			lineEditFgFsPath->setText(filePathFgfs);
		}
		
	fgfs_check_path();
}

void CoreSettingsWidget::on_select_fgrootbutton()

{
#ifdef USE_ALTERNATE_GETFILE
    QString dirPathFgRoot = util_getDirName(this, tr("Select FlightGear data directory (fgdata)"),
                                            lineEditFgRootPath->text());
#else // !#ifdef USE_ALTERNATE_GETFILE
    QString dirPathFgRoot = QFileDialog::getExistingDirectory(this, tr("Select FlightGear data directory (fgdata)"),
                                                              lineEditFgRootPath->text(), QFileDialog::ShowDirsOnly);
#endif // #ifdef USE_ALTERNATE_GETFILE y/n
	
	if(dirPathFgRoot.length() > 0){
		lineEditFgRootPath->setText(dirPathFgRoot);
	}
	fgroot_check_path();
	if (fgrootcheck == true) {
		reload_lists();
	}
}

void CoreSettingsWidget::on_select_terrasyncexebutton()
{
    QString title = tr("Select Terrasync binary (terrasync)");
    QString previous = lineEditTerraSyncExePath->text(); // get current
    if (previous.length() == 0) { // try harder to help user
        previous = lineEditFgFsPath->text(); // if they have already set 'fgfs'
        previous = util_getBasePath(previous);
        previous.append("terrasync");       // add default exe name
#ifdef Q_OS_WIN
        previous.append(".exe");            // add windows extension
#endif
    }
#ifdef USE_ALTERNATE_GETFILE
    QString filePathTerrasync = util_getFileName((QWidget *)this, title, previous);
#else // !#ifdef USE_ALTERNATE_GETFILE
    QString filePathTerrasync = QFileDialog::getOpenFileName(this, title, previous);
#endif // #ifdef USE_ALTERNATE_GETFILE y/n

	if(filePathTerrasync.length() > 0){
		lineEditTerraSyncExePath->setText(filePathTerrasync);
	}
	
	terrasync_exe_check_path();
}

void CoreSettingsWidget::on_select_terrasyncdatabutton()

{
#ifdef USE_ALTERNATE_GETFILE
    QString dirPathTerrasyncData = util_getDirName(this, tr("Select Terrasync data directory"),
                                                   lineEditTerraSyncDataPath->text());
#else // !#ifdef USE_ALTERNATE_GETFILE
    QString dirPathTerrasyncData = QFileDialog::getExistingDirectory(this, tr("Select Terrasync data directory"),
                                                                     lineEditTerraSyncDataPath->text(), QFileDialog::ShowDirsOnly);
#endif // #ifdef USE_ALTERNATE_GETFILE y/n

	if(dirPathTerrasyncData.length() > 0){
		lineEditTerraSyncDataPath->setText(dirPathTerrasyncData);
	}
	terrasync_data_check_path();
}

void CoreSettingsWidget::on_select_customscenerybutton()

{
#ifdef USE_ALTERNATE_GETFILE
    QString dirPathCustomScenery = util_getDirName(this, tr("Select custom scenery data directory"),
                                                   lineEditCustomScenePath->text());
#else // !#ifdef USE_ALTERNATE_GETFILE
    QString dirPathCustomScenery = QFileDialog::getExistingDirectory(this, tr("Select custom scenery data directory"),
                                                                     lineEditCustomScenePath->text(), QFileDialog::ShowDirsOnly);
#endif // #ifdef USE_ALTERNATE_GETFILE y/n

    if(dirPathCustomScenery.length() > 0){
		lineEditCustomScenePath->setText(dirPathCustomScenery);
	}
	custom_scenery_check_path();
}


// Reloading aircraft and airport lists
// needed from setting new fgdata path and from clicking reset button on launcherWindow

void CoreSettingsWidget::reload_lists(){
	mainObject->launcherWindow->aircraftWidget->on_reload_cache();
	mainObject->launcherWindow->airportsWidget->on_reload_cache();
	// and get the airport names
	mainObject->launcherWindow->airportsWidget->on_loadaptdat_done();
}

// RandInt for choosing an image
int CoreSettingsWidget::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}
