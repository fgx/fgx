

#include <QDebug>

#include "xobjects/xsettingsmodel.h"




/*! \class XSettingsModel
 * \brief The XSettingsModel is an extended QStandardItemModel, and the main "setting and state" for FGx
 *
 *
 * @author: Peter Morgan
 */



XSettingsModel::XSettingsModel(MainObject *mob, QObject *parent) :
    QStandardItemModel(parent)
{
	mainObject = mob;

	setColumnCount(7);

	QStringList headers;
	headers  << "Option" << "Ena" << "Val" << "Default" << "Level" << "Description" << "Class";
	setHorizontalHeaderLabels(headers);

	//========================================================================
	// This list wil be the main issue and debates for a long time probably, said pete
	//========================================================================


	//+++++++
	add_option("fgfs_custom_path", false,"","",0,"","paths");
	add_option("fgroot_custom_path", false,"","",0,"","paths");
	add_option("terrasync_path", false,"","",0,"","paths");

	//==


	add_option( "show_mpmap", false, "", "",10,"Follow in MpMap","Map");


	add_option( "--enable-auto-coordination",false, "", "",10,"Enabled Rudder and Airelon","Control");

	add_option( "--full-screen",false, "", "", 1 ,"Enable Full Screen at Startup","Startup");
	add_option( "--disable-splash-screen", false, "", "", 1 ,"Enable Splash Screen at Startup","Startup");


	add_option( "--callsign=", true,"", "fgx001", 1 ,"Your Callsign","Core");



	add_option( "--airport=", false,"", "", 1 ,"Airport","Airport");


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
	add_option( "enable_mp", false, "","",1,"","");
	add_option( "--multiplay=in",  false,"", ",10,localhost,20",2,"Multiplayer In","MultiPlayer");
	add_option( "--multiplay=out", false, "", ",10,localhost,20",2,"Multiplayer In","MultiPlayer");

	//= FGCom Related
	add_option( "--fgcom=",false, "", "fgcom.flightgear.org.uk:16661",3,"FgCom","FgCom");

	//= Local Servers
	add_option( "--telnet=",false, "", "",3,"Enable Telnet","servers");
	add_option( "--httpd=",false, "", "",3,"Enable Httpd","servers");
	add_option( "--jpg-httpd=",false, "", "",3,"Enable Screenshot","servers");


	//-- Radio
	add_option( "--nav1=",false, "", "",3,"","Radio");
	add_option( "--nav2=",false, "", "",3,"","Radio");
	add_option( "--adf=",false, "", "",3,"","Radio");
	add_option( "--com1=",false, "", "",3,"","Radio");
	add_option( "--com2=",false, "", "",3,"","Radio");

	add_option( "--aircraft=", false,"", "", 1 ,"Aircraft","Aircraft");
	add_option("use_default_aircraft",true,"","",1,"","Aircraft");
	
	//-- Fuel
	add_option( "--prop:/consumables/fuels/tank[1]/level-gal=",false, "", "",9,"","Fuel");
	add_option( "--prop:/consumables/fuels/tank[2]/level-gal=",false, "", "",9,"","Fuel");
	add_option( "--prop:/consumables/fuels/tank[3]/level-gal=",false, "", "",9,"","Fuel");



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
	//= Find item matching the "option"
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	//qDebug() << "opts" << items;

	//TODO handle error if not found

	//= Get/update the "enabled" item in the same row
	QStandardItem *eItem = item(items[0]->row(),C_ENABLED);
	eItem->setText(enabled ? "1" : "0");


	//= Get/update the "value" item in the same row
	QStandardItem *vItem = item(items[0]->row(),C_VALUE);
	vItem->setText(value);

	//= Announce the change
	emit upx(option, enabled,  value);
	emit uplines(get_command_lines());
}

//==================================================
// == Get Option
QString XSettingsModel::getx(QString option)
{
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	//#qDebug() << "opts" << items;

	// Get the item in the same row in the enabled field
	QStandardItem *xItem = item(items[0]->row(),C_VALUE);
	return xItem->text();

}


//==================================================
// == Get Option enabled
bool XSettingsModel::get_ena(QString option)
{
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	//#qDebug() << "opts" << items;

	// Get the item in the same row in the enabled field
	QStandardItem *xItem = item(items[0]->row(),C_ENABLED);
	return xItem->text() == "1";

}

//==================================================
//= File Functions
//==================================================
QString XSettingsModel::ini_file_path()
{
	return mainObject->settings->data_file("profile.ini");
}


// == Write Ini
void XSettingsModel::write_ini()
{
	//= create ini settings object
	QSettings settings(ini_file_path(), QSettings::IniFormat);

	//= loop rows and save each "option" as an [ini section] with enabled, value as values
	for(int row_idx=0; row_idx < rowCount(); row_idx++){
		settings.beginGroup(item(row_idx, C_OPTION)->text());
			settings.setValue( "enabled", item(row_idx, C_ENABLED)->text());
			settings.setValue( "value", item(row_idx, C_VALUE)->text());
		settings.endGroup();
	}
	qDebug() << "Written ini";
}


// == Read Ini
void XSettingsModel::read_ini()
{
	//= Create ini settings object
	QSettings settings(ini_file_path(), QSettings::IniFormat);

	for(int row_idx=0; row_idx < rowCount(); row_idx++){
		//= loop rows and load each "option" as an [ini section] with enabled, value as values
		settings.beginGroup(item(row_idx, C_OPTION)->text());
			item(row_idx, C_ENABLED)->setText( settings.value("enabled").toBool() ? "1" : "0" );
			QString val = settings.value("value").toString();
			if(val == ""){
				val = item(row_idx, C_DEFAULT)->text();
			}
			item(row_idx, C_VALUE)->setText(val );

			//= Broadcast changes
			emit upx(item(row_idx, C_OPTION)->text(),
					 item(row_idx, C_ENABLED)->text() == "1",
					 item(row_idx, C_VALUE)->text()
					 );
		settings.endGroup();
	}
	qDebug() << "Read ini";
}



//==========================================================
QStringList XSettingsModel::get_command_lines()
{
	QStringList lines;
	for(int row_idx=0; row_idx < rowCount(); row_idx++){
		if(item(row_idx, C_ENABLED)->text() == "1"){
			QString str("");
			if(item(row_idx, C_VALUE)->text().startsWith("--")){
				str.append(item(row_idx, C_OPTION)->text()).append(item(row_idx, C_VALUE)->text());
			}
			lines << str;
		}
	}
	return lines;
}

QString XSettingsModel::get_command_string()
{
	return get_command_lines().join(" ");
}



