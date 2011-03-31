

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QVariant>

#include <QtGui/QDesktopServices>
#include <QtGui/QWidget>

#include "xsettings.h"

/* Extends the QSettings with fg_root(), savewindow and other util functions
   callable as mainObject->settings->foo()
*/

XSettings::XSettings(QObject *parent) :
    QSettings(parent)
{
}

//===========================================================================
//** fgfs Executable Full Path
//===========================================================================
QString XSettings::fgfs_path(){
	// TODO = return the default windows path
	return this->value("FGFS", "fgfs").toString();
}



//===========================================================================
//** fg_root()
//===========================================================================
QString XSettings::fg_root(){
    return this->value("FG_ROOT", "/usr/share/games/FlightGear/").toString();
}

QString XSettings::fg_root(QString append_path){
    return this->fg_root().append(append_path);
}


//===========================================================================
//** Paths Sane
//===========================================================================
bool XSettings::paths_sane(){
	if(!QFile::exists(fgfs_path())){
		return false;
	}
	if(!QFile::exists(fg_root())){
		return false;
	}
	return true;
}


//===========================================================================
//** Aircraft Path
//===========================================================================
QString XSettings::aircraft_path(){
	return fg_root().append("/Aircraft");
}

QString XSettings::aircraft_path(QString dir){
	return fg_root().append("/Aircraft/").append(dir);
}


//===========================================================================
//** Aiprots path
//===========================================================================
QString XSettings::airports_path(){
	if(use_terrasync()){
		return scenery_path().append("/Airports");
	}
	return fg_root().append("/Scenery/Airports");
}


//===========================================================================
//** Scenery Path
//===========================================================================
QString XSettings::scenery_path(){
	if(use_terrasync()){
		return terrasync_path();
	}
	return fg_root("/Scenery");
}

//===========================================================================
//** TerraSync
//===========================================================================
bool XSettings::use_terrasync(){
	return value("use_terrasync").toBool();
}
QString XSettings::terrasync_path(){
	return value("terrasync_path").toString();
}


//===========================================================================
//** temp
//===========================================================================
QString XSettings::temp(){
    //** Check the temp dubdirctory exists or create it
    if( _temp.length() == 0 ){
        QDir tmpDir = QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation));
        if(!tmpDir.exists("ffs-desktop")){
            tmpDir.mkdir("ffs-desktop");
        }
        _temp = QDesktopServices::storageLocation(QDesktopServices::TempLocation).append("/ffs-desktop/");
    }
    return _temp;
}
QString XSettings::temp(QString append_path){
    return this->temp().append(append_path);

}


//===========================================================================
//** Save/Restore Window
//===========================================================================
void XSettings::saveWindow(QWidget *widget){
	qDebug() << "saveWindow" << widget->saveGeometry();
	setValue( _windowName(widget), QVariant(widget->saveGeometry()) );
}
void XSettings::restoreWindow(QWidget *widget){
	qDebug() << "Restore";
	widget->restoreGeometry( value(_windowName(widget)).toByteArray() );
}
QString XSettings::_windowName(QWidget *widget){
	QString key_name = "window/";
	key_name.append(widget->property("settings_namespace").toString());
	key_name.append("/geometry");
	return key_name;
}


int XSettings::runningOS() {
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


QString XSettings::default_fgcom_no(){
	return QString("fgcom -Sfgcom.flightgear.org.uk");
}
QString XSettings::default_fgcom_port(){
	return QString("16661");
}
