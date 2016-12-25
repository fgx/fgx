// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // launcherwindow.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#include <QUrl>
#include <QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QWhatsThis>


#include <QApplication>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDesktopServices>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStyleFactory>
#include <QMessageBox>
#include <QToolBar>
#include <QFileDialog>

#include "xwidgets/xgroupboxes.h"

#include "launcher/launcherwindow.h"
#include "utilities/utilities.h"
#include "utilities/messagebox.h"

#include "xobjects/xsettingsmodel.h"

/*! \class LauncherWindow
 *  \brief The LauncherWindow is raised and controlled by MainObject
 *
 * At Present its created upon application launch as a hidden/show window
*/
LauncherWindow::LauncherWindow(MainObject *mob, QWidget *parent)
    : QWidget(parent)
{

    initializing = true;
    mainObject = mob;
    setAccessibleName("launcherWindow");

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


    //== Application Menu

    menuWidget = new MenuWidget(mainObject);
    outerContainer->addWidget(menuWidget, 0);


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

    //* Aircraft Widget
    aircraftWidget = new AircraftWidget(mainObject);
    tabWidget->addTab(aircraftWidget, tr("Aircraft"));

    //** Position Tab
    airportsWidget = new AirportsWidget(mainObject);
    tabWidget->addTab(airportsWidget, tr("Position"));

    //* Time / Weather Widget
    timeWeatherWidget = new TimeWeatherWidget(mainObject);
    tabWidget->addTab(timeWeatherWidget, tr("Time and Weather"));

    //* Rendering Widget
    renderingWidget = new RenderingWidget(mainObject);
    tabWidget->addTab(renderingWidget, tr("Rendering"));


    //* Network Tab
    networkWidget = new NetworkWidget(mainObject);
    tabWidget->addTab( networkWidget, tr("Network"));


    //* Expert Options
    expertOptionsWidget = new ExpertOptionsWidget(mainObject);
    tabWidget->addTab( expertOptionsWidget, tr("Expert Options"));


    mainLayout->addSpacing(10);
    mainLayout->setAlignment( Qt::AlignTop );



    //========================================================================================
    //**Bottom Bar
    //========================================================================================

    //* Show bottom bar
    QHBoxLayout *bottomActionLayout = new QHBoxLayout();
    bottomActionLayout->setAlignment( Qt::AlignTop );
    mainLayout->addLayout(bottomActionLayout);

    XGroupHBox *toolBox = new XGroupHBox(tr("Profiles"));
    bottomActionLayout->addWidget(toolBox);

    //toolBox->setStyleSheet("XGroupHBox::title { color: #000000; background-color: yellow }");

    //= Load Settings
    QToolButton *buttonLoadSettings = new QToolButton(this);
    buttonLoadSettings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    buttonLoadSettings->setText("Load");
    buttonLoadSettings->setAutoRaise(true);
    buttonLoadSettings->setIcon(QIcon(":/icon/load"));
    buttonLoadSettings->setStyleSheet("padding: 0px;");
    toolBox->addWidget(buttonLoadSettings);
    connect(buttonLoadSettings, SIGNAL(clicked()),
            this, SLOT(load_profile())
            );

    //= Save Settings
    QToolButton *buttonSaveSettings = new QToolButton(this);
    buttonSaveSettings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    buttonSaveSettings->setText("Save");
    buttonSaveSettings->setAutoRaise(true);
    buttonSaveSettings->setIcon(QIcon(":/icon/save"));
    buttonSaveSettings->setStyleSheet("padding: 0px;");
    toolBox->addWidget(buttonSaveSettings);
    connect(buttonSaveSettings, SIGNAL(clicked()),
            this, SLOT(save_profile())
            );

    //= Reset Settings
    QToolButton *buttonResetSettings = new QToolButton(this);
    buttonResetSettings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    buttonResetSettings->setText("Reset");
    buttonResetSettings->setAutoRaise(true);
    buttonResetSettings->setIcon(QIcon(":/icon/reset"));
    buttonResetSettings->setStyleSheet("padding: 0px;");
    toolBox->addWidget(buttonResetSettings);
    connect(buttonResetSettings, SIGNAL(clicked()),
            this, SLOT(reset_profile())
            );
    bottomActionLayout->addStretch(20);



    bottomActionLayout->addStretch(100);

    //=============================================================
    //== Start Stop ==
    //=============================================================


    //= TerraSync
    exeTerraSync = new ExeControls("TerraSync");
    bottomActionLayout->addWidget(exeTerraSync);
    connect(exeTerraSync->buttonStart, SIGNAL(clicked()),
            this, SLOT(on_start_terrasync_clicked())
            );

    connect(mainObject->processTerraSync, SIGNAL(running(bool)), exeTerraSync, SLOT(set_running(bool)));
    connect(exeTerraSync, SIGNAL(stop()), mainObject->processTerraSync, SLOT(stop()));

    //= FlightGear
    exeFgfs = new ExeControls("FlightGear (fgfs)");
    bottomActionLayout->addWidget(exeFgfs);
    connect(	exeFgfs->buttonStart, SIGNAL(clicked()),
            this, SLOT(on_start_fgfs_clicked())
            );
    connect(mainObject->processFgFs, SIGNAL(running(bool)), exeFgfs, SLOT(set_running(bool)));
    connect(exeFgfs, SIGNAL(stop()), mainObject->processFgFs, SLOT(stop()));

    //= All
    exeAll = new ExeControls("FlightGear");
    //exeAll->setStyleSheet("QGroupBox::title { color: #ffffff; background-color: #007eff }");
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


    connect(this, SIGNAL(setx(QString,bool,QString)), mainObject->X, SLOT(set_option(QString,bool,QString)) );
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

    //= First load the profiles

    QSettings firstsettings;
    QString previous = mainObject->X->getLastUsed();
    int done = 0;
    while (!firstsettings.value("firststartup").toBool()) {
        // stay here UNTIL the FIRST SAVE has been done
        if (done == 0) {    // first time - set a kind, gentle message
            previous = "Welcome to FGx. You need to save this Settings Profile first. Choose a writable location in next dialog and click \"Save\".";
            outLog("*** Looks like first startup without profile, reading default values. ***");
            mainObject->X->read_default_ini();
        }
        QMessageBox::warning(this, tr("FGx First Startup - Settings Reset"),previous,QMessageBox::Ok);
        done++;
        if ( mainObject->X->save_profile() ) {
            firstsettings.setValue("firststartup", "true");
            firstsettings.sync();

        } else {    // did NOT write a profile - get quite INSISTANT ;=()
            previous.sprintf("Try %d! You MUST save this Settings Profile first. Choose a writable location in next dialog and click \"Ok\" ONLY.",
                         (done + 1));
        }
        // reload lists after setting default init values
        mainObject->launcherWindow->aircraftWidget->on_reload_cache();
        mainObject->launcherWindow->airportsWidget->on_reload_cache();
    }

    // This we do everytime, solves a lot of problems loading the first saved profile
    previous = mainObject->X->getLastUsed();
    mainObject->X->load_last_profile(previous);
    QFileInfo fi(previous);
    QString name = fi.fileName();
    header_show_message("Last used profile loaded: "+name);

    //= check paths are sane
    if(!mainObject->X->paths_sane()){
        //mainObject->show_setup_wizard();
    }

    //* Paths are sane so we can initialize;
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
    //save_settings();
    mainObject->start_all();
}

//= Start FgFs
void LauncherWindow::on_start_fgfs_clicked() {
    qDebug() << "pre validate";
    if(!validate()){
        return;
    }
    qDebug() << "save";
    //save_settings();
    mainObject->start_fgfs();
}

//= Start Terrasync
void LauncherWindow::on_start_terrasync_clicked() {
    if(!validate()){
        return;
    }
    //save_settings();
    mainObject->start_terrasync();
}



//================================================================================
// Save Settings
//================================================================================
void LauncherWindow::save_settings()
{
    mainObject->settings->sync();
    // get lastused profile name
    QString previous = mainObject->X->getLastUsed();
    previous = util_getBaseName(previous);
    headerWidget->showMessage("Settings saved to "+previous);
    outLog("FGx: LauncherWindow widget says: settings saved to "+previous);

}

//================================================================================
// Load Profile
//================================================================================
void LauncherWindow::load_profile()
{
    QString message("Profile load abandoned");
    if (mainObject->X->load_profile()) {
        // get lastused profile name
        QString previous = mainObject->X->getLastUsed();
        previous = util_getBaseName(previous);
        message = "Profile loaded from "+previous;
    }
    headerWidget->showMessage(message);
    reset_profile(); // resets the profile, gives emitting back, reload cache

}

//================================================================================
// Save Profile
//================================================================================
void LauncherWindow::save_profile()
{
    QString message("Profile save abandoned");
    if (mainObject->X->save_profile()) {
        // get lastused profile name
        QString previous = mainObject->X->getLastUsed();
        previous = util_getBaseName(previous);
        message = "Profile saved to "+previous;
    }
    headerWidget->showMessage(message);
}


//================================================================================
// Reset Profile
//================================================================================
void LauncherWindow::reset_profile()
{
    QString message("Reset to loaded profile.");
    QString previous = mainObject->X->getLastUsed();
    mainObject->X->load_last_profile(previous);
    headerWidget->showMessage(message);
    airportsWidget->on_reload_cache(); // Reload the aircraft and airport lists
}


//==============================================
//== View Buttons
void LauncherWindow::on_command_preview(){
    if(!validate()){
        return;
    }
    save_settings();
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
//* Message Event
//=======================================================================================================================

void LauncherWindow::header_show_message(QString message)

{
    headerWidget->showMessage(message);
}


//=======================================================================================================================
//* Misc Events
//=======================================================================================================================
void LauncherWindow::moveEvent(QMoveEvent *ev){
    Q_UNUSED(ev);
    this->mainObject->settings->saveWindow(this);
}

//= window close
void LauncherWindow::closeEvent(QCloseEvent *event){

    QMessageBox msgBox;
    msgBox.setText("Profile and Settings:");
    msgBox.setInformativeText("Closing application: Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Save:
            mainObject->X->save_profile();
            save_settings();
            mainObject->settings->sync();
            event->accept();
            break;
        case QMessageBox::Discard:
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        default:
            mainObject->X->save_profile();
            save_settings();
            mainObject->settings->sync();
            event->accept();
            break;
    }

    mainObject->on_quit();


}


//=== Tab Changes
void LauncherWindow::on_tab_changed(int tab_idx){
    if(initializing){
        return;
    }

    /* if(tab_idx == tabWidget->indexOf(airportsWidget)){
        airportsWidget->treeWidgetAirportInfo->update();
        airportsWidget->mapWidget->update();
    } */

    if(tab_idx == tabWidget->indexOf(this->expertOptionsWidget)){
        //on_command_preview();
    }else{
        //= we dont want to restore to output preview cos it validates and will throw popup
        mainObject->settings->setValue("launcher_last_tab", tabWidget->currentIndex());
    }

    // deal with focus in events
    if(tab_idx == tabWidget->indexOf(this->aircraftWidget)) {
        this->aircraftWidget->txtFilter->setFocus();
    }
}





//============================================
//= Set Widgets on  Debug Mode
void LauncherWindow::on_debug_mode()
{
    exeFgfs->setVisible(mainObject->debug_mode == true);
    exeTerraSync->setVisible(mainObject->debug_mode == true);
    exeAll->setVisible(mainObject->debug_mode == false);
}



//============================================
//= Update Settings
void LauncherWindow::on_upx(QString option, bool enabled, QString value)
{
    Q_UNUSED(enabled);
    Q_UNUSED(value);

    if(option == "--callsign=" ){
        headerWidget->setCallsign( value );
    }

    QStringList keys;
    keys << "--callsign="  <<  "--aircraft=" << "--airport="  << "--runway=" << "--parking-id=";
    if(keys.contains(option)){
        QString header = QString("<font color=#ff0000>%1</font> : <b>%2</b>"
                                    ).arg( mainObject->X->getx("--callsign=")
                                    ).arg( mainObject->X->getx("--aircraft=")
                                );
        XOpt apt = mainObject->X->get_opt("--airport=");
        if(apt.enabled){
            header.append(" @ ").append(apt.value);

            XOpt rwy = mainObject->X->get_opt("--runway=");
            if(rwy.enabled){
                header.append(" RWY ").append(rwy.value);
            }else{
                XOpt stand = mainObject->X->get_opt("--parking-id=");
                if(stand.enabled){
                    header.append(" Stand: ").append(stand.value);
                }
            }
        }

        headerWidget->setHeader( header );


    }


}


