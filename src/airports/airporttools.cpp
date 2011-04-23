
#include <QDebug>

#include <QtCore/QDirIterator>
#include <QtCore/QFile>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QtXml/QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>


#include <QtGui/QProgressDialog>

#include "airporttools.h"

AirportTools::AirportTools(QObject *parent, MainObject *mOb) :
    QObject(parent)
{

	mainObject = mOb;

}


//============================================================================
//== DataBase De/Re-construction
//============================================================================
void AirportTools::create_db_tables(){

	//* Drop and recreate the tables - then index after index later for speed..
	QStringList sql_commands;
	sql_commands.append("DROP TABLE IF EXISTS airports;");
	sql_commands.append("DROP TABLE IF EXISTS runways;");
	sql_commands.append("DROP TABLE IF EXISTS parking;");
	sql_commands.append("CREATE TABLE airports(code varchar(10) NOT NULL PRIMARY KEY, name varchar(50) NULL);");
	sql_commands.append("CREATE TABLE runways(airport_code varchar(10) NOT NULL, runway varchar(15), length int, lat float, lng float, heading float )");
	sql_commands.append("CREATE TABLE parking(airport_code varchar(10) NOT NULL, parking varchar(50), lat varchar(10), lng varchar(10), heading float )");

	execute_sql_commands_list(sql_commands);
}


//============================================================================
//== Database Indexes - Indexing is done after import of raw data
//============================================================================
void AirportTools::create_db_indexes(){

	//* Create the indexes.. we only really nees the airport_code,, runways an parking are done in GUI atmo
	QStringList sql_commands;
	sql_commands.append("CREATE INDEX airport_code_idx ON runways (airport_code);");
	//sql_commands.append("CREATE INDEX runway ON TABLE runways (runway);");
	sql_commands.append("CREATE INDEX airport_code_idx ON parking (airport_code);");

	execute_sql_commands_list(sql_commands);
}

//============================================================================
//== Execute Commands From List
//============================================================================
void AirportTools::execute_sql_commands_list(QStringList sql_commands){
	QSqlQuery query(mainObject->db);
	for(int i = 0; i < sql_commands.size(); ++i){
		qDebug() << sql_commands.at(i);
		if(!query.exec(sql_commands.at(i))){
			//TODO ignored for now - mysql is safe .. maybe
			//qDebug() << "OOps=" << mainObject->db.lastError(); //TODO

		}
	}
}

//============================================================================
//== Scan XML's for airports
//============================================================================
// This function finds the threshold.xml files
// The aiport CODE is derived from the file name in ICAO style
// The runways are the nodes
void AirportTools::scan_airports_xml(){

	int c = 0;
	int found = 0;

	//=================================
	//** Show something as this takes time
	QProgressDialog progress("Loading Airports to Cache", "Cancel", 0, 0);
	progress.setWindowModality(Qt::WindowModal);
	progress.setRange(0, 20000);
	progress.show();


	//* Drop and recreate database tables
	create_db_tables();

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
		if (c % 10 == 0){
			progress.setValue(c);
		}

		//* Get file handle if there is one
		xFile = loopAirportsFiles.next();

		//* Check if file entry is a *.threshold.xml - cos this is what we want
		if(xFile.endsWith(".threshold.xml") ){		

			//* Split out "<CODE>.threshold.xml" with a .
			QFileInfo fileInfoThreshold(xFile);
			QString code = fileInfoThreshold.fileName().split(".").at(0);

			//* Insert code to airports table == primary key
			sqlAirportInsert.bindValue(0, code);
			if(!sqlAirportInsert.exec()){
				qDebug() << "CRASH" << mainObject->db.lastError() << "=" << c;
				// TODO catch error log
				return;
			}else{
				//qDebug() << "APT=" << code << " c=" << c;
			}

			//* Parse the XML files for runways and parking
			parse_runways_xml(fileInfoThreshold.absoluteDir(), code);
			parse_parking_xml(fileInfoThreshold.absoluteDir(), code);



			found++;
		}

		if(c % 100 == 0){
			//QString str = QString("%1 airports found").arg(found);
			//statusBarAirports->showMessage(str);
		}
		if(progress.wasCanceled()){
			return;
		}
		if(c == 2000){
			qDebug() <<  "<, KILLED CRASH out";
			//progress.hide();
			//return;
			break;
		}
	}
	//* Create the database indexes
	create_db_indexes();
	progress.setLabelText("Creating Indexes");
	progress.hide();
}

void AirportTools::parse_ils_xml(QDir dir, QString airport_code){

}


//==============================================================
// Parse the <CODE>.threshold.xml file to get runways
//==============================================================
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
void AirportTools::parse_runways_xml(QDir dir, QString airport_code){

	//* Prepare the Insert Runway Query
	QSqlQuery sqlRunwayInsert(mainObject->db);
	sqlRunwayInsert.prepare("INSERT INTO runways(airport_code, runway, heading, lat, lng)VALUES(?,?,?,?,?);");

	//* Get the contents of the file whcile is path and code..
	QFile fileXmlThrehsold(dir.absolutePath().append("/").append(airport_code).append(".threshold.xml"));
	fileXmlThrehsold.open(QIODevice::ReadOnly);

	//* Make file contents into a string from bytearray
	QString xmlThresholdString = fileXmlThrehsold.readAll();

	//* Create domDocument - important dont pass string in  QDomConstrucor(string) as ERRORS.. took hours DONT DO IT
	QDomDocument dom;
	dom.setContent(xmlThresholdString); //* AFTER dom has been created, then set the content from a string from the file

	//* Get threhold nodes and loop for values to database
	QDomNodeList nodesThreshold = dom.elementsByTagName("threshold");
	if (nodesThreshold.count() > 0){
		for(int idxd =0; idxd < nodesThreshold.count(); idxd++){
			 QDomNode thresholdNode = nodesThreshold.at(idxd);
			 sqlRunwayInsert.bindValue(0, airport_code);
			 sqlRunwayInsert.bindValue(1, thresholdNode.firstChildElement("rwy").text());
			 sqlRunwayInsert.bindValue(2, thresholdNode.firstChildElement("hdg-deg").text());
			 sqlRunwayInsert.bindValue(3, thresholdNode.firstChildElement("lat").text());
			 sqlRunwayInsert.bindValue(4, thresholdNode.firstChildElement("lon").text());
			 if(!sqlRunwayInsert.exec()){
				//* TODO - ignore error for now
			 }
		}
	}
}

//================================================================
// Parse the <groundnet/parking>.threshold.xml file for Parking Postiton
//================================================================
/*
<?xml version="1.0"?>
<groundnet>
  <frequencies>
	<AWOS>12807</AWOS>
	<CLEARANCE>12197</CLEARANCE>
	<GROUND>12190</GROUND>
	<TOWER>12447</TOWER>
	<APPROACH>13497</APPROACH>
  </frequencies>
  <parkingList>
	<Parking index="0"
			 type="gate"
			 name="EGLL London Heathrow Ramp #11111"
			 number="1"
			 lat="N51 28.192"
			 lon="W00 27.892"
			 heading="56"
			 radius="54"
			 airlineCodes="" />
  .... snipped ....
*/
void AirportTools::parse_parking_xml(QDir dir, QString airport_code){

	//* Prepare the Insert Parking Query
	QSqlQuery sqlParkingInsert(mainObject->db);
	sqlParkingInsert.prepare("INSERT INTO parking(airport_code, parking, heading, lat, lng)VALUES(?,?,?,?,?);");

	//* Files in terrasync are named "groundnet.xml"; in scenery their "parking.xml" -- Why asks pete??
	QString fileName(dir.absolutePath().append("/").append(airport_code));
	fileName.append(mainObject->settings->value("use_terrasync").toBool() == true ? ".groundnet.xml" : ".parking.xml");

	//* Check parking file exists
	if(QFile::exists(fileName)){

		//* Open file and read contents to string
		QFile ppfile(fileName);
		ppfile.open(QIODevice::ReadOnly);
		QString xmlString = ppfile.readAll();

		//* Create domDocument - important - don't pass string in  QDomConstrucor(string) as ERRORS.. took hours DONT DO IT
		QDomDocument dom;
		dom.setContent(xmlString); //* AFTER dom has been created, then set the content from a string from the file

		//* Get <Parking/> nodes and loop thru them
		QDomNodeList parkingNodes = dom.elementsByTagName("Parking");
		if (parkingNodes.count() > 0){
			for(int idxd =0; idxd < parkingNodes.count(); idxd++){

				 QDomNode parkingNode = parkingNodes.at(idxd);
				 QDomNamedNodeMap attribs = parkingNode.attributes();

				//* Check it doesnt already exist - pete is confused as to multipkle entries
				 if(!listParkingPositions.contains(attribs.namedItem("name").nodeValue())){

					//* insert = airport_code, parking, heading, lat, lng
					sqlParkingInsert.bindValue(0, airport_code);
					sqlParkingInsert.bindValue(1, attribs.namedItem("name").nodeValue());
					sqlParkingInsert.bindValue(2, attribs.namedItem("heading").nodeValue());
					sqlParkingInsert.bindValue(3, attribs.namedItem("lat").nodeValue());
					sqlParkingInsert.bindValue(4, attribs.namedItem("lon").nodeValue());

					if(!sqlParkingInsert.exec()){
						//TODO - ignore error qDebug() << mainObject->db.lastError();
					}
					//* Append position to eliminate dupes
					listParkingPositions.append(attribs.namedItem("name").nodeValue());
				}
			}

		}
	} /* File Exists */
	/*
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

