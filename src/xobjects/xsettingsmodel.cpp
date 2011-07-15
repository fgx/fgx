

#include <QDebug>

#include "xobjects/xsettingsmodel.h"

XSettingsModel::XSettingsModel(MainObject *mob, QObject *parent) :
    QStandardItemModel(parent)
{
	mainObject = mob;

	setColumnCount(7);

	QStringList headers;
	headers  << "Option" << "Ena" << "Val" << "Default" << "Level" << "Description" << "Class";
	setHorizontalHeaderLabels(headers);

	//======================================
	// This list wil be the main issue and debates for a long time probably

	add_option( "show_mapmap",false, "", "",10,"Follow in MpMap","Map");


	add_option( "--enable-auto-coordination",false, "", "",10,"Enabled Rudder and Airelon","Control");

	add_option( "--full-screen",false, "", "", 1 ,"Enable Full Screen at Startup","Startup");
	add_option( "--disable-splash-screen", false, "", "", 1 ,"Enable Splash Screen at Startup","Startup");


	add_option( "--callsign=", true,"", "fgx001", 1 ,"Your Callsign","Core");

	add_option( "--aircraft=", false,"", "?", 1 ,"Aircraft","Aircraft");

	add_option( "--airport=", false,"", "?", 1 ,"Airport","Airport");


	add_option( "--multiplay=in", false,"", ",10,localhost,20",2,"Multiplayer In","MultiPlayer");
	add_option( "--multiplay=out",false, "", ",10,localhost,20",2,"Multiplayer In","MultiPlayer");
	add_option( "--fgcom=",false, "", "",3,"FgCom","FgCom");

}



//==================================================
// == Add an Option
void XSettingsModel::add_option( QString option, bool enabled, QString value, QString preset, int level, QString description, QString area)
{

	QList<QStandardItem *>items;
	items 	<< new QStandardItem(option)
			<< new QStandardItem(enabled ? "1" : "0")
			<< new QStandardItem(value)
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
	emit upx(enabled, option, value);	

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
//= File Functions
//==================================================
QString XSettingsModel::ini_file_path()
{
	return mainObject->settings->data_file("profile.ini");
}


// == Write Ini
void XSettingsModel::write_ini()
{
	QSettings settings(ini_file_path(), QSettings::IniFormat);
	for(int row_idx=0; row_idx < rowCount(); row_idx++){
		settings.beginGroup(item(row_idx, C_OPTION)->text());
			settings.setValue( "enabled", item(row_idx, C_ENABLED)->text());
			settings.setValue( "value", item(row_idx, C_VALUE)->text());
		settings.endGroup();
	}

}


// == Read Ini
void XSettingsModel::read_ini()
{
	QSettings settings(ini_file_path(), QSettings::IniFormat);
	for(int row_idx=0; row_idx < rowCount(); row_idx++){
		settings.beginGroup(item(row_idx, C_OPTION)->text());
			item(row_idx, C_ENABLED)->setText( settings.value("enabled").toBool() ? "1" : "0" );
			item(row_idx, C_VALUE)->setText( settings.value("value").toString() );
			emit upx(item(row_idx, C_ENABLED)->text() == "1", item(row_idx, C_OPTION)->text(), item(row_idx, C_VALUE)->text());
		settings.endGroup();
	}
}
