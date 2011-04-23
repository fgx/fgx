
#include <QDebug>

#include <QtCore/QDirIterator>
#include <QtCore/QFile>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QtGui/QProgressDialog>

#include "airporttools.h"

AirportTools::AirportTools(QObject *parent, MainObject *mOb) :
    QObject(parent)
{

	mainObject = mOb;

}


//============================================================================
//== Scan XML's for airports
// This function finds the threshold.xml files
// The aiport CODE is derived from the file name in ICAO style
// The runways are the nodes
/*
<?xml version="1.0"?>
<PropertyList>
  <runway>
	<threshold>
	  <lon>0.044298885776989</lon>
	  <lat>51.505569223906</lat>
	  <rwy>10</rwy>
	  <hdg-deg>92.88</hdg-deg>
	  <displ-m>95</displ-m>
	  <stopw-m>55</stopw-m>
	</threshold>
	<threshold>
	  <lon>0.065996952433288</lon>
	  <lat>51.5048897753222</lat>
	  <rwy>28</rwy>
	  <hdg-deg>272.88</hdg-deg>
	  <displ-m>71</displ-m>
	  <stopw-m>90</stopw-m>
	</threshold>
  </runway>
</PropertyList>
*/
//============================================================================

void AirportTools::scan_airports_xml(){

	int c = 0;
	int found = 0;

	//=================================
	//** Show something as this takes time
	QProgressDialog progress("Loading Airports to Cache", "Cancel", 0, 0);
	progress.setWindowModality(Qt::WindowModal);
	progress.setRange(0, 20000);
	progress.show();


	//=================================
	//* DataBase
	//* Drop and recreate the Aiports table - then idex after import later
	QStringList sql_commands;
	sql_commands.append("DROP TABLE IF EXISTS airports;");
	sql_commands.append("DROP TABLE IF EXISTS runways;");
	sql_commands.append("CREATE TABLE airports(code varchar(10) NOT NULL PRIMARY KEY, name varchar(50) NULL);");
	sql_commands.append("CREATE TABLE runways(airport_code varchar(10) NOT NULL, runway varchar(15), length int, lat float, lng float, heading float )");

	QSqlQuery query(mainObject->db);

	for(int i = 0; i < sql_commands.size(); ++i){
		qDebug() << sql_commands.at(i);

		if(!query.exec(sql_commands.at(i))){
			qDebug() << "OOps=" << mainObject->db.lastError();
			return;
		}
	}

	//* Insert Airport query
	QSqlQuery sqlAirportInsert(mainObject->db);
	sqlAirportInsert.prepare("INSERT INTO airports(code)VALUES(?);");

	//* Insert Runway Query
	QSqlQuery sqlRunwayInsert(mainObject->db);
	sqlRunwayInsert.prepare("INSERT INTO runways(airport_code, runway, heading, length, lat, lng)VALUES(?,?,?,?,?,?);");


	//================================================
	//* Lets Loop the directories
	//qDebug() << mainObject->settings->airports_path();
	QString airports_dir_path = mainObject->settings->airports_path();
	QString entry;
	QDirIterator it(airports_dir_path, QDirIterator::Subdirectories);
	while (it.hasNext()) {

		//* Update counter and progress periodically
		c++;
		if (c % 100 == 0){
			progress.setValue(c);
		}

		//* Check if file entry is a *.threshold.xml
		entry = it.next();
		if(entry.endsWith(".threshold.xml") ){

			//** This is an airport - so import
			QFileInfo fileInfoThreshold(entry);

			//* Split out "<CODE>.threshold.xml" with a period
			QString code = fileInfoThreshold.fileName().split(".").at(0);

			//* Insert code to aiports table == primary key
			sqlApt.bindValue(0, code);
			if(!sqlApt.exec()){
				qDebug() << "CRASH" << mainObject->db.lastError();
				// TODO catch error log
				return;
			}

			//* Parse the XMl file for runways


			found++;
		}

		if(c % 100 == 0){
			QString str = QString("%1 airports found").arg(found);
			//statusBarAirports->showMessage(str);
		}
		if(progress.wasCanceled()){
			return;
		}
		if(c == 1000){
			qDebug() <<  "CRASH out";
			progress.hide();
		}
	}

	//mainObject->settings->setValue("cache/airports", airportsList);
	//mainObject->settings->sync();
	progress.hide();
}

