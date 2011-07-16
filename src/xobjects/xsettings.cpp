

//#include <QtDebug>

#include <QtCore/QDir>
#include <QtCore/QVariant>

#include <QtGui/QDesktopServices>
#include <QtGui/QWidget>

#include "xsettings.h"
#include "utilities/utilities.h"


/*! \class XSettings
 * \brief The XSettings is an extended QSetttings, but also includes most to the FGx configuration.
 *
 * The general idea of the class is to  provide one central place to access settings. It is
 * for this reason that methods are created to hide some of the completities, such as aircraft_path().
 *
 *
 * === Path Settings ===
 *
 * The essential path settings stored in the ini are:
 * - "fgfs_use_default" - whether to use the default installed fgfs binary
 * - "fgfs_custom_path" - the custom path to the fgfs executable
 * - "fgroot_use_default" - whether to use the default FG_ROOT path
 * - "fgroot_custom_path" - the custom path to fgdata/ eg a git checkout
 * - "terrasync_enabled" - whether terrasync is enabled
 * - "terrasync_path" - the path to the terrasync data directory
 *
 * The essential path settings abovar are written out on ConfirmPage of the SetupWizard
 *
 * @author: Peter Morgan
 * @author: Yves Sablonier
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
//** Save/Restore Window
//===========================================================================
/** \brief Saves a window position
 *
 */
void XSettings::saveWindow(QWidget *widget){
    //setValue( _windowName(widget), QVariant(widget->saveGeometry()) );
    QString key = _windowName(widget);
    QByteArray ba = widget->saveGeometry();
    //outLog("saveWindow: Key="+key+", values "+ba.toHex());
    setValue( key, QVariant(ba) );
}
/** \brief Restores a window position
 *
 */
void XSettings::restoreWindow(QWidget *widget){
    // widget->restoreGeometry( value(_windowName(widget)).toByteArray() );
    QString key = _windowName(widget);
    QByteArray ba = value(_windowName(widget)).toByteArray();
    //outLog("restoreWindow: Key="+key+", values "+ba.toHex());
    widget->restoreGeometry(ba);
}

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

QString XSettings::fgx_current_dir(){
	return QDir::current().absolutePath();
}


QString XSettings::cache_dir(){
	return QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
}
