
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
//** Scan XML's for airports
//============================================================================
void AirportTools::scan_airports_xml(){
	int c = 0;
	int found = 0;
	QProgressDialog progress("Loading Airports to Cache", "Cancel", 0, 0);
	progress.setWindowModality(Qt::WindowModal);
	progress.setRange(0, 20000);
	progress.show();

	qDebug() << mainObject->settings->airports_path();
	QString directory = mainObject->settings->airports_path();


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
	return;
	//* Insert Aircraft query
	QSqlQuery sqlApt(mainObject->db);
	sqlApt.prepare("INSERT INTO airports(code)VALUES(?);");


	//QStringList airportsList;PageName
	QString entry;

	QDirIterator it(directory, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		c++;
		if (c % 100 == 0){
			progress.setValue(c);
		}
		entry = it.next();
		if(entry.endsWith(".threshold.xml") ){
			QFileInfo info(entry);
			//#airportsList << QString("%1~|~%2").arg(	entry,
			//										info.fileName().split(".").at(0)
			//
			// Split out "FOO.threshold.xml"
			QString code = info.fileName().split(".").at(0);
			//qDebug() << entry << info.fileName() << code;
			sqlApt.bindValue(0, code);
			if(!sqlApt.exec()){
				qDebug() << mainObject->db.lastError();
			}
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

