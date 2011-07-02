
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

void AirportsData::import(QWidget *parent, MainObject *mainObject, bool import_icao_only){

	//====================================
	// Step 1: Get a hash map of aircraft descriptions from aptdat
	QHash<QString, QString> airports;
    QString msg;
    QTime tm;
    int   ms;
#ifdef USE_LOCAL_ZLIB
    QString zf = mainObject->settings->fg_root("/Airports/apt.dat.gz");
    QString zfile = mainObject->settings->apt_dat_file(); // for test only
    fgx_gzHandle gzf; // opaque file gz handle
    QFile f;
    // just to test the TEXT interface
    //if (f.exists(zfile)) {
    //} else
    if (f.exists(zf)) {
        zfile = zf;
    } else {
        outLog("ERROR: Failed to find ["+zf+"! NO AIRPORT FILE DATA!");
        return;
    }
    tm.start();
    gzf = fgx_gzOpen(zfile);
    if (!gzf) {
        outLog("ERROR: Failed to open ["+zfile+"]");
        return;
    }
    outLog("Processing file ["+zfile+"]");
#else
    QFile file( mainObject->settings->apt_dat_file() );
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		qDebug("OOPS: file problem");
		return;
    }
#endif // USE_LOCAL_ZLIB

    int air_count = 0;
	int line_counter = 0;
	int estimated_lines = 1510000;
	QRegExp rxICAOAirport("[A-Z]{4}");


	QProgressDialog progress("Importing Airports", "Cancel", 0, estimated_lines, parent);
	progress.setWindowTitle("Importing Airports");
	progress.setWindowModality(Qt::WindowModal);
	progress.setFixedWidth(400);
	progress.setWindowIcon(QIcon(":/icons/import"));
	progress.setMinimumDuration(0);

	//* ignore first line
#ifdef USE_LOCAL_ZLIB
    fgx_gzReadline(gzf);
    QString credits = fgx_gzReadline(gzf);
#ifdef ADD_EXTRA_DEBUG
    outLog("Credits: ["+credits+"]");
#endif
#else
    file.readLine();
	//** second line contains the version string
	QString credits = file.readLine();
#endif
    int version = 999;
	if(credits.startsWith("810 Version")){
		version = 810;
	}

	 QString airport_code;
	 QString airport_name;
	 QString elevation;
	 QString tower;

#ifdef USE_LOCAL_ZLIB
     while ( ! fgx_gzEof(gzf)) {
         QString line = fgx_gzReadline(gzf);
#ifdef ADD_EXTRA_DEBUG
         zf.sprintf("%6d: ", (line_counter + 1));
         outLog(zf+"["+line+"]");
#endif
#else
    while( !file.atEnd() ){
        QByteArray lineBytes = file.readLine();
        QString line = QString(lineBytes).trimmed();
#endif
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
			return;

		}
		line_counter++;
		if(line_counter % 1000 == 0){
			progress.setValue(line_counter);
			QString prog_text = QString("%1 of approx %2").arg(line_counter).arg(estimated_lines);
			progress.setLabelText(prog_text);
			progress.repaint();
		}



	} /* end while readline */
#ifdef USE_LOCAL_ZLIB
    fgx_gzClose(gzf);
#endif
    msg.sprintf("Done %d lines, found %d airport entries",line_counter,air_count);
    outLog(msg+", in "+getElapTimeStg(tm.elapsed()));
	//===================================================
	// Step Two - walk the xml sets
	int c = 0;
	int found = 0;
    int air_added = 0;
    ms = tm.restart();

	//= Cache File
	QFile cacheFile( mainObject->settings->data_file("airports.txt") );
	if(!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        //qDebug() << "TODO Open error cachce file=";
        outLog("ERROR: Failed to open cache file "+cacheFile.fileName());
		return;
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
		c++;
		xFileName = loopAirportsFiles.next();

		//= Check if file entry is a *.threshold.xml - cos this is what we want
		if(xFileName.endsWith(".threshold.xml") ){

			//= Split out "CODE.threshold.xml" with a "."
			QFileInfo fileInfoThreshold(xFileName);
			QString airport_code = fileInfoThreshold.fileName().split(".").at(0);

			//* Update progress
			progress.setValue(progress.value() + 1);
			progress.setLabelText(airport_code);

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
			return;
		}
	}

	cacheFile.close();
    msg.sprintf("Walked %d files, found %d threshold.xml, appended %d to cache",
                c, found, air_added);
    outLog(msg+", in "+getElapTimeStg(tm.elapsed()));
	progress.hide();
}
