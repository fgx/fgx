
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

	qDebug() << mainObject->settings->airports_path();
	QString directory = mainObject->settings->airports_path();


	//* Clear any records in the database
	QSqlQuery sql("delete from airports;", mainObject->db);
	sql.exec();

	//* Insert Aircraft query
	QSqlQuery sqlApt(mainObject->db);
	sqlApt.prepare("INSERT INTO airports(code)VALUES(?)");


	//QStringList airportsList;
	QString entry;

	QDirIterator it(directory, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		progress.setValue(c);
		entry = it.next();
		if(entry.endsWith(".threshold.xml") ){
			QFileInfo info(entry);
			//#airportsList << QString("%1~|~%2").arg(	entry,
			//										info.fileName().split(".").at(0)
			//									);
			//qDebug() << entry << info.fileName();
			sqlApt.bindValue(0, info.fileName().split(".").at(0));
			sqlApt.exec();
			found++;
		}
		c++;
		if(c % 100 == 0){
			QString str = QString("%1 airports found").arg(found);
			//statusBarAirports->showMessage(str);
		}
		if(progress.wasCanceled()){
			return;
		}
	}

	//mainObject->settings->setValue("cache/airports", airportsList);
	//mainObject->settings->sync();
	progress.hide();
}

