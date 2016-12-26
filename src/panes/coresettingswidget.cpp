// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // coresettingswidget.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#include "app_config.h"
//#include <QtDebug>

#include <QProcess>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QFileDialog>
#include <QWhatsThis>
#include <QColor>

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
    strFgFsExeLabel = this->mainObject->runningOs() == OS_WINDOWS ? "fgfs.exe" : "fgfs";

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




    //==================================================================
    //= Set Callsign
    XGroupVBox *grpCallsign = new XGroupVBox(tr("Callsign"));
    layoutLeft->addWidget(grpCallsign);

    txtCallSign = new QLineEdit(this);
    txtCallSign->setText("");
    txtCallSign->setMaxLength(7);
    grpCallsign->addWidget(txtCallSign);
    connect(txtCallSign, SIGNAL(textChanged(QString)), this, SLOT(on_callsign_changed(QString)) );
    grpCallsign->setWhatsThis(tr("<b>Your Callsign</b><br>Edit your callsign (max. 7 letters). This will be your uniqe callsign \
                                 used by FlightGear. For local offline flight or in multiplayer mode."));



    layoutLeft->addStretch(10);



    //===========================================================================
    //= Paths
    QVBoxLayout *layoutPaths = new QVBoxLayout();
    layoutMiddle->addLayout(layoutPaths);


    //----------------------------------------------
    //= FlightGear box
    XGroupVBox *grpFgfs = new XGroupVBox("FlightGear Program");
    layoutPaths->addWidget(grpFgfs, 2);

    //----------------------------------------------
    //= FlightGear executable (fgfs)
    QString slbl = QString("Path to the `%1` executable:").arg (  this->strFgFsExeLabel);
    labelFgfsProgram = new QLabel(slbl);
    grpFgfs->addWidget(labelFgfsProgram);

    QHBoxLayout *fgfsPathBox = new QHBoxLayout();
    grpFgfs->addLayout(fgfsPathBox);

    //#lineEditFgFsPath = new QLineEdit("");
    //lineEditFgFsPath->setFixedSize(QSize(240,20));
    comboFgFsPath = new QComboBox();
    //comboFgFsPath->setFixedSize(QSize(240,20));
    comboFgFsPath->setEditable(true);
    comboFgFsPath->setEditText( mainObject->X->fgfs_path() );
    fgfsPathBox->addWidget(comboFgFsPath, 2);

    labelFgfsCheck = new QLabel("");
    fgfsPathBox->addWidget(labelFgfsCheck);

    buttonSetFgfsPath = new QToolButton();
    buttonSetFgfsPath->setFixedSize(20,20);
    buttonSetFgfsPath->setIcon(QIcon(":/icon/path"));
    fgfsPathBox->addWidget(buttonSetFgfsPath);


    connect(comboFgFsPath, SIGNAL(textChanged(QString)), this, SLOT(check_fgfs_path()));
    connect(comboFgFsPath, SIGNAL(textChanged(QString)), this, SLOT(on_fgfs_path_changed(QString)));
    connect(buttonSetFgfsPath, SIGNAL(clicked()),this, SLOT(on_select_fgfs_clicked()) );

    //----------------------------------------------
    //= Base Data Directory (/fgdata)
    //----------------------------------------------
    XGroupVBox *grpFgdata = new XGroupVBox("FlightGear Data");
    layoutPaths->addWidget(grpFgdata, 2);

    labelFgRootData = new QLabel("Path to data directory (fgdata): ");
    grpFgdata->addWidget(labelFgRootData);

    QHBoxLayout *fgfsRootBox = new QHBoxLayout();
    grpFgdata->addLayout(fgfsRootBox);

    lineEditFgRootPath = new QLineEdit("");
    //lineEditFgRootPath->setFixedSize(QSize(240,20));
    lineEditFgRootPath->setText( mainObject->X->fgroot() );
    fgfsRootBox->addWidget(lineEditFgRootPath, 2);

    labelFgRootCheck = new QLabel("");
    fgfsRootBox->addWidget(labelFgRootCheck);

    buttonSetFgRootPath = new QToolButton();
    buttonSetFgRootPath->setFixedSize(20,20);
    buttonSetFgRootPath->setIcon(QIcon(":/icon/path"));
    fgfsRootBox->addWidget(buttonSetFgRootPath);

    connect(buttonSetFgRootPath, SIGNAL(clicked()),this, SLOT(on_select_fgroot_clicked()) );
    connect(lineEditFgRootPath, SIGNAL(textChanged(QString)), this, SLOT(on_fgroot_path_changed(QString)));



    //----------------------------------------------
    //= FGX Workspace
    //----------------------------------------------
    XGroupVBox *grpFgxWs = new XGroupVBox("FGx Workspace");
    layoutPaths->addWidget(grpFgxWs, 2);

    labelFgxWsData = new QLabel("Path to a fgx workspace directory for downloads etc");
    grpFgxWs->addWidget(labelFgxWsData);

    QHBoxLayout *fgxWsBox = new QHBoxLayout();
    grpFgxWs->addLayout(fgxWsBox);

    lineEditFgxWsPath = new QLineEdit("");
    //lineEditFgxWsPath->setFixedSize(QSize(240,20));
    fgxWsBox->addWidget(lineEditFgxWsPath, 2);

    labelFgxWsCheck = new QLabel("");
    fgxWsBox->addWidget(labelFgxWsCheck);

    buttonSetFgxWsPath = new QToolButton();
    buttonSetFgxWsPath->setFixedSize(20,20);
    buttonSetFgxWsPath->setIcon(QIcon(":/icon/path"));
    fgxWsBox->addWidget(buttonSetFgxWsPath);

    //Check if path exists and set pixmap

    connect(lineEditFgxWsPath, SIGNAL(textChanged(QString)), this, SLOT(on_fgx_workspace_path_changed(QString)));
    connect(buttonSetFgxWsPath, SIGNAL(clicked()), this, SLOT(on_select_fgx_workspace_clicked()));


    //----------------------------------------------
    //= Scenery box
    //----------------------------------------------
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

    labelCustomScene = new QLabel("Path(s) to custom scenery directories:");
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
    //connect(lineEditCustomScenePath, SIGNAL(textChanged(QString)), this, SLOT(custom_scenery_check_path()));
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
    //#comboMpMapServer->addItem("mpmap01.flightgear.org", "http://mpmap01.flightgear.org"); // Olivers is down
    comboMpMapServer->addItem("mpmap02.flightgear.org", "http://mpmap02.flightgear.org");
    comboMpMapServer->setCurrentIndex(0);
    grpMapFeatures->addWidget(comboMpMapServer);
    connect(comboMpMapServer, SIGNAL(currentIndexChanged(int)), this, SLOT(on_show_mp_map()));
    layoutRight->addStretch(20);




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

    this->add_fgfs_paths();

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
    // This will do NOTHING if no 'environment' defined
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
    emit( setx("--callsign=", true, txt ) );
    mainObject->launcherWindow->on_upx("--callsign=", true, txt); // change text in headerwidget
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
void CoreSettingsWidget::on_fgfs_path_changed(QString txt)
{
    this->set_fgfs_path();
    this->check_fgfs_path();
}

//=====================================
// Emit fgroot path
void CoreSettingsWidget::on_fgroot_path_changed(QString txt)
{
    emit( setx("fgroot_path", true, txt));
}


//=====================================
// Emit fgroot path
void CoreSettingsWidget::on_fgx_workspace_path_changed(QString txt_ignore)
{
    Q_UNUSED(txt_ignore);
    this->set_fgx_workspace_path();
    this->check_fgx_workspace_path();
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

void CoreSettingsWidget::set_fgfs_path() {
    emit setx("fgfs_path", true, comboFgFsPath->currentText());
}

void CoreSettingsWidget::set_fgroot_path() {
    emit setx("fgroot_path", true, lineEditFgRootPath->text());
}
void CoreSettingsWidget::set_fgx_workspace_path() {
    emit setx("fgx_workspace_path", true, lineEditFgxWsPath->text());
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

    }else if(option == "show_mpmap"){
        checkBoxShowMpMap->setChecked(enabled);
        comboMpMapServer->setCurrentIndex(comboMpMapServer->findData(value));

    }else if(option == "fgfs_path"){
        comboFgFsPath->setEditText(mainObject->X->fgfs_path());
        this->check_fgfs_path();

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

    }else if(option == "fgx_workspace_path"){
        lineEditFgxWsPath->setText(mainObject->X->fgx_workspace_path());

    }else if(option == "custom_scenery_path"){
        lineEditCustomScenePath->setText(enabled ? value : mainObject->X->custom_scenery_path());
    }
}


//======================================================================
// Check paths and give some feedback
//======================================================================
void CoreSettingsWidget::check_fgfs_path()
{
    bool exists = QFile::exists(comboFgFsPath->currentText());
    if (exists) {
        labelFgfsCheck->setPixmap(QPixmap(":/icon/ok"));
    } else {
        labelFgfsCheck->setPixmap(QPixmap(":/icon/not-ok"));
    }

}

void CoreSettingsWidget::check_fgroot_path()
{
    // NEW: check if "version" file exists in folder.
    // Without this file also fgfs won’t start
    // Needs a version check later ...
    bool exists = QFile::exists(lineEditFgRootPath->text() + "/version");
    if (exists) {
        labelFgRootCheck->setPixmap(QPixmap(":/icon/ok"));
        this->fgrootcheck = true;
    } else {
        labelFgRootCheck->setPixmap(QPixmap(":/icon/not-ok"));
        this->fgrootcheck = false;
    }
}
void CoreSettingsWidget::check_fgx_workspace_path()
{
    bool exists = QFile::exists(lineEditFgxWsPath->text());
    if (exists) {
        labelFgxWsCheck->setPixmap(QPixmap(":/icon/ok"));
    } else {
        labelFgxWsCheck->setPixmap(QPixmap(":/icon/not-ok"));
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

void CoreSettingsWidget::on_select_fgfs_clicked()

{
    QString title = QString("Select FlightGear binary (%1)").arg(this->strFgFsExeLabel);
#ifdef USE_ALTERNATE_GETFILE
    QString filePathFgfs = util_getFileName((QWidget *)this,
                                             title ,
                                            comboFgFsPath->currentText());
#else // !#ifdef USE_ALTERNATE_GETFILE
    QString filePathFgfs = QFileDialog::getOpenFileName(this, title, comboFgFsPath->currentText());
#endif // #ifdef USE_ALTERNATE_GETFILE y/n
        if(filePathFgfs.length() > 0){
            comboFgFsPath->setEditText(filePathFgfs);
            check_fgfs_path();
            this->set_fgfs_path();
        }



}

void CoreSettingsWidget::on_select_fgroot_clicked()
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
        check_fgroot_path();
        this->set_fgroot_path();
    }

    if (fgrootcheck == true) {
        reload_lists();
    }
}


void CoreSettingsWidget::on_select_fgx_workspace_clicked(){
    QString dirPathFgxWs = QFileDialog::getExistingDirectory(this, tr("Select FGx Workslpace directory "),
                                                              lineEditFgxWsPath->text(), QFileDialog::ShowDirsOnly);
    if(dirPathFgxWs.length() > 0){
        lineEditFgxWsPath->setText(dirPathFgxWs);
        this->set_fgx_workspace_path();
        this->check_fgx_workspace_path();

    }

}


void CoreSettingsWidget::on_select_terrasyncexebutton()
{
    QString title = tr("Select Terrasync binary (terrasync)");
    QString previous = lineEditTerraSyncExePath->text(); // get current
    if (previous.length() == 0) { // try harder to help user
        previous = comboFgFsPath->currentText(); // if they have already set 'fgfs'
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
        if (lineEditCustomScenePath->text() > 0) {
            QString currentPaths = lineEditCustomScenePath->text().append(":");
            lineEditCustomScenePath->setText(currentPaths.append(dirPathCustomScenery));
        }
        else {
        lineEditCustomScenePath->setText(dirPathCustomScenery);
        }
    }
    custom_scenery_check_path();
}


// Reloading aircraft and airport lists
// needed from setting new fgdata path and from clicking reset button on launcherWindow

void CoreSettingsWidget::reload_lists(){
    mainObject->launcherWindow->aircraftWidget->on_reload_cache();
    mainObject->launcherWindow->airportsWidget->on_reload_cache();
    // and get the airport names, and potentially start apt.dat thread load...
    mainObject->launcherWindow->airportsWidget->on_loadaptdat_done();
}

// RandInt for choosing an image
int CoreSettingsWidget::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

/** Detects possible paths for fgfs
 * @todo Add Windows and OSX paths
*/
void CoreSettingsWidget::add_fgfs_paths(){

    QString which;
    QStringList lst;
    if(this->mainObject->runningOs() == OS_LINUX){
        //= LINUX
        lst.append("/usr/bin/fgfs");
        lst.append("/usr/local/bin/fgfs");

        which = this->which_fgfs();
        if(which != "" && lst.indexOf(which) == -1){
            lst.append(which);
        }
    }
    if(this->mainObject->runningOs() == OS_WINDOWS){
        // @TODO add predictable paths

    }

    lst.sort();


    for(int i = 0; i < lst.size(); i++){

        //= get path and check it exists
        QString path = lst.at(i);
        QFileInfo info(path);
        bool exists = false;
        if(info.exists() && info.isFile()){
            exists = true;
        }

        //= add to combo
        int idx = comboFgFsPath->findText(path, Qt::MatchExactly); // maybe we need case insensitive ??
        if(idx == -1){
            comboFgFsPath->addItem(path);
        }
        idx = comboFgFsPath->findText(path, Qt::MatchExactly); // refind the new the index

        //= set color of item
        QColor color;
        color.setNamedColor(exists ? "green" : "red");
        comboFgFsPath->setItemData(idx, color, Qt::ForegroundRole);

        //= if it exists, then we need to select
        if(which != ""){
             idx = comboFgFsPath->findText(which, Qt::MatchExactly);
             comboFgFsPath->setEditText(comboFgFsPath->itemText(idx));
        }
    }

}

/** detects runs "which fgms" and returns result on nix */
QString CoreSettingsWidget::which_fgfs(){

    QString command = "which fgfs";

    QProcess *process = new QProcess(this);
    process->start(command);

    if(process->waitForStarted()){

        process->waitForFinished();
        QByteArray result =  process->readAllStandardOutput();
        //QByteArray errorResult = process->readAllStandardError();
        QString exe = QString(result).trimmed();

        if(exe.length() == 0){

        }else{
            return exe;
        }
    }
    return QString("");
}
