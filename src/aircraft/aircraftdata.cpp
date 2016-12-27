// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // aircraftdata.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#include <QFile>
#include <QTextStream>
#include <QProgressDialog>
#include <QStringList>
#include <QDir>
#include <QXmlQuery>
#include <QDomDocument>
#include <QDomNode>

#include "aircraft/aircraftdata.h"

#include "utilities/utilities.h"
#include "utilities/messagebox.h"

//AircraftData::AircraftData()
//{
//}

bool AircraftData::import(QProgressDialog *progress, MainObject *mainObject){

    int c = 0;
    int found = 0;

    progress->setRange(0, 2000);
    progress->setWindowTitle("Scanning Aircraft Directories");
    progress->repaint();

    // Removing cache file, if exists()
    if (QFile::exists(mainObject->data_file("aircrafts.txt"))) {
        outLog("*** FGx aircrafts/hangar data reload: cache file exists!");
        QFile::remove(mainObject->data_file("airports.txt"));
        outLog("*** FGx aircrafts/hangar data reload: REMOVED AIRPORTS CACHE FILE");
    }

    //= Cache File
    QFile cacheFile( mainObject->data_file("aircraft.txt") );
    if(!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "TODO Open error cachce file=ssssssssssssssssssssssssssssssssssssssss";
        return true;
    }



    QTextStream out(&cacheFile);

    QString aircraft_base_path =  mainObject->X->aircraft_path();
    //= Get files Entries from Aircaft/ directory
    QDir aircraftDir(aircraft_base_path);
    qDebug() << "aircraft_path()=" << aircraft_base_path;

    aircraftDir.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    QStringList entries = aircraftDir.entryList();
    progress->setRange(0, entries.size() + 20);

    for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry ){

        // Filter out default dir names, should be a QDir name filter?
        if (*entry != "Instruments" &&  *entry != "Instruments-3d" && *entry != "Generic") {
            //qDebug() << "ENTRY+" << *entry;
            progress->setValue(c);
            progress->setLabelText(*entry);
            progress->repaint();

            //** get the List of *-set.xml files in dir
            QDir dir( mainObject->X->aircraft_path(*entry) );
            QStringList filters;
            filters << "*-set.xml";
            QStringList list_xml = dir.entryList(filters);

            if(list_xml.count() > 0){ // << Scan MOdels
                QString xml_file;


                //** Add Path Node
                QString directory = QString(*entry);
                //** Add Models
                for (int i = 0; i < list_xml.size(); ++i){

                    xml_file = QString(list_xml.at(i));

                    QString file_path =  mainObject->X->aircraft_path(*entry);
                    file_path.append("/");
                    file_path.append(xml_file);

                    //ModelInfo mi = AircraftData::read_model_xml(file_path);
                    //mi.filter_path = aircraft_base_path;
                    //QStringList lines;
                    //lines  << mi.dir << mi.aero << mi.description << mi.fdm << mi.authors << mi.xml_file << mi.file_path << mi.filter_path;
                    //out << lines.join("\t") << "\n";

                    found++;

                    if(progress->wasCanceled()){
                        return true;
                    }
                    c++;
                }

            } /* list_xml.count() > 0 */
        } /* entry != INstruments etc */
    } /* loop entries.() */

    cacheFile.close();
    return false;
}

