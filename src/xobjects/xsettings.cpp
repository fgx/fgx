

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
    //widget->restoreGeometry( value(_windowName(widget)).toByteArray() );
    QString key = _windowName(widget);
    QByteArray ba = value(_windowName(widget)).toByteArray();
    outLog("restoreWindow: Key="+key+", values "+ba.toHex());
    
	// Set fixed size to 900,700 of all windows for first startup
	if (ba != "") {
		widget->restoreGeometry(ba);
	}else {
		widget->resize(900,700);
	}

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

// this should be used, yves
QString XSettings::fgx_current_dir(){
	return QDir::current().absolutePath();
}

// and this too, hrmbl !
QString XSettings::cache_dir(){
	return QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
}
