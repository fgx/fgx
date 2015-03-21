// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // menuwidget.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#include "app_config.h"
#include <QtDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>

#include "utilities/utilities.h"
#include "launcher/menuwidget.h"
#include "launcher/launcherwindow.h"
#include "xobjects/mainobject.h"

/*! \class MenuWidget
 *  \brief The MenuWidget is the top part of the LauncherWindow. Its contains the toplevel menu
 *         and has ended up separate to make tweaking for OSX easier.
 *
*/
MenuWidget::MenuWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

    mainObject = mob;

    settings = new XSettings();
    debug_mode = settings->value("DEBUG_MODE","0").toBool();

    menuLayout = new QHBoxLayout();
    menuLayout->setContentsMargins(0,0,0,0);

    // Macro needed to make the widget "placeholder" disappearing on OSX.
    // Not nice at all, but working.
    if(MainObject::runningOs() != OS_MAC){
        setLayout(menuLayout);
    }

    // Menu "FGx" (OSX = "Windows") actions
    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcut(QString("Ctrl+q"));
    quitAction->setStatusTip(tr("Exit the application"));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(on_menu_quit()));

    logWindowAction = new QAction(tr("Show &logs"), this);
    logWindowAction->setShortcut(QString("Ctrl+Shift+l"));
    logWindowAction->setStatusTip(tr("Shows all logs in a separate window"));
    connect(logWindowAction, SIGNAL(triggered()), this, SLOT(on_show_log_window()));

    debugWindowAction = new QAction(tr("&Debug settings"), this);
    debugWindowAction->setShortcut(QString("Ctrl+d"));
    debugWindowAction->setStatusTip(tr("Shows the settings debug window"));
    connect(debugWindowAction, SIGNAL(triggered()), this, SLOT(on_show_debug_window()));

    propsWindowAction = new QAction(tr("&Properties browser"), this);
    propsWindowAction->setShortcut(QString("Ctrl+Shift+p"));
    propsWindowAction->setStatusTip(tr("Shows properties browser"));
    connect(propsWindowAction, SIGNAL(triggered()), this, SLOT(on_show_props_window()));

    // Menu "Mode" actions
    debugmodeAction = new QAction(tr("&Debug mode"), this);
    debugmodeAction->setShortcut(QString("Ctrl+Shift+d"));
    debugmodeAction->setStatusTip(tr("Switching debug mode"));
    debugmodeAction->setCheckable(true);
    debugmodeAction->setChecked(debug_mode);
    connect(debugmodeAction, SIGNAL(triggered()), this, SLOT(on_menu_debug_mode()));

    // Switching menu name, maybe should better go to two menus later
    if(MainObject::runningOs() == OS_MAC){
        applicationMenu = new QMenu(tr("&Windows"));
    }else {
        applicationMenu = new QMenu(tr("&FGx"));
    }

    modeMenu = new QMenu(tr("&Mode"));

    profileMenu = new QMenu(tr("&Profile"));

    //== Help Menu
    helpMenu = new QMenu(tr("&Help"));
    helpFGxGroup = new QMenu(tr("&FGx Online Help"));
    helpFlightGearGroup = new QMenu(tr("&FlightGear Online Help"));



    // Menu Profile actions
    loadProfileAction = new QAction(tr("&Load ..."), this);
    loadProfileAction->setStatusTip(tr("Loads a profile"));
    connect(loadProfileAction, SIGNAL(triggered()), this, SLOT(on_menu_load_profile()));

    saveProfileAction = new QAction(tr("&Save as ..."), this);
    saveProfileAction->setStatusTip(tr("Saves current settings in a profile"));
    connect(saveProfileAction, SIGNAL(triggered()), this, SLOT(on_menu_save_profile()));

    resetProfileAction = new QAction(tr("&Reset"), this);
    resetProfileAction->setStatusTip(tr("Reset all settings to current loaded profile"));
    connect(resetProfileAction, SIGNAL(triggered()), this, SLOT(on_menu_reset_profile()));

    defaultProfileAction = new QAction(tr("&Load default profile"), this);
    defaultProfileAction->setStatusTip(tr("Reload FGx default profile (built-in)"));
    connect(defaultProfileAction, SIGNAL(triggered()), this, SLOT(on_menu_default_profile()));

    //=======================================
    // Menu FlightGear Help actions
    urlActionWiki = new QAction(tr("Wiki - wiki.flightgear.org"), this);
    connect(urlActionWiki, SIGNAL(triggered()), this, SLOT(on_menu_url_wiki()));

    urlActionForums = new QAction(tr("Forums - forums.flightgear.org"), this);
    connect(urlActionForums, SIGNAL(triggered()), this, SLOT(on_menu_url_forums()));

    //=======================================
    // Menu FGx Help actions

    urlActionFGxUserHelp = new QAction(tr("FGx - User Forum"), this);
    connect(urlActionFGxUserHelp, SIGNAL(triggered()), this, SLOT(on_menu_fgx_user_forums()));

    // Add actions to menu "FGx"/"Windows" (OSX = "Windows")
    applicationMenu->addAction(quitAction);
    applicationMenu->addAction(logWindowAction);
    applicationMenu->addAction(debugWindowAction);
    applicationMenu->addAction(propsWindowAction);

    // Add actions to menu "Mode"
    modeMenu->addAction(debugmodeAction);

    // Add actions to menu "Profile"
    profileMenu->addAction(loadProfileAction);
    profileMenu->addAction(saveProfileAction);
    profileMenu->addAction(resetProfileAction);
    profileMenu->addAction(defaultProfileAction);

    // Add actions to menu "Help"
    helpFlightGearGroup->addAction(urlActionWiki);
    helpFlightGearGroup->addAction(urlActionForums);
    helpFGxGroup->addAction("Website", this, SLOT(on_menu_fgx_www()) );
    helpFGxGroup->addAction("Bugs & Requests", this, SLOT(on_menu_fgx_issues()) );
    helpFGxGroup->addAction("Source Code", this, SLOT(on_menu_fgx_github()) );
    helpFGxGroup->addAction(urlActionFGxUserHelp);

    helpMenu->addMenu(helpFGxGroup);
    helpMenu->addSeparator();
    helpMenu->addMenu(helpFlightGearGroup);
    helpMenu->addSeparator();
    helpMenu->addAction("About FGx", this, SLOT(on_menu_fgx_version_info()));
    helpMenu->addAction("About Qt", this, SLOT(on_menu_qt_version_info()));

    // Create menubar, parentless 0 is needed for OSX using the wrapper for
    // getting native OSX menus, see qt4 MenuBar doc
    mainMenu = new QMenuBar(0);

    // Adding the menus to the MenuBar
    mainMenu->addMenu(applicationMenu);
    mainMenu->addMenu(modeMenu);
    mainMenu->addMenu(profileMenu);
    mainMenu->addMenu(helpMenu);

    // No action for OSX, but gives the menu for x/win
    menuLayout->addWidget(mainMenu, 0);

}

//================================================================================
// Quit application
//================================================================================
void MenuWidget::on_menu_quit(){
    mainObject->on_quit();
}

//================================================================================
// Show log window, focus
//================================================================================
void MenuWidget::on_show_log_window(){
    mainObject->viewLogsWidget->show();
    mainObject->viewLogsWidget->raise();
    mainObject->viewLogsWidget->activateWindow();

}

//================================================================================
// Show debug window, focus
//================================================================================
void MenuWidget::on_show_debug_window(){
    mainObject->fgxDebugWidget->show();
    mainObject->fgxDebugWidget->raise();
    mainObject->fgxDebugWidget->activateWindow();

}

//================================================================================
// Show properties window, focus
//================================================================================
void MenuWidget::on_show_props_window(){
    mainObject->propertiesBrowser->show();
    mainObject->propertiesBrowser->raise();
    mainObject->propertiesBrowser->activateWindow();

}

//================================================================================
// Set application to debug mode (show all exe buttons)
//================================================================================
void MenuWidget::on_menu_debug_mode(){
    debug_mode = debugmodeAction->isChecked();
    mainObject->set_debug_mode(debug_mode);

}

//================================================================================
// Load Profile
//================================================================================
void MenuWidget::on_menu_load_profile()
{
    QString message("Profile load abandoned");
    if (mainObject->X->load_profile()) {
        // get lastused profile name
        QString previous = mainObject->X->getLastUsed();
        previous = util_getBaseName(previous);
        message = "Profile loaded from "+previous;
    }
    mainObject->launcherWindow->headerWidget->showMessage(message);
}

//================================================================================
// Save Profile
//================================================================================
void MenuWidget::on_menu_save_profile()
{
    QString message("Profile save abandoned");
    if (mainObject->X->save_profile()) {
        // get lastused profile name
        QString previous = mainObject->X->getLastUsed();
        previous = util_getBaseName(previous);
        message = "Profile saved to "+previous;
    }
    mainObject->launcherWindow->headerWidget->showMessage(message);
}


//================================================================================
// Reset Profile
//================================================================================
void MenuWidget::on_menu_reset_profile()
{
    QString message("Reset to loaded profile.");
    QString previous = mainObject->X->getLastUsed();
    mainObject->X->load_last_profile(previous);
    mainObject->launcherWindow->headerWidget->showMessage(message);
}

//================================================================================
// Default Profile
//================================================================================
void MenuWidget::on_menu_default_profile()
{
    QString message("Reset to FGx default profile.");
    //QString previous = mainObject->X->getLastUsed();
    mainObject->X->read_default_ini();
    mainObject->launcherWindow->headerWidget->showMessage(message);
}

//================================================================================
// Help
//================================================================================
void MenuWidget::on_menu_url_wiki()
{
    QUrl url("http://wiki.flightgear.org");
    QDesktopServices::openUrl( url );
}

void MenuWidget::on_menu_url_forums()
{
    QUrl url("http://forum.flightgear.org/");
    QDesktopServices::openUrl( url );
}

void MenuWidget::on_menu_fgx_user_forums()
{
    QUrl url("http://forum.flightgear.org/viewforum.php?f=65");
    QDesktopServices::openUrl( url );
}
void MenuWidget::on_menu_fgx_www(){
    QDesktopServices::openUrl( QUrl(APP_WWW) );
}
void MenuWidget::on_menu_fgx_issues(){
    QDesktopServices::openUrl( QUrl(APP_ISSUES) );
}
void MenuWidget::on_menu_fgx_github(){
    QDesktopServices::openUrl( QUrl(APP_PROJECT) );
}

void MenuWidget::on_menu_fgx_version_info(){
    this->mainObject->on_about_fgx();
}

void MenuWidget::on_menu_qt_version_info(){
   this->mainObject->on_about_qt();
}
