/*
 *  xsettingsmodel.cpp
 *  FGx
 *
 *  Created by Peter Morgan
 *  © 2011 --- GPL2
 *
 */
#include "app_config.h"
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QDir>
#include <QFileDialog>

#include "xobjects/xsettingsmodel.h"
#include "utilities/utilities.h"


/*! \class XSettingsModel
 * \brief The XSettingsModel is an extended QStandardItemModel, and the main "setting and state" for FGx
 *
 *
 * @authors: Peter Morgan
 */



XSettingsModel::XSettingsModel(MainObject *mob, QObject *parent) :
	QStandardItemModel(parent)
{
	mainObject = mob;

	_loading = false;

	setColumnCount(7);

	QStringList headers;
	headers  << "Option" << "Ena" << "Val" << "Default" << "Level" << "Description" << "Class";
	setHorizontalHeaderLabels(headers);



	//=================================================================================
	// This list will be the main issue and debates for a long time probably, said pete
	//=================================================================================


	//==================
	add_option("profile", false, "", "", 0, "", "profiles");
	
	//==================
	add_option("fgfs_path", false,"","",0,"","paths");
	add_option("fgroot_path", false,"","",0,"","paths");
	add_option("terrasync_enabled", false,"","",0,"","paths");
	add_option("terrasync_exe_path", false,"","",0,"","paths");
	add_option("terrasync_data_path", false,"","",0,"","paths");
	add_option("custom_scenery_enabled", false,"","",0,"","paths");
	add_option("custom_scenery_path", false,"","",0,"","paths");
    add_option("fgcom_exe_path", false, "", "", 0, "", "paths");
	add_option("fgcom_enabled", false, "", "", 0, "", "paths");
	add_option("jsdemo_enabled", false, "", "", 0, "", "paths");
    add_option("jsdemo_exe_path", false, "", "", 0, "", "paths");



	add_option("extra_args", false,"","",0,"","expert");
	add_option("extra_env", false,"","",0,"","expert");

	add_option("runtime", false,"","",0,"","expert");
	add_option("--log-level=", false,"","",0,"","expert");

	add_option("first_launcher_close", false, "", "", 0, "Check for launcher window close", "Launcher Window");

    // Coordinates
    add_option( "use_coordinates",false, "", "",3,"","Coordinates");
    add_option( "--vor=",false, "", "",3,"","Coordinates");
    add_option( "--ndb=",false, "", "",3,"","Coordinates");
    add_option( "--fix=",false, "", "",3,"","Coordinates");
    add_option( "--lat=",false, "", "",3,"","Coordinates");
    add_option( "--lon=",false, "", "",3,"","Coordinates");
    add_option( "--offset-distance=",false, "", "",3,"","Coordinates");
    add_option( "--altitude=",false, "", "",3,"","Coordinates");
    add_option( "--heading=",false, "", "",3,"","Coordinates");

    //==


	add_option( "show_mpmap", false, "", "",10,"Follow in MpMap","Map");


	add_option( "--enable-auto-coordination",false, "", "",10,"Enabled Rudder and Airelon","Control");


	//== Screen
	add_option("--geometry=", false, "", "", 1, "Screen Size","");
	add_option( "--full-screen",false, "", "", 1 ,"Enable Full Screen at Startup","Startup");
	add_option( "--disable-splash-screen", false, "", "", 1 ,"Enable Splash Screen at Startup","Startup");


	add_option( "--callsign=", true,"", "fgx001", 1 ,"Your Callsign","Core");

	//= Season
	add_option("--season=", false,"","summer",1,"Season", "time");

	//= Time
	add_option("--timeofday=", false,"","realtime",1,"Time of day", "time");

	//= Weather
	add_option("weather",						true, "","none",2,"Weather Flag","weather");
	add_option("--enable-real-weather-fetch",	false, "","",1,"Enable Real Weather","weather");
	add_option("--disable-real-weather-fetch",	false, "","",1,"Disable Real Weather","weather");
	add_option("--metar=",						false,"","",1,"","weather");


	//= MultiPlayer
	add_option( "--multiplay=in",  false,"", ",10,localhost,20",2,"Multiplayer In","MultiPlayer");
	add_option( "--multiplay=out", false, "", ",10,localhost,20",2,"Multiplayer In","MultiPlayer");

	//= FGCom Related
	add_option( "--fgcom=",false, "", "fgcom.flightgear.org.uk:16661",3,"FgCom","FgCom");
	add_option( "fgcom_enabled", false, "", "",10,"FgCom Socket","FgCom");

	//= Local Servers
	add_option( "--telnet=",false, "", "",3,"Enable Telnet","servers");
	add_option( "--httpd=",false, "", "",3,"Enable Httpd","servers");
	add_option( "--jpg-httpd=",false, "", "",3,"Enable Screenshot","servers");


	//-- Radio
	add_option( "--nav1=",false, "", "",3,"","Radio");
	add_option( "--nav2=",false, "", "",3,"","Radio");
	add_option( "--adf1=",false, "", "",3,"","Radio");
	add_option( "--adf2=",false, "", "",3,"","Radio");
	add_option( "--com1=",false, "", "",3,"","Radio");
	add_option( "--com2=",false, "", "",3,"","Radio");

	//== Aircraft
	add_option( "--aircraft=", false,"", "", 1 ,"Aircraft","Aircraft");
	add_option( "--fg-aircraft=", false,"", "", 1 ,"Custom Aircraft Folder","Aircraft");
	add_option( "custom_hangar_enabled",false, "", "",1,"Custom Hangar Checked","Aircraft");



	//-- Fuel
	//add_option( "use_default_fuel",true,"","",3,"","Aircraft");
	add_option( "--enable-fuel-freeze",false,"","",3,"","Aircraft");
	add_option( "--prop:/consumables/fuels/tank[1]/level-gal=",false, "", "",9,"","Fuel");
	add_option( "--prop:/consumables/fuels/tank[2]/level-gal=",false, "", "",9,"","Fuel");
	add_option( "--prop:/consumables/fuels/tank[3]/level-gal=",false, "", "",9,"","Fuel");

	//= Airport Tab
	add_option( "start_postition", true, "","0",0,"Start Location", "Airport");
	add_option( "--airport=", false,"", "", 1 ,"Airport","Airport");
	add_option( "--runway=", false,"", "", 1 ,"Runway","Airport");
	add_option( "--parking-id=", false,"", "", 1 ,"Parking","Airport");
	add_option("runway_stand", false, "", "",1,"Flag to whether runway or stand", "Airport" );



}



//==================================================
// == Add an Option
void XSettingsModel::add_option( QString option, bool enabled, QString value, QString preset, int level, QString description, QString area)
{
	Q_UNUSED(value); //= Uses preset intead
	QList<QStandardItem *>items;
	items 	<< new QStandardItem(option)
			<< new QStandardItem(enabled ? "1" : "0")
			<< new QStandardItem(preset)
			<< new QStandardItem(preset)
			<< new QStandardItem(QString::number(level))
			<< new QStandardItem(description)
			<< new QStandardItem(area);
	appendRow(items);

}



//==================================================
// == Set An Option
void XSettingsModel::set_option(QString option, bool enabled, QString value)
{
	//qDebug() << "set " << option << _loading;
	if(_loading){
		return;
	}
	//= Find item matching the "option"
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	//qDebug() << "opts" << items;

	//TODO handle error if not found
    if (items.size() == 0) {
        outLog("set_option:setx called with INVALID option ["+option+"]");
        return;
    }

	//= Get/update the "enabled" item in the same row
	QStandardItem *eItem = item(items[0]->row(),C_ENABLED);
	eItem->setText(enabled ? "1" : "0");


	//= Get/update the "value" item in the same row
	QStandardItem *vItem = item(items[0]->row(),C_VALUE);
	vItem->setText(value);

	set_row_bg(items[0]->row(), enabled ? QColor(200,255,200) : QColor(240,240,240));

	//= Announce the change
	//emit upx(option, enabled,  value);
	emit updated(get_fgfs_list());
}

//==================================================
// == Set An Option Enabled or Disabled
void XSettingsModel::set_enabled(QString option, bool enabled)
{
	//qDebug() << "set " << option << _loading;
	if(_loading){
		return;
	}
	//= Find item matching the "option"
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	//qDebug() << "opts" << items;

	//TODO handle error if not found

	//= Get/update the "enabled" item in the same row
	QStandardItem *eItem = item(items[0]->row(),C_ENABLED);
	eItem->setText(enabled ? "1" : "0");

	set_row_bg(items[0]->row(), enabled ? QColor(200,255,200) : QColor(240,240,240));

	QStandardItem *vItem = item(items[0]->row(),C_VALUE);

	emit upx(option, enabled, vItem->text());
	emit updated(get_fgfs_list());
}

//==================================================
// == Set An Option's  Value
void XSettingsModel::set_value(QString option, QString value)
{
	//qDebug() << "set " << option << _loading;
	if(_loading){
		return;
	}
	//= Find item matching the "option"
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	//qDebug() << "opts" << items;

	//TODO handle error if not found

	//= Get/update the "enabled" item in the same row
	QStandardItem *vItem = item(items[0]->row(),C_VALUE);
	vItem->setText(value);

	QStandardItem *eItem = item(items[0]->row(),C_ENABLED);



	emit upx(option, eItem->text() == "1",  value);
	emit updated(get_fgfs_list());
}



QModelIndex XSettingsModel::get_index(QString option)
{
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	return indexFromItem(items[0]);
}

//==================================================
// == Get Option
//==================================================
QString XSettingsModel::getx(QString option)
{
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);

	// Get the item in the same row in the enabled field
	QStandardItem *xItem = item(items[0]->row(),C_VALUE);
	return xItem->text();

}

QString XSettingsModel::getx(QString option, bool return_default)
{
	//=PETE - I dont think this is being used...
	Q_UNUSED(return_default);
	XOpt opt = get_opt(option);
	if(opt.value.length() == 0){
		return opt.default_value;
	}
	return opt.value;
}

QString XSettingsModel::getx(QString option, QString default_string)
{
	QString v = getx(option);
	if(v.length() == 0){
		return default_string;
	}
	return v;

}

//==================================================
// == Get Option enabled
bool XSettingsModel::get_ena(QString option)
{
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);

	// Get the item in the same row in the enabled field
	QStandardItem *xItem = item(items[0]->row(),C_ENABLED);
	return xItem->text() == "1";

}

XOpt XSettingsModel::get_opt(QString option)
{
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	return XOpt(item(items[0]->row(), C_OPTION)->text(),
				item(items[0]->row(), C_ENABLED)->text() == "1",
				item(items[0]->row(), C_VALUE)->text(),
				item(items[0]->row(), C_DEFAULT)->text()
				);
}


//==================================================
// == Read Default Ini File: First Startup and Reset
/** \brief Read Values from ini
 */

void XSettingsModel::read_default_ini()
{
	QString defaultSettings("");
	switch (mainObject->runningOs()) {
		case MainObject::MAC:
			defaultSettings = ":/default/osx_default.ini";
			break;
		case MainObject::WINDOWS:
			defaultSettings = ":/default/win_default.ini";
			break;
		case MainObject::LINUX:
			defaultSettings = ":/default/x_default.ini";
			break;
		default:
			outLog("*** FGx shout: No default settings for this system");
			break;
	}
	
	QSettings settings(defaultSettings,QSettings::IniFormat);

	bool ena;
	for(int row_idx=0; row_idx < rowCount(); row_idx++){
		//= loop rows and load each "option" as an [ini section] with enabled, value as values
		settings.beginGroup(item(row_idx, C_OPTION)->text());
			ena = settings.value("enabled").toBool() ;
			item(row_idx, C_ENABLED)->setText( ena ? "1" : "0");
			QString val = settings.value("value").toString();
			if(val == ""){
				val = item(row_idx, C_DEFAULT)->text();
			}
			item(row_idx, C_VALUE)->setText(val );
			set_row_bg(row_idx, ena ? QColor(200,255,200) : QColor(240,240,240));
			//= Broadcast changes
			emit upx(item(row_idx, C_OPTION)->text(),
					 item(row_idx, C_ENABLED)->text() == "1",
					 item(row_idx, C_VALUE)->text()
					 );
		settings.endGroup();
	}
	//qDebug() << "Read ini";
	emit updated(get_fgfs_list());
	
}

//=============================================
// == Load Last Profile
/** \brief Opens last used profile
 */

void XSettingsModel::load_last_profile(QString profile)
{
	_loading = true;
	
	QString filename = profile;
	QSettings settings(filename,QSettings::IniFormat);
	
	outLog("*** FGx loading last used profile:"+filename);
	
	bool ena;
	for(int row_idx=0; row_idx < rowCount(); row_idx++){
		//= loop rows and load each "option" as an [ini section] with enabled, value as values
		settings.beginGroup(item(row_idx, C_OPTION)->text());
		ena = settings.value("enabled").toBool() ;
		item(row_idx, C_ENABLED)->setText( ena ? "1" : "0");
		QString val = settings.value("value").toString();
		if(val == ""){
			val = item(row_idx, C_DEFAULT)->text();
		}
		item(row_idx, C_VALUE)->setText(val );
		set_row_bg(row_idx, ena ? QColor(200,255,200) : QColor(240,240,240));
		//= Broadcast changes
		emit upx(item(row_idx, C_OPTION)->text(),
				 item(row_idx, C_ENABLED)->text() == "1",
				 item(row_idx, C_VALUE)->text()
				 );
		settings.endGroup();
	}
	_loading = false;
	emit updated(get_fgfs_list());
	
}

//=============================================
// == Load Profile
/** \brief Opens Profile Dialog for loading a .ini File
 */

void XSettingsModel::load_profile()
{
	_loading = true;
#ifdef USE_ALTERNATE_GETFILE
    QString filename = util_getFileName(0,  "Load Profiles",  profile(), QStringList("*.ini") );
#else // !#ifdef USE_ALTERNATE_GETFILE
    QString filename = QFileDialog::getOpenFileName(0,  "Load Profiles",  profile(), "Profile files (*.ini)" );
#endif // #ifdef USE_ALTERNATE_GETFILE y/n
	QSettings settings(filename,QSettings::IniFormat);
	
	bool ena;
	for(int row_idx=0; row_idx < rowCount(); row_idx++){
		//= loop rows and load each "option" as an [ini section] with enabled, value as values
		settings.beginGroup(item(row_idx, C_OPTION)->text());
		ena = settings.value("enabled").toBool() ;
		item(row_idx, C_ENABLED)->setText( ena ? "1" : "0");
		QString val = settings.value("value").toString();
		if(val == ""){
			val = item(row_idx, C_DEFAULT)->text();
		}
		item(row_idx, C_VALUE)->setText(val );
		set_row_bg(row_idx, ena ? QColor(200,255,200) : QColor(240,240,240));
		//= Broadcast changes
		emit upx(item(row_idx, C_OPTION)->text(),
				 item(row_idx, C_ENABLED)->text() == "1",
				 item(row_idx, C_VALUE)->text()
				 );
		settings.endGroup();
	}
	_loading = false;
	emit updated(get_fgfs_list());
	outLog("*** Profile loaded: "+filename);
	
}

//=============================================
// == Save Profile
/** \brief Opens Profile Dialog for loading a .ini File
 */

void XSettingsModel::save_profile()
{
	
#ifdef USE_ALTERNATE_GETFILE
    QString filename = util_getFileName(0, "Save Profiles", "NewProfile.ini", QStringList("*.ini"));
#else // !#ifdef USE_ALTERNATE_GETFILE
    QString filename = QFileDialog::getSaveFileName(0, "Save Profiles", "NewProfile.ini", "Profile files (*.ini)" );
#endif // #ifdef USE_ALTERNATE_GETFILE y/n
    QSettings settings(filename,QSettings::IniFormat);
	
	// selected profile filename will be stored in settings
	set_option("profile", true, filename);
	
	// saved profile will be used in settings to load as last used profile
	QSettings lastused;
	lastused.setValue("lastprofile", filename);
	
	//= loop rows and save each "option" as an [ini section] with enabled, value as values
	for(int row_idx=0; row_idx < rowCount(); row_idx++){
		settings.beginGroup(item(row_idx, C_OPTION)->text());
		settings.setValue( "enabled", item(row_idx, C_ENABLED)->text());
		settings.setValue( "value", item(row_idx, C_VALUE)->text());
		settings.endGroup();
	}
	
	
	
	outLog("*** Profile written to disk: "+filename);
	
}




//==========================================================
//= Get Options
//==========================================================
QStringList XSettingsModel::get_fgfs_args()
{
	//= Read --options from tree
	QStringList args;

	for(int row_idx=0; row_idx < rowCount(); row_idx++){

		if(item(row_idx, C_ENABLED)->text() == "1"){
			QString str("");
			QString op = item(row_idx, C_OPTION)->text();
			if(op.startsWith("--")){
				str.append(item(row_idx, C_OPTION)->text()).append(item(row_idx, C_VALUE)->text());
			}
			if(str.length() > 0){
				args << str;
			}


			

		}
	}
	
	// Process unique items, like fgcom socket
	if(fgcom_enabled()){
		args << "--generic=socket,out,10,localhost,16661,udp,fgcom";
	}

	//= add Extra args
	XOpt opt = get_opt("extra_args");
	if (opt.enabled and  opt.value.length() > 0) {
		QStringList parts = opt.value.split("\n");
		if(parts.count() > 0){
			for(int i=0; i < parts.count(); i++){
				QString part = parts.at(i).trimmed();
				if(part.length() > 0){
					args << part;
				}
			}
		}
	}

	//== FgRoot
	args <<  QString("--fg-root=").append(fgroot());

	//= Terrasync and custom scenery path
    // setup the DIFFERENT path separators!!!
    QString path_sep = ":"; // normal unix path separator
#ifdef Q_OS_WIN
    path_sep = ";"; // oops, switch the WINDOWS spearator
#endif
    if (terrasync_enabled() && !custom_scenery_enabled()) {
        args << QString("--fg-scenery=").append(terrasync_data_path()).append(path_sep).append(scenery_path());
		args << QString("--atlas=socket,out,5,localhost,5505,udp");
    }
	
    else if (terrasync_enabled() && custom_scenery_enabled()) {
        args << QString("--fg-scenery=").append(custom_scenery_path()).append(path_sep).append(terrasync_data_path()).append(path_sep).append(scenery_path());
		args << QString("--atlas=socket,out,5,localhost,5505,udp");
	}

	else if (!terrasync_enabled() && custom_scenery_enabled())  {
        args << QString("--fg-scenery=").append(custom_scenery_path()).append(path_sep).append(scenery_path());
	}
	
	else if (!terrasync_enabled() && !custom_scenery_enabled())  {
		args << QString("--fg-scenery=").append(scenery_path());
	}
	
	return args;
}


QStringList XSettingsModel::get_fgfs_list()
{
	//TODO append the commands here
	QStringList args;
	args << fgfs_path();
	args << get_fgfs_args();
	return args;
}

QString XSettingsModel::get_fgfs_command_string()
{
	return fgfs_path().append(" ").append( get_fgfs_args().join(" ") );
}



//========================================================
//** Get Enviroment

QStringList XSettingsModel::get_fgfs_env(){

	QStringList args;
	QString extra = getx("extra_env");
	if (extra.length() > 0) {
		QStringList parts = extra.split("\n");
		if(parts.count() > 0){
			for(int i=0; i < parts.count(); i++){
				QString part = parts.at(i).trimmed();
				if(part.length() > 0){
					args << part;
				}
			}
		}
	}
	return args;
}

//===========================================================================
//== Get recent profile
//===========================================================================
/** \brief Path to fgfs executable
 */
QString XSettingsModel::profile(){
	return QString(getx("profile"));
	
}


//===========================================================================
//== fgfs Executable
//===========================================================================
/** \brief Path to fgfs executable
 */
QString XSettingsModel::fgfs_path(){
	QSettings firstsettings;
	if(!firstsettings.value("firststartup").toBool()){
		return QDir::currentPath().append("/").append(getx("fgfs_path"));
	}else {
		return QString(getx("fgfs_path"));
	}


}

//===========================================================================
//== terrasync enabled/disabled
//===========================================================================
/** \brief Sert terrasync enabled/disabled
 */
bool XSettingsModel::terrasync_enabled(){
	return get_ena("terrasync_enabled");
	
}


//===========================================================================
//== terrasync Executable
//===========================================================================
/** \brief Path to terrasync executable
 */
QString XSettingsModel::terrasync_exe_path(){
	QSettings firstsettings;
	if(!firstsettings.value("firststartup").toBool()){
		return QDir::currentPath().append("/").append(getx("terrasync_exe_path"));
	}else{
		return QString(getx("terrasync_exe_path"));
	}
	
}

//===========================================================================
//== terrasync Data Path
//===========================================================================
/** \brief Path to terrasync executable
 */
QString XSettingsModel::terrasync_data_path(){
	return QString(getx("terrasync_data_path"));
}

//===========================================================================
//** Return FGCom enabled/disabled
//===========================================================================

bool XSettingsModel::fgcom_enabled(){
	return get_ena("fgcom_enabled");
}

//===========================================================================
//== fgcom Executable
//===========================================================================
/** \brief Path to terrasync executable
 */

QString XSettingsModel::fgcom_exe_path(){
	QSettings firstsettings;
	if(!firstsettings.value("firststartup").toBool()){
		return QDir::currentPath().append("/").append(getx("fgcom_exe_path"));
	}else {
		return QString(getx("fgcom_exe_path"));
	}
}


	
	
	
	
QString XSettingsModel::jsdemo_exe_path(){
    return "";	//nothing
	
}



//===========================================================================
//== fgroot  path
//===========================================================================
/** \return The absolute path to FG_ROOT ie /fgdata directory */
QString XSettingsModel::fgroot(){
	QSettings firstsettings;
	if(!firstsettings.value("firststartup").toBool()){
		return QDir::currentPath().append("/").append(getx("fgroot_path"));
	}else {
		return QString(getx("fgroot_path"));
	}
}


/** \brief Path to FG_ROOT with appended path
 *
 * Returns the path to the FG_ROOT. If the default install
 * is selected, then that is returned, otherwise the custom selected fg_data path.
 * The file is appended with the append_path
 * \return The absolute path.
 */

QString XSettingsModel::fgroot(QString append_path){
	return this->fgroot().append(append_path);
}

//===========================================================================
//** Paths Sane
//===========================================================================
/** \brief Checks whether the executablem FG_ROOT paths are sane.
 *
 * \return true if sane
 */
bool XSettingsModel::paths_sane(){
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
  * \return fgroot/Aircraft absolute path --fg-aircraft path in case
 */
QString XSettingsModel::aircraft_path(){
	
	if (!mainObject->X->get_ena("custom_hangar_enabled")) {
		return fgroot().append("/Aircraft");
	}else{
		return mainObject->X->getx("--fg-aircraft=");
	}
}

/** \brief Path to the /Aircraft directory with a dir appended.
 *
 * \return absolute path.
 */
QString XSettingsModel::aircraft_path(QString dir){
	return fgroot().append("/Aircraft/").append(dir);
}


//===========================================================================
//** Airports path
//===========================================================================
/** \brief Return the absolute path to the /Airports directory
 *
 * \return If TerraSync is enabled, then returns a terrasync folder, otherwise the default.
 */
QString XSettingsModel::airports_path(){
	QString rpath;
	//= Using terrasync
	if(terrasync_enabled()){
		//if(mainObject->runningOs() == MainObject::MAC){   // what the h. says gral to himself
		//	rpath = QDir::homePath();
		//	rpath.append("/Documents/TerrasyncScenery");
		//}else{
			// Use the terra sync path
		rpath = terrasync_data_path().append("/Airports");
		}
	
	else{ // Otherwise return the FG_ROOT airports/
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
QString XSettingsModel::scenery_path(){

	return fgroot("/Scenery");
}

//===========================================================================
//** Custom scenery
//===========================================================================
QString XSettingsModel::custom_scenery_path(){
	return getx("custom_scenery_path");
}

bool XSettingsModel::custom_scenery_enabled(){
	return get_ena("custom_scenery_path");
}




void XSettingsModel::set_row_bg(int row_idx, QColor  bg_color)
{
	for(int col_idx=0; col_idx < columnCount(); col_idx++)
	{
		item(row_idx, col_idx)->setBackground(bg_color);
	}
}



