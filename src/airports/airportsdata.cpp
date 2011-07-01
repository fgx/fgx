
#include <QFile>
#include <QHash>
#include <QProgressDialog>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>

#include "airports/airportsdata.h"



//AirportsData::AirportsData()
//{
//}

bool AirportsData::import(QProgressDialog &progress, MainObject *mainObject, bool import_icao_only){

	//====================================
	// Step 1: Get a hash map of aircraft descriptions from aptdat
	int estimated_lines = 1510000;

	progress.setValue(0);
	progress.setWindowTitle("Importing Apt Dat");
	progress.setRange(0, estimated_lines);
	progress.repaint();

	QHash<QString, QString> airports;

	QFile file( mainObject->settings->apt_dat_file() );
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug("OOPS: file problem");
		return true;
	 }

	int line_counter = 0;

	QRegExp rxICAOAirport("[A-Z]{4}");



	//* ignore first line
	file.readLine();

	//** second line contains the version string
	QString credits = file.readLine();
	int version = 999;
	if(credits.startsWith("810 Version")){
		version = 810;
	}

	 QString airport_code;
	 QString airport_name;
	 QString elevation;
	 QString tower;

	while( !file.atEnd() ){

		QByteArray lineBytes = file.readLine();
		QString line = QString(lineBytes).trimmed();
		//qDebug() << line;
		QString row_code = line.section(' ',0, 0);
		//qDebug() << row_code;
		QStringList parts = line.split(" ", QString::SkipEmptyParts);

		//**********************************************************************
		//*** Airport
		if(row_code == "1"){

			// http://data.x-plane.com/file_specs/Apt715.htm
			// 0 = airport code
			//  1 = elevation
			// 2 = has tower
			// 3 = not approp
			// 4 = code
			// 5+ description

			airport_code = parts[4];
			elevation = parts[1];
			tower =  parts[2] == "1" ? "1" : "";
			airport_name.clear();
			for(int p = 5; p < parts.size(); p++){ //** TODO WTF ?
				airport_name.append(parts[p]).append(" ");
			}
			if(import_icao_only){
				if( rxICAOAirport.exactMatch(airport_code) ){
						airports[airport_code] = airport_name;
					}
			}else{
					airports[airport_code] = airport_name;
			} /* if(is_icao) */

		} /* if(row_code == "1") airport */



		if (progress.wasCanceled()){
			progress.hide();
			return true;

		}
		line_counter++;
		if(line_counter % 1000 == 0){
			progress.setValue(line_counter);
			QString prog_text = QString("%1 of approx %2").arg(line_counter).arg(estimated_lines);
			progress.setLabelText(prog_text);

			progress.repaint();
		}



	} /* end while readline */


	//====================================================================================
	// Step Two - walk the xml sets

	progress.setValue(0);
	progress.setWindowTitle("Scanning Airport Directories");
	progress.setRange(0, 20000);


	int c = 0;
	int found = 0;

	//= Cache File
	QFile cacheFile( mainObject->settings->data_file("airports.txt") );
	if(!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		//qDebug() << "TODO Open error cachce file=";
		return true;
	}
	QTextStream out(&cacheFile);


	//================================================
	//* Lets Loop the directories
	//* Get out aiports path from setings and get the the subdir also
	QDirIterator loopAirportsFiles( mainObject->settings->airports_path(), QDirIterator::Subdirectories );
	QString xFileName;


	while (loopAirportsFiles.hasNext()) {

		//= Get file handle if there is one
		xFileName = loopAirportsFiles.next();

		//= Check if file entry is a *.threshold.xml - cos this is what we want
		if(xFileName.endsWith(".threshold.xml") ){

			//= Split out "CODE.threshold.xml" with a "."
			QFileInfo fileInfoThreshold(xFileName);
			QString airport_code = fileInfoThreshold.fileName().split(".").at(0);

			//* Update progress
			if(c % 100 == 0){
				progress.setValue(c);
				progress.setLabelText(xFileName);
				progress.repaint();
			}

			QString airport_name("");
			if(airports.contains(airport_code)){
				airport_name = airports.value(airport_code);
			}

			QStringList cols; // missing in middle is description ??
			cols << airport_code << airport_name << fileInfoThreshold.absoluteDir().absolutePath();

			if( import_icao_only){
				if( rxICAOAirport.exactMatch(airport_code) ){
					out << cols.join("\t").append("\n");
				}
			}else{
				out << cols.join("\t").append("\n");
			}

			found++;
		}

		if(progress.wasCanceled()){
			progress.hide();
			return true;
		}
		c++;
	}

	cacheFile.close();
	return false;
}
