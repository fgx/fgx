

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
	
	QVBoxLayout *layoutMiddle = new QVBoxLayout();
	mainLayout->addLayout(layoutMiddle, 1);
	
	QVBoxLayout *layoutRight = new QVBoxLayout();
	mainLayout->addLayout(layoutRight, 1);
	

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
	lineEditFgFsPath->setFixedSize(QSize(280,20));
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
	
	//Check if path exists and set pixmap
	connect(lineEditFgFsPath, SIGNAL(textChanged(QString)), this, SLOT(fgfs_check_path()));
	connect(lineEditFgFsPath, SIGNAL(textChanged(QString)), this, SLOT(fgfs_set_path()));

	//----------------------------------------------
	//= FlightGear Root Data Directory (/fgdata)

	labelFgRootData = new QLabel("Path to FlightGear data directory (fgdata): ");
	labelFgRootCheck = new QLabel("");
	lineEditFgRootPath = new QLineEdit("");
	lineEditFgRootPath->setFixedSize(QSize(280,20));
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
	connect(lineEditFgRootPath, SIGNAL(textChanged(QString)), this, SLOT(fgroot_set_path()));
	
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
	lineEditTerraSyncExePath->setFixedSize(QSize(280,20));
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
	connect(lineEditTerraSyncExePath, SIGNAL(textChanged(QString)), this, SLOT(terrasyncexe_set_path()));
	
	
	//----------------------------------------------
	//= Terrasync Directory (custom, but set by default)
	
	labelTerrasyncData = new QLabel("Path to terrasync scenery: ");
	labelTerrasyncDataCheck = new QLabel("");
	labelTerrasyncDataCheck->setEnabled(false);
	lineEditTerraSyncDataPath = new QLineEdit("");
	lineEditTerraSyncDataPath->setFixedSize(QSize(280,20));
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
	connect(lineEditTerraSyncDataPath, SIGNAL(textChanged(QString)), this, SLOT(terrasyncdata_set_path()));
	
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
	lineEditCustomScenePath->setFixedSize(QSize(280,20));
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
	connect(lineEditCustomScenePath, SIGNAL(textChanged(QString)), this, SLOT(customscenery_set_path()));
	
	
	//===========================================================================
	//= Input
	QVBoxLayout *layoutInput = new QVBoxLayout();
	layoutRight->addLayout(layoutInput);
	
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

//=====================================
// Terrasync enabled
void CoreSettingsWidget::on_terrasync_enabled()
{
	emit( setx("terrasync_enabled", checkBoxUseTerrasync->isChecked(), ""));
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
	//labelFgFsInfo->setText("Using FlightGear path (fgfs)");
	lineEditFgFsPath->setText(mainObject->X->fgfs_path());

	//= fgroot path
	//labelFgRootInfo->setText("Using fgroot path");
	lineEditFgRootPath->setText(mainObject->X->fgroot());
		
	//= terrasync exe path
	lineEditTerraSyncExePath->setText(mainObject->X->terrasync_exe_path());

	//= terrasync data path
	lineEditTerraSyncDataPath->setText(mainObject->X->terrasync_data_path());
		
	//= terrasync data path
	//lineEditFGComExePath->setText(mainObject->X->fgcom_exe_path());
	
	
	//= custom scenery
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
	bool fgroot_exists = QFile::exists(lineEditFgRootPath->text());
	if (fgroot_exists) {
		labelFgRootCheck->setPixmap(QPixmap(":/icon/ok"));
	} else {
		labelFgRootCheck->setPixmap(QPixmap(":/icon/not-ok"));
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


// Path check special case, not full path is used in settings, base dir is $HOME
// So this checks against $HOME/path/to/terrasync/data
void CoreSettingsWidget::terrasync_data_check_path()
{
	QDir homepath(QDir::homePath());
	QString homepathString(QDir::homePath());
	homepathString.append(lineEditTerraSyncDataPath->text());
	bool terrasync_data_exists = homepath.exists(homepathString);
	if (terrasync_data_exists) {
		labelTerrasyncDataCheck->setPixmap(QPixmap(":/icon/ok"));
	} else {
		labelTerrasyncDataCheck->setPixmap(QPixmap(":/icon/not-ok"));
	}
	
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
// Set paths via Buttons
//======================================================================

void CoreSettingsWidget::on_select_fgfsbutton()

{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select FlightGear binary (fgfs)"),
	lineEditFgFsPath->text());
		if(filePath.length() > 0){
			lineEditFgFsPath->setText(filePath);
		}
		
	fgfs_check_path();
}

void CoreSettingsWidget::on_select_fgrootbutton()

{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select FlightGear data folder"),
	lineEditFgRootPath->text(), QFileDialog::ShowDirsOnly);
	
	if(dirPath.length() > 0){
		lineEditFgRootPath->setText(dirPath);
	}
	fgroot_check_path();
}

void CoreSettingsWidget::on_select_terrasyncexebutton()

{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select FlightGear binary (fgfs)"),
													lineEditTerraSyncExePath->text());
	if(filePath.length() > 0){
		lineEditTerraSyncExePath->setText(filePath);
	}
	
	terrasync_exe_check_path();
}

void CoreSettingsWidget::on_select_terrasyncdatabutton()

{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Terrasync binary path (terrasync)"),
														lineEditTerraSyncDataPath->text(), QFileDialog::ShowDirsOnly);
	
	if(dirPath.length() > 0){
		lineEditTerraSyncDataPath->setText(dirPath);
	}
	terrasync_data_check_path();
}

void CoreSettingsWidget::on_select_customscenerybutton()

{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Terrasync binary path (terrasync)"),
														lineEditCustomScenePath->text(), QFileDialog::ShowDirsOnly);
	
	if(dirPath.length() > 0){
		lineEditCustomScenePath->setText(dirPath);
	}
	custom_scenery_check_path();
}
