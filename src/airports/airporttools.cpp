
#include <QDebug>

#include <QtCore/QDirIterator>
#include <QtCore/QFile>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QtXml/QXmlStreamReader>


#include <QtGui/QProgressDialog>

#include "airporttools.h"

AirportTools::AirportTools(QObject *parent, MainObject *mOb) :
    QObject(parent)
{

	mainObject = mOb;

}


//============================================================================
//== Scan XML's for airports
//============================================================================
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

void AirportTools::scan_airports_xml(){

	int c = 0;
	int found = 0;
	qDebug() << "AirportTools::scan_airports_xml";
	//return;
	//=================================
	//** Show something as this takes time
	QProgressDialog progress("Loading Airports to Cache", "Cancel", 0, 0);
	progress.setWindowModality(Qt::WindowModal);
	progress.setRange(0, 20000);
	progress.show();


	//=================================
	//* DataBase De/Re-construction
	//* Drop and recreate the Aiports table - then index after import later
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


	//================================================
	//* Lets Loop the directories
	//qDebug() << mainObject->settings->airports_path();

	//* Get out aiports path from setings and get the the subdir also
	QString airports_dir_path = mainObject->settings->airports_path();
	QDirIterator loopAirportsFiles(airports_dir_path, QDirIterator::Subdirectories);
	QString xFile;

	while (loopAirportsFiles.hasNext()) {

		//* Update counter and progress periodically
		c++;
		if (c % 100 == 0){
			progress.setValue(c);
		}

		//* Get file handle if there is one
		xFile = loopAirportsFiles.next();

		//* Check if file entry is a *.threshold.xml - cos this is what we want
		if(xFile.endsWith(".threshold.xml") ){

			//** This is an airport - so import
			QFileInfo fileInfoThreshold(xFile);

			//* Split out "<CODE>.threshold.xml" with a period
			QString code = fileInfoThreshold.fileName().split(".").at(0);

			//* Insert code to aiports table == primary key
			sqlAirportInsert.bindValue(0, code);
			if(!sqlAirportInsert.exec()){
				qDebug() << "CRASH" << mainObject->db.lastError();
				// TODO catch error log
				return;
			}else{
				qDebug() << "Insert Aiport Done";
			}

			//* Parse the XMl file for runways
			parse_runways(fileInfoThreshold);

			found++;
		}

		if(c % 100 == 0){
			QString str = QString("%1 airports found").arg(found);
			//statusBarAirports->showMessage(str);
		}
		if(progress.wasCanceled()){
			return;
		}
		if(c == 100){
			qDebug() <<  "100 and  ..k CRASH out";
			progress.hide();
		}
	}

	progress.hide();
}




//==============================================================
// Parse the Threshold file to get runways
//==============================================================
void AirportTools::parse_runways(QFileInfo thresholdXmlFileInfo){

	//* Insert Runway Query
	QSqlQuery sqlRunwayInsert(mainObject->db);
	sqlRunwayInsert.prepare("INSERT INTO runways(airport_code, runway, heading, length, lat, lng)VALUES(?,?,?,?,?,?);");


	//*** Load Runways
	QStringList runwayList;
	QFile thresholdXmlFile(thresholdXmlFileInfo.absoluteFilePath());
	if (thresholdXmlFile.open(QIODevice::ReadOnly)) {

		QXmlStreamReader rwyreader(&thresholdXmlFile);
		QXmlStreamReader::TokenType tokenType;

		while ((tokenType = rwyreader.readNext()) != QXmlStreamReader::EndDocument) {
			if (rwyreader.name() == "rwy") {
				runwayList << rwyreader.readElementText();

			}
		}

		thresholdXmlFile.close();
	}
	qDebug() <<"==================\n" << thresholdXmlFileInfo.absoluteFilePath();
	qDebug() << "runway" << runwayList;


	//runwayList.removeDuplicates();
	//runwayList.sort();

	/*
	QTreeWidgetItem *runwaysParent = new QTreeWidgetItem();
	runwaysParent->setText(0, "Runways" );
	runwaysParent->setIcon(0, QIcon(":/icon/folder"));
	treeWidgetRunways->addTopLevelItem(runwaysParent);
	treeWidgetRunways->setItemExpanded(runwaysParent, true);
	if(runwayList.count() == 0){
		QTreeWidgetItem *item = new QTreeWidgetItem(runwaysParent);
		item->setText(0, "None");
	}else{
		for(i =0; i < runwayList.count(); i++){
			QTreeWidgetItem *item = new QTreeWidgetItem(runwaysParent);
			item->setText(0, runwayList.at(i));
			item->setText(1, "runway");
		}
	}
	*/

	//*** Load Parking Positions
	/*
	QStringList parkingPositions;
	QString parkPosFile = QString(airportXmlFile);
	parkPosFile.chop(13); // strip the threshold.xml part

	//terrasync check because with terrasync scenery parking.xml is renamed to groundnet.xml
	if (mainObject->settings->value("use_terrasync").toBool() == true) {
	parkPosFile.append("groundnet.xml");
	} else {
		parkPosFile.append("parking.xml");
	}

	QFile ppfile(parkPosFile);
	if(ppfile.open(QIODevice::ReadOnly)) {

		QXmlStreamReader ppreader(&ppfile);
		QXmlStreamReader::TokenType tokenType;

		while ((tokenType = ppreader.readNext()) != QXmlStreamReader::EndDocument) {
			if (ppreader.name() == "Parking") {
				QXmlStreamAttributes attributes = ppreader.attributes();

				if (attributes.value("type").toString() == "gate" && attributes.value("name").toString() != "Startup Location") {
					QString ppname = attributes.value("name").toString();
					QString ppnumber = attributes.value("number").toString();
					QString ppall ;
					ppall.append(ppname); ppall.append(ppnumber);
					parkingPositions << ppall;
				}

			}
		}

		ppfile.close();
	}
	parkingPositions.removeDuplicates();
	parkingPositions.sort();
	*/
	/*
	QTreeWidgetItem *parkingParent = new QTreeWidgetItem();
	parkingParent->setText(0, "Park Positions" );
	parkingParent->setIcon(0, QIcon(":/icon/folder"));
	treeWidgetRunways->addTopLevelItem(parkingParent);
	treeWidgetRunways->setItemExpanded(parkingParent, true);
	if(parkingPositions.count() == 0){
		QTreeWidgetItem *item = new QTreeWidgetItem(parkingParent);
		item->setText(0, "None");
	}else{
		for(i =0; i < parkingPositions.count(); i++){
			QTreeWidgetItem *item = new QTreeWidgetItem(parkingParent);
			item->setText(0, parkingPositions.at(i));
			item->setText(1, "stand");
		}
	}

	QString messagecount;
	messagecount.append(QString("%1 Runways, ").arg(runwaysParent->childCount()));
	if(parkingPositions.count() != 0) {
		messagecount.append(QString("%1 Park Position(s)").arg(parkingParent->childCount()));
	} else {
		messagecount.append(QString("No Park Positions."));
	}

	statusBarRunways->showMessage(messagecount);
	*/
}

