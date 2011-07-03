

//#include <QtDebug>

#include <QtCore/QDir>
#include <QtCore/QVariant>

#include <QtGui/QDesktopServices>
#include <QtGui/QWidget>

#include "xsettings.h"
#include "utilities/utilities.h"

#define UNZIP_READ_BUFFER (256*1024)


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

/** \brief The default style for platform
*/
QString XSettings::style_default(){
	if(runningOs() == MAC){
		return QString("Macintosh (aqua)");
	}
	if(runningOs() == WINDOWS){
		return QString("cleanlooks");
	}
	if(runningOs() == LINUX){
		return QString("cleanlooks");
	}
	return QString("cleanlooks");
}

//===========================================================================
//** fgx Install path
//===========================================================================
QString XSettings::fgx_path(){
	return QDir::currentPath().append("/fgx.app/Contents/MacOS");
}


//===========================================================================
//== fgfs Executable
//===========================================================================
/** \brief Path to fgfs executable
 */
QString XSettings::fgfs_path(){
	if(fgfs_use_default()){
		return fgfs_default_path();
	}
	return value("fgfs_custom_path", "").toString();
}

/** \brief true is the default path is being used
 */

bool XSettings::fgfs_use_default(){
	return value("fgfs_use_default", '1').toBool();
}

/** \brief absolute default path for the fgfs executable
 */
QString XSettings::fgfs_default_path(){

	if(runningOs() == MAC){
		return QDir::currentPath().append("/fgx.app/Contents/MacOS/fgfs");

	}else if(runningOs() == LINUX){
		return QString("/usr/local/bin/fgfs");


	}else if(runningOs() == WINDOWS){
		return QString("C:/Program Files/FlightGear/bin/win32/fgfs.exe");
	}

	return QString("UNKNOW OS in default_fgfs_path()");
}

//===========================================================================
//== fgroot  path
//===========================================================================
/** \return The absolute path to FG_ROOT ie /fgdata directory */
QString XSettings::fgroot(){
	if(fgroot_use_default()){
		return fgroot_default_path();
	}
	return value("fgroot_custom_path", "").toString();
}
bool XSettings::fgroot_use_default(){
	return value("fgroot_use_default", '1').toBool();
}

/** \brief Path to FG_ROOT with appended path
 *
 * Returns the path to the FG_ROOT. If the default install
 * is selected, then that is returned, otherwise the custom selected fg_data path.
 * The file is appended with the append_path
 * \return The absolute path.
 */

QString XSettings::fgroot(QString append_path){
	return this->fgroot().append(append_path);
}

/** \brief Platform specific default path for the FG_ROOT dir
 *
  * \return The absolute path to FG_ROOT
 */
QString XSettings::fgroot_default_path(){

	if(runningOs() == MAC){
		return QDir::currentPath().append("/fgx.app/Contents/Resources/fgx-fgdata");

	}else if(runningOs() == LINUX){
		return QString("/usr/share/games/FlightGear");

	}else if(runningOs() == WINDOWS){
		return QString("C:/Program Files/FlightGear/data");
	}

	return QString("Your system is not handled for default fgdata path");
}

//===========================================================================
//** Paths Sane
//===========================================================================
/** \brief Checks whether the executablem FG_ROOT paths are sane.
 *
 * \return true if sane
 */
bool XSettings::paths_sane(){
	if(!QFile::exists(fgfs_path())){
		return false;
	}
	if(!QFile::exists(fgroot())){
		return false;
	}
	return true;
}


//===========================================================================
//** Aircraft Path
//===========================================================================
/** \brief The path to the /Aircraft directory
 *
  * \return absolute path.
 */
QString XSettings::aircraft_path(){
	return fgroot().append("/Aircraft");
}

/** \brief Path to the /Aircraft directory with a dir appended.
 *
 * \return absolute path.
 */
QString XSettings::aircraft_path(QString dir){
	return fgroot().append("/Aircraft/").append(dir);
}


//===========================================================================
//** Airports path
//===========================================================================
/** \brief Return the absolute path to the /Airports directory
 *
 * \return If TerraSync is enabled, then returns a terrasync folder, otherwise the default.
 */
QString XSettings::airports_path(){
    QString rpath;
	//= Using terrasync
	if(terrasync_enabled()){
		if(runningOs() == MAC){
			rpath = QDir::homePath();
			rpath.append("/Documents/TerrasyncScenery");
		}else{
			// Use the terra sync path
			rpath = terrasync_sync_data_path().append("/Airports");
		}
	} else{ // Otherwise return the FG_ROOT airports/
		rpath = fgroot().append("/Scenery/Airports");
	}
	//outLog("*** FGx settings: Airports path: " + rpath + " ***");
	return rpath;
}

//===========================================================================
//** Scenery Path - TODO x/win
//===========================================================================
/** \brief Returns the absolute path to the /Scenery file
 *
 * \return The absolute path.
 */
QString XSettings::scenery_path(){

	return fgroot("/Scenery");
}

//===========================================================================
//** TerraSync
//===========================================================================
/** \brief  Using terrasync for scenery
 *
 * \return true if using terrasync
 */
bool XSettings::terrasync_enabled(){
	return value("terrasync_enabled").toBool();
}
/** \brief terrasync executable
 *
  * \return path to terrasync executable
  * \todo Windows path
 */
QString XSettings::terrasync_exe_path(){
	if (runningOs() == MAC) {
		//* points to terrasync binary in app bundle
		return QDir::currentPath().append("/fgx.app/Contents/MacOS/terrasync");

	}else if(runningOs() == LINUX){
		return QString("terrasync");

	}else if(runningOs() == WINDOWS){
		return QString("TODO/path to terrasync.exe");
	}
	return QString("TODO - terrasync");
}
/** \brief terrasync data path
 *
  * \return path to where terrasync will store files downloaded.
 */
QString XSettings::terrasync_sync_data_path(){
	return value("terrasync_path").toString();
}	
	


//===========================================================================
//** temp
//===========================================================================
/** \brief location if temp directoty , os specific
 *
 * Shortcut method for Qt's storageLocation()
 * \return Absolute path
 */
QString XSettings::temp_dir(){
	return QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).absolutePath();
}
/** \brief location if temp directoty , os specific with appended file/path
 *
 * Shortcut method for Qt's storageLocation()
 * \return Absolute path with appended paths.
 */
QString XSettings::temp_dir(QString append_path){
	return temp_dir().append(append_path);
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
    outLog("saveWindow: Key="+key+", values "+ba.toHex());
    setValue( key, QVariant(ba) );
}
/** \brief Restores a window position
 *
 */
void XSettings::restoreWindow(QWidget *widget){
    // widget->restoreGeometry( value(_windowName(widget)).toByteArray() );
    QString key = _windowName(widget);
    QByteArray ba = value(_windowName(widget)).toByteArray();
    outLog("restoreWindow: Key="+key+", values "+ba.toHex());
    widget->restoreGeometry(ba);
}

QString XSettings::_windowName(QWidget *widget){
	QString key_name = "window/";
	key_name.append(widget->property("settings_namespace").toString());
	key_name.append("/geometry");
	return key_name;
}

//===========================================================================
//** OS detection
//===========================================================================
/** \brief What OS is running
 *
 * \return a OS enum value
 */
int XSettings::runningOs() {
    #ifdef Q_WS_X11
		return XSettings::LINUX;
    #endif

    #ifdef Q_WS_MAC
		return XSettings::MAC;
    #endif

    #ifdef Q_WS_WIN
		return XSettings::WINDOWS;
    #endif

	return XSettings::UNKNOWN;
}

//===========================================================================
//** FGCom
//===========================================================================
QString XSettings::default_fgcom_no(){
	return QString("fgcom.flightgear.org.uk");
}
QString XSettings::default_fgcom_port(){
	return QString("16661");
}


QString XSettings::fgcom_exe_path(){
		if (runningOs() == MAC) {
			// Default fgcomx installation with fgcom installer by gral
			// Will change with fgcom coming in app bundle
			return QString("/usr/local/bin/fgcom");
		}
		else if(runningOs() == LINUX){
			return QString("fgcom");
		}
	
		else if(runningOs() == WINDOWS){
			return QString("fgcom"); // TODO!
		}
	
	return "fgcom";
}


//===========================================================================
//** Data File eg airports.txt
//===========================================================================
/** \brief Path to a data file eg data_file("airports.txt")
 *
 * \return Absolute path to the file
 */
QString XSettings::data_file(QString file_name){
	QString storedir = QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).absolutePath();

	// create path is not exist
	if(!QFile::exists(storedir)){
		QDir *dir = new QDir("");
		dir->mkpath(storedir);
	}
	return storedir.append("/").append(file_name);
}




/** \brief Log File
 *
 * \return Absolute path to log file
 */
QString XSettings::log_file_path(){
	if(runningOs() == WINDOWS){
		return temp_dir("/fgx-log.txt");

	}else if(runningOs() == MAC){
		return QDir::homePath().append("/Library/Logs/fgx.log");

	}else if(runningOs() == LINUX){
		return temp_dir("/fgx.log");

	}else{
		return "UNKNOWN log_file_path()";
	}
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
