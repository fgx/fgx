// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xsettings.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QDir>
#include <QVariant>

#include <QDesktopServices>
#include <QWidget>

#include "xsettings.h"
#include "utilities/utilities.h"


/*! \class XSettings
 * \brief The XSettings is an extended QSetttings, but also includes most to the FGx configuration.
 *
 * The general idea of the class is to  provide one central place to access settings. It is
 * for this reason that methods are created to hide some of the completities, such as aircraft_path().
 *
 */


XSettings::XSettings(QObject *parent) :
    QSettings(parent)
{
}

//===========================================================================
// Style
//===========================================================================
/** \brief return the currently selected style or the default
 */
QString XSettings::style_current(){
    return value("gui_style", style_default()).toString();
}

QString XSettings::style_default(){
    return "cleanlooks";

}

//===========================================================================
//** fgx Install path
//===========================================================================
QString XSettings::fgx_path(){
    return QDir::currentPath().append("/fgx.app/Contents/MacOS");
}

//===========================================================================
//** Save/Restore Splitter
//===========================================================================
/** \brief Saves a splitters dimensions
 *
 * The `settings_namespace` property needs to be set
 *
 * @todo check for settings_namespace
 * @bug no namespace problem
 */
void XSettings::saveSplitter(QSplitter *splitter){
    QString key = splitter->property("settings_namespace").toString();
    QByteArray ba = splitter->saveState();
    outLog("saveSplitter: Key="+key+", values "+ba.toHex());
    setValue( key, QVariant(ba) );
}

/** \brief Restores a splitters dimensions
 * @todo check for settings_namespace
 * @bug no namespace problem
 */
void XSettings::restoreSplitter(QSplitter *splitter){
    QString key = splitter->property("settings_namespace").toString();
    QByteArray ba = value(key).toByteArray();
    outLog("saveSplitter: Key="+key+", values "+ba.toHex());
    splitter->restoreState( ba );
}


//===========================================================================
//** Save/Restore Window
//===========================================================================
/** \brief Saves a window's size and position
 *
 */
void XSettings::saveWindow(QWidget *widget){
    QString key = _windowName(widget);
    QByteArray ba = widget->saveGeometry();
    outLog("saveWindow: Key="+key+", values "+ba.toHex());
    setValue( key, QVariant(ba) );
}
/** \brief Restores a size window position
 *
 * @todo check screensize
 * @bug monitor could be offscreen
 */
void XSettings::restoreWindow(QWidget *widget){
    QString key = _windowName(widget);
    QByteArray ba = value(_windowName(widget)).toByteArray();
    outLog("restoreWindow: Key="+key+", values "+ba.toHex());

    // Set fixed size to 900,700 of all windows for first startup
    if (ba != "") {
        widget->restoreGeometry(ba);
    } else {
        if (widget->accessibleName() == "launcherWindow") {
            widget->resize(900,700);
        }
    }

    // We need to check here window is nto offsreen,
    // eg a dual monitor previous and now no dual so screen is restored off screen
    //QDesktopSccreen dimensions vs postion
}

/** \brief Extracts the `settings_namespace` property */
QString XSettings::_windowName(QWidget *widget){
    QString key_name = "window/";
    key_name.append(widget->property("settings_namespace").toString());
    key_name.append("/geometry");
    return key_name;
}


/** \brief The "Special Developer Flag"
 *
 * This is one of those magic workaround functions to detect if in development mode.
 * The functions detects if the file "DEV_MODE.txt" exists in the current directory.
 * eg in fgx/src/DEV_MODE.txt
 * The precense of this file make a few things happen.. such as redirecting login to shell
 * Reading stuff from files in stead of resources etc..
 * \return  developer mode enabled
 */
bool XSettings::dev_mode(){
    QString curr = XSettings::fgx_current_dir().append("/DEV_MODE.txt");
    return QFile::exists(curr);
}

/** \brief this should be used, yves ?
 */
QString XSettings::fgx_current_dir(){
    return QDir::current().absolutePath();
}

/** \brief shortcut to cache directory */
QString XSettings::cache_dir(){
    return QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
}

QString XSettings::aircraft_downloads_url(){
    return QString("http://localhost/~fg/flightgear-aircraft/");
    //return QString("http://downloads.freeflightsim.org/flightgear-aircraft/");
}
QString XSettings::aircraft_downloads_url(QString file){
    return XSettings::aircraft_downloads_url().append(file);
}
QString XSettings::aircraft_downloads_url(QString dir, QString file){
    return XSettings::aircraft_downloads_url().append(dir).append("/").append(file);
}

