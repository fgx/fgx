

#include <QDebug>

#include "xobjects/xsettingsmodel.h"

XSettingsModel::XSettingsModel(QObject *parent) :
    QStandardItemModel(parent)
{

	setColumnCount(7);

	QStringList headers;
	headers << "Enabled" << "Ki" << "Val" << "Default" << "Level" << "Description" << "Class";
	setHorizontalHeaderLabels(headers);

	//======================================
	// This list wil be the main issue and debates for a long time probably

	add_option(false, "--multiplay=in", "", ",10,localhost,20",2,"Multiplayer In","MultiPlayer");
	add_option(false, "--multiplay=out", "", ",10,localhost,20",2,"Multiplayer In","MultiPlayer");

	add_option(false, "--enable-auto-coordination", "", "",10,"Enabled Rudder and Airelon","Control");

	add_option(false, "--full-screen", "", "", 1 ,"Enable Full Screen at Startup","Startup");

	add_option(true, "--callsign=", "", "fgx001", 1 ,"Your Callsign","Core");

	add_option(false, "--aircraft=", "", "?", 1 ,"Aircraft","Aircraft");

	add_option(false, "--airport=", "", "?", 1 ,"Airport","Airport");

}



//==================================================
// == Add an Option
void XSettingsModel::add_option(bool enabled, QString option, QString value, QString preset, int level, QString description, QString area)
{

	QList<QStandardItem *>items;
	items << new QStandardItem(enabled ? "1" : "0")
				<< new QStandardItem(option)
				<< new QStandardItem(value)
				<< new QStandardItem(preset)
				<< new QStandardItem(QString::number(level))
				<< new QStandardItem(description)
				<< new QStandardItem(area);
	appendRow(items);

}



//==================================================
// == Set An Option
void XSettingsModel::set_option(bool enabled, QString option, QString value)
{
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	qDebug() << "opts" << items;

	// Get the item in the same row in the enabled field
	QStandardItem *eItem = item(items[0]->row(),C_ENABLED);
	eItem->setText(enabled ? "1" : "0");


	// Get the item in the same row in the value field
	QStandardItem *vItem = item(items[0]->row(),C_VALUE);
	vItem->setText(value);

	emit upx(enabled, option, value);
	//if(option == "--callsign=" || option == "--airport=" || option == "--aircraft="){
	//	emi
	//}

}


QString XSettingsModel::getx(QString option)
{
	QList<QStandardItem *>items = findItems(option, Qt::MatchExactly,C_OPTION);
	//#qDebug() << "opts" << items;

	// Get the item in the same row in the enabled field
	QStandardItem *xItem = item(items[0]->row(),C_VALUE);
	return xItem->text();

}

