// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // airportsdata.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

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


bool AirportsData::import(QProgressDialog &progress, MainObject *mainObject){


	QHash<QString, QString> airports;
    QString msg;
    QTime tm;
    int   ms;

	progress.setValue(0);
	progress.setWindowTitle("Scanning Airport Directories");
	progress.setRange(0, 50000);


	int c = 0;
	int found = 0;
    int air_added = 0;
    ms = tm.restart();
	
	// Removing cache file, if exists()
	if (QFile::exists(mainObject->data_file("airports.txt"))) {
		outLog("*** FGx airportsdata reload: cache file exists!");
		QFile::remove(mainObject->data_file("airports.txt"));
		outLog("*** FGx airportsdata reload: REMOVED AIRPORTS CACHE FILE");
	}

	//= Cache File
	QFile cacheFile( mainObject->data_file("airports.txt") );
	if(!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		//qDebug() << "TODO Open error cachce file=";
		return true;
	}
	QTextStream out(&cacheFile);


	//================================================
	//* Lets Loop the directories
	//* Get out aiports path from setings and get the the subdir also
	QDirIterator loopAirportsFiles( mainObject->X->airports_path(), QDirIterator::Subdirectories );
	QString xFileName;

    msg = "*** FGx airportsdata reload: Scanning XML files in "+mainObject->X->airports_path();
    outLog(msg);
    progress.setWindowTitle(msg);
	progress.setRange(0, 50000);

    // Check the fgfs additional argument list,
    // and/or any additional scenery path inputs
    // *** take care NOT to duplicate ***
	QStringList fgfs_args = mainObject->X->get_fgfs_args();
    int i, ind;
    QDir d;
    QString path;
#ifdef Q_OS_WIN
    QChar psep(';');
#else
    QChar psep(':');
#endif
	
	// AIIIIII, found the doubler !, said yves very very loud
    for (i = 0; i < fgfs_args.size(); i++) {
        msg = fgfs_args.at(i);
        ind = msg.indexOf(QChar('"'));
        if (ind == 0)
            msg = msg.mid(1,msg.length()-2);
        if (msg.indexOf("--fg-scenery=") == 0) {
            // got a scenery folder to scan
            msg = msg.mid(13);
            ind = msg.indexOf(QChar('"'));
            if (ind == 0)
                msg = msg.mid(1,msg.length()-2);
            QStringList path_list = msg.split(psep);
			int pathnumber = 0;
            for( QStringList::ConstIterator entry = path_list.begin(); entry != path_list.end(); entry++) {
                path = *entry;
				pathnumber = pathnumber + 1;
                if (d.exists(path)) {
                    // we have a PATH to check, but we are ONLY checking 'Airports'
                    if ( !(path.indexOf(QChar('/')) == (path.size()-1)) &&
                         !(path.indexOf(QChar('\\')) == (path.size()-1)) )
                        path.append("/");
                    path.append("Airports"); // XML is only in here
                    if (!d.exists(path))
                        continue;
                    QDirIterator loopFiles( path, QDirIterator::Subdirectories );
                    while (loopFiles.hasNext()) {

                        //= Get file handle if there is one
                        xFileName = loopFiles.next();

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
                            cols << airport_code << airport_name << fileInfoThreshold.absoluteDir().absolutePath() << QString::number(pathnumber);

							out << cols.join("\t").append("\n");
							air_added++;


                            found++;
                        }

                        if(progress.wasCanceled()){
                            progress.hide();
                            return true;
                        }
                        c++;
                    }
                }
            }
        }
    }


	cacheFile.close();

    msg.sprintf("*** FGx airportsdata reload: Walked %d files, found %d threshold.xml, appended %d to cache",
                c, found, air_added);
    outLog(msg+", in "+getElapTimeStg(tm.elapsed()));
	progress.hide();
	return false;
}
