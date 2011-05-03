
#include <QDebug>

#include <QtCore/QDirIterator>
#include <QtCore/QFile>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QtXml/QXmlStreamReader>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>


#include <QtGui/QProgressDialog>

#include "airportsimport.h"

AirportsImport::AirportsImport(QObject *parent, MainObject *mOb) :
    QObject(parent)
{

	mainObject = mOb;

	progressDialog.setWindowModality(Qt::WindowModal);
	progressDialog.hide();

}


//============================================================================
//== DataBase De/Re-construction
//============================================================================
void AirportsImport::create_db_tables(){

	progressDialog.setLabelText(tr("Destroy and Create database tables"));
	//* Drop and recreate the tables - then index after index later for speed..
	QStringList sql_commands;
	sql_commands.append("DROP TABLE IF EXISTS airports;");
	sql_commands.append("DROP TABLE IF EXISTS runways;");
	sql_commands.append("DROP TABLE IF EXISTS ils;");
	sql_commands.append("DROP TABLE IF EXISTS parking;");
	sql_commands.append("CREATE TABLE airports(code varchar(10) NOT NULL PRIMARY KEY, name varchar(50) NULL);");
	sql_commands.append("CREATE TABLE runways(airport_code varchar(10) NOT NULL, runway varchar(15), length int, lat float, lon float, heading float );");
	sql_commands.append("CREATE TABLE ils(airport_code varchar(10) NOT NULL, runway varchar(15) NOT NULL, ident varchar(10), lat float, lon float, heading float );");
	sql_commands.append("CREATE TABLE stands(airport_code varchar(10) NOT NULL, stand varchar(50), lat varchar(10), lon varchar(10), heading float );");

	execute_sql_commands_list(sql_commands);
}


//============================================================================
//== Database Indexes - Indexing is done after import of raw data
//============================================================================
void AirportsImport::create_db_indexes(){

	progressDialog.setLabelText(tr("Create database indexes"));
	//* Create the indexes.. we only really nees the airport_code,, runways an parking are done in GUI atmo
	QStringList sql_commands;
	sql_commands.append("CREATE INDEX airport_code_idx ON runways (airport_code);");
	sql_commands.append("CREATE INDEX airport_runway_idx ON ils (airport_code, runway);");
	sql_commands.append("CREATE INDEX airport_code_idx ON stands (airport_code);");

	execute_sql_commands_list(sql_commands);
}


//============================================================================
//== Execute Commands From List
//============================================================================
void AirportsImport::execute_sql_commands_list(QStringList sql_commands){
	QSqlQuery query(mainObject->db);
	for(int i = 0; i < sql_commands.size(); ++i){
		if(!query.exec(sql_commands.at(i))){
			//TODO ignored for now - mysql is safe .. maybe said pete
			//qDebug() << "OOps=" << mainObject->db.lastError(); //TODO
		}
	}
}





//============================================================================
//== Import Airports
//============================================================================
// * Nukes then recreates tables
// * Recursive search got .threshold.xml files
// * The aiport CODE is derived from the file name in ICAO style
// * The runways, ils, and stands imported (ils unused atmo)
// * Create the db index
// * Import AptDat
void AirportsImport::import_airports(){

	int c = 0;
	int found = 0;

	//=================================
	//** Show Progress as this takes time
	progressDialog.setLabelText("Loading Airports to Cache");
	progressDialog.setRange(0, 20000);
	progressDialog.show();

	//* Drop and recreate database tables
	create_db_tables();

	//* Insert Airport query
	QSqlQuery sqlAirportInsert(mainObject->db);
	sqlAirportInsert.prepare("INSERT INTO airports(code)VALUES(?);");

	//================================================
	//* Lets Loop the directories
	//* Get out aiports path from setings and get the the subdir also
	QDirIterator loopAirportsFiles( mainObject->settings->airports_path(), QDirIterator::Subdirectories );
	QString xFileName;

	while (loopAirportsFiles.hasNext()) {

		//* Get file handle if there is one
		c++;
		xFileName = loopAirportsFiles.next();

		//* Check if file entry is a *.threshold.xml - cos this is what we want
		if(xFileName.endsWith(".threshold.xml") ){

			//* Split out "CODE.threshold.xml" with a "."
			QFileInfo fileInfoThreshold(xFileName);
			QString airport_code = fileInfoThreshold.fileName().split(".").at(0);

			//* Update progress periodically
			if (c % 1000 == 0){
				progressDialog.setValue(c);
				QString progLabel = QString("%2").arg(c).arg(airport_code);
				progressDialog.setLabelText(progLabel);
			}

			//* Insert airport_code to airports table == primary key
			sqlAirportInsert.bindValue(0, airport_code);
			if(!sqlAirportInsert.exec()){
				//qDebug() << "CRASH" << mainObject->db.lastError() << "=" << c;
				// TODO catch error log
			}else{
				listAirportCodes.append(airport_code);
			}

			//* Parse the XML files
			parse_runways_xml(fileInfoThreshold.absoluteDir(), airport_code);
			parse_ils_xml(fileInfoThreshold.absoluteDir(), airport_code);
			parse_parking_xml(fileInfoThreshold.absoluteDir(), airport_code);



			found++;
		}

		if(progressDialog.wasCanceled()){
			return;
		}
		if(c == 5000){
			//qDebug() <<  "<, KILLED CRASH out";
			//progress.hide();
			//return;
			break;
		}
	}

	//* Create the database indexes
	create_db_indexes();

	//* Import APTDAT
	parse_aptdat();

	progressDialog.hide();
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
void AirportsImport::parse_runways_xml(QDir dir, QString airport_code){

	//* Prepare the Insert Runway Query
	QSqlQuery sqlRunwayInsert(mainObject->db);
	sqlRunwayInsert.prepare("INSERT INTO runways(airport_code, runway, heading, lat, lon)VALUES(?,?,?,?,?);");

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




//=======================================================================
// Parse the ILS files
//=======================================================================
/*
<?xml version="1.0"?>
<PropertyList>
  <runway>
	<ils>
	  <lon>-0.756683</lon>
	  <lat>51.2823330000001</lat>
	  <rwy>06</rwy>
	  <hdg-deg>61.98</hdg-deg>
	  <elev-m>69.49</elev-m>
	  <nav-id>IFRG</nav-id>
	</ils>
  </runway>
</PropertyList>
*/

void AirportsImport::parse_ils_xml(QDir dir, QString airport_code){

	//* Prepare the Insert ILS Query
	QSqlQuery sqlILSInsert(mainObject->db);
	sqlILSInsert.prepare("INSERT INTO ils(airport_code, runway, ident, heading, lat, lon)VALUES(?,?,?,?,?,?);");

	QString fileName(dir.absolutePath().append("/").append(airport_code).append(".ils.xml"));

	//* Check ILS file exists
	if(QFile::exists(fileName)){

		//* Open file and read contents to string
		QFile ilsFile(fileName);
		ilsFile.open(QIODevice::ReadOnly);
		QString xmlString = ilsFile.readAll();

		//* Create domDocument - important - don't pass string in  QDomConstrucor(string) as ERRORS.. took hours DONT DO IT
		QDomDocument dom;
		dom.setContent(xmlString); //* AFTER dom has been created, then set the content from a string from the file

		//* Get <ils/> nodes and loop thru them
		QDomNodeList ilsNodes = dom.elementsByTagName("ils");
		if (ilsNodes.count() > 0){
			for(int idxd =0; idxd < ilsNodes.count(); idxd++){

				 QDomNode ilsNode = ilsNodes.at(idxd);
				//* insert = airport_code, runway, ident, heading, lat, lon
				sqlILSInsert.bindValue(0, airport_code);
				sqlILSInsert.bindValue(1, ilsNode.firstChildElement("rwy").text());
				sqlILSInsert.bindValue(2, ilsNode.firstChildElement("nav-id").text());
				sqlILSInsert.bindValue(3, ilsNode.firstChildElement("hdg-deg").text());
				sqlILSInsert.bindValue(4, ilsNode.firstChildElement("lat").text());
				sqlILSInsert.bindValue(5, ilsNode.firstChildElement("lon").text());

				if(!sqlILSInsert.exec()){
					//TODO - ignore error
					//qDebug() << mainObject->db.lastError() << "=" << ilsNode.firstChildElement("rwy").text();
				}

			}

		}
	} /* File Exists */
}





//=======================================================================
// Parse the <groundnet/parking>.threshold.xml file for Parking Postiton
//=======================================================================
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
void AirportsImport::parse_parking_xml(QDir dir, QString airport_code){

	//* Prepare the Insert Parking Query
	QSqlQuery sqlParkingInsert(mainObject->db);
	sqlParkingInsert.prepare("INSERT INTO stands(airport_code, stand, heading, lat, lon)VALUES(?,?,?,?,?);");

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

					//* insert = airport_code, parking, heading, lat, lon
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

}



//====================================================================================================
// Parse apt.dat file.. ta Robin
//====================================================================================================
void AirportsImport::parse_aptdat(){

	progressDialog.setLabelText("Parsing AptDat");
	progressDialog.setRange(0, 1510000);

	//* Open the apt.dat file (TODO not dure is this is always unzipped
	QFile file(mainObject->settings->apt_dat_file());
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug("OOPS: file problem");
		return;
	 }

	//* Update DB queries

	QSqlQuery queryRunwayUpdate;
	queryRunwayUpdate.prepare("TODO");

	//====================================================
	//* start File read >>

	//* ignore first line
	file.readLine();

	//* second line contains the version string - unused ATMO
	QString credits = file.readLine();
	int version = -9999;
	if(credits.startsWith("810 Version")){
		version = 810;
	}else{
		version = -1; //* Future use
	}

	//* Prepare the loop variables
	int line_count = 0;
	QRegExp rxICAOAirport("[A-Z]{4}");



	//** Now loop the aptdat file
	while( !file.atEnd() ){

		line_count++;

		QByteArray lineBytes = file.readLine();
		QString line = QString(lineBytes).trimmed();
		//qDebug() << line;

		QString row_code = line.section(' ',0, 0);
		//qDebug() << row_code;
		QStringList parts = line.split(" ", QString::SkipEmptyParts);

		//==========================================
		//* Process Rows
		if(row_code == "1"){
			update_aptdat_airport(parts);
		}

		if(line_count % 100 == 0){
			QString progress_text = QString("AptDat line: %1").arg(line_count);
			progressDialog.setLabelText(progress_text);
			progressDialog.setValue(line_count);
		}

		//* Get outta here
		if(progressDialog.wasCanceled()){
			return;
		}
	}
}

//=====================================
// Update aptdat airport row
//=====================================
/* http://data.x-plane.com/file_specs/Apt715.htm
  0 = airport code
  1 = elevation
  2 = has tower
  3 = not approp
  4 = code
  5+ description
*/
void AirportsImport::update_aptdat_airport(QStringList parts){

	QString airport_code;
	QString airport_name;
	int elevation;
	QString tower;

	QSqlQuery queryAirportUpdate;
	queryAirportUpdate.prepare("UPDATE airports SET name=? WHERE code=?;");

	//* Airport code
	airport_code = parts[4];

	//* Check its in our import list..
	if( listAirportCodes.contains(airport_code) ){

		//* Get values from row
		elevation = parts[1].toInt();
		tower =  parts[2] == "1" ? "1" : "";
		airport_name.clear();
		for(int p = 5; p < parts.size(); p++){ //** loop to the end to get description (spit on spaces..) - TODO neater way
			airport_name.append(parts[p]).append(" ");
		}
		queryAirportUpdate.bindValue(0,airport_name.trimmed()  );
		queryAirportUpdate.bindValue(1, airport_code  );
		if(!queryAirportUpdate.exec()){
			qDebug() << queryAirportUpdate.lastError();
		}
	}
}
