

#include <QFile>
#include <QHash>
#include <QProgressDialog>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>
#include <QTime>

#include "airports/airportsdata.h"
#include "utilities/utilities.h"
#include "utilities/fgx_gzlib.h" /* FGx interface to glib */

#define USE_LOCAL_ZLIB
#undef ADD_EXTRA_DEBUG

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
    QString msg;
    QTime tm;
    int   ms;

	QString zf = mainObject->settings->fgroot("/Airports/apt.dat.gz");
    fgx_gzHandle gzf; // opaque file gz handle
    QFile f;
    if (!f.exists(zf)) {

        outLog("ERROR: Failed to find ["+zf+"! NO AIRPORT FILE DATA!");
		return true;
    }
    tm.start();
    gzf = fgx_gzOpen(zf);
    if (!gzf) {
        outLog("ERROR: Failed to open ["+zf+"]");
		return true;
    }
    outLog("Processing file ["+zf+"]");

    int air_count = 0;
	int line_counter = 0;

	QRegExp rxICAOAirport("[A-Z]{4}");



	//* ignore first line
    fgx_gzReadline(gzf);
    QString credits = fgx_gzReadline(gzf);

    int version = 999;
	if(credits.startsWith("810 Version")){
		version = 810;
	}

	 QString airport_code;
	 QString airport_name;
	 QString elevation;
	 QString tower;


     while ( ! fgx_gzEof(gzf)) {
         QString line = fgx_gzReadline(gzf);

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
                        air_count++;
					}
			}else{
					airports[airport_code] = airport_name;
                    air_count++;
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


    fgx_gzClose(gzf);

    msg.sprintf("Done %d lines, found %d airport entries",line_counter,air_count);
    outLog(msg+", in "+getElapTimeStg(tm.elapsed()));


	//===================================================
	// Step Two - walk the xml sets

	progress.setValue(0);
	progress.setWindowTitle("Scanning Airport Directories");
	progress.setRange(0, 20000);


	int c = 0;
	int found = 0;
    int air_added = 0;
    ms = tm.restart();

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

    msg = "Scanning XML files";
    outLog(msg);
    progress.setWindowTitle(msg);
	progress.setRange(0, 20000);

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
                    air_added++;
				}
			}else{
				out << cols.join("\t").append("\n");
                air_added++;
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

    msg.sprintf("Walked %d files, found %d threshold.xml, appended %d to cache",
                c, found, air_added);
    outLog(msg+", in "+getElapTimeStg(tm.elapsed()));
	progress.hide();
	return false;
}
