#ifndef AIRPORTSIMPORT_H
#define AIRPORTSIMPORT_H

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <QtGui/QProgressDialog>

#include "xobjects/mainobject.h"

class AirportsImport : public QObject
{
Q_OBJECT
public:
	explicit AirportsImport(QObject *parent, MainObject *mOb);

	MainObject *mainObject;

	//* The lists hold unique entries
	QStringList listParkingPositions;
	QStringList listAirportCodes;

	//QProgressDialog progressDialog;
	void import_airports(QWidget *parent);
	
	//void execute_sql_commands_list(QStringList sql_commands);
	//void create_db_tables();
	//void create_db_indexes();


	void parse_runways_xml(QDir dir, QString airport_code);
	void parse_ils_xml(QDir dir, QString airport_code);
	void parse_parking_xml(QDir dir, QString airport_code);

	void parse_aptdat(QProgressDialog progressDialog);
	void update_aptdat_airport(QStringList parts);


signals:

public slots:

};

#endif // AIRPORTSIMPORT_H
