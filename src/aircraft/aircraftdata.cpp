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

bool AircraftData::import(QProgressDialog &progress, MainObject *mainObject){

    int c = 0;
    int found = 0;

    progress.setRange(0, 2000);
    progress.setWindowTitle("Scanning Aircraft Directories");
    progress.show();
    progress.repaint();

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
    progress.setRange(0, entries.size() + 20);

    for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry ){

        // Filter out default dir names, should be a QDir name filter?
        if (*entry != "Instruments" &&  *entry != "Instruments-3d" && *entry != "Generic") {
            qDebug() << "ENTRY+" << *entry;
            progress.setValue(c);
            progress.setLabelText(*entry);
            progress.repaint();

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

                    ModelInfo mi = AircraftData::read_model_xml(file_path);
                    mi.filter_path = aircraft_base_path;
                    QStringList lines;
                    lines  << mi.dir << mi.aero << mi.description << mi.fdm << mi.authors << mi.xml_file << mi.file_path << mi.filter_path;
                    out << lines.join("\t") << "\n";

                    found++;

                    if(progress.wasCanceled()){
                        //qDebug() << "Progress cancelled!";
                        progress.hide();
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

/* \brief Returns all -set.xml files in a directory. Recus is for custom folder and selecting a parent */
QFileInfoList AircraftData::get_xml_set_files(QString dir_path, bool recus){
    QDir dir( dir_path );
    QStringList filters;
    filters << "*-set.xml";
    QFileInfoList setList =  dir.entryInfoList(filters);
    if(recus){
        QFileInfoList allentries = dir.entryInfoList();
        for(int i = 0; i < allentries.length(); i++){
            qDebug() << "--" << allentries.at(i).absoluteFilePath();
            if(allentries.at(i).isDir()){
                QFileInfoList subset = AircraftData::get_xml_set_files(allentries.at(i).absoluteFilePath(), false);
                for(int ii = 0; ii < subset.length(); ii++){
                    setList.append(subset.at(ii));
                }
            }
        }
    }
    return setList;
}

/* \brief Parses the xml-set file by querying DOM */
ModelInfo AircraftData::read_model_xml(QString xml_set_path){

    QFileInfo fInfo(xml_set_path);

    ModelInfo mi;
    mi.ok = false;
    mi.file_path = xml_set_path;
    //mi.dir_path = fInfo.dir().absolutePath();
    mi.dir = fInfo.dir().dirName();

    // the model = filename without --set.xml
    mi.aero = fInfo.fileName();
    mi.aero.chop(8);


    QFile xmlFile( xml_set_path );
    if (xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){

        /* The file content is converted to UTF-8.
             Some files are Windows, encoding and throw error with QxmlQuery etc
             Its a hack and don't quite understand whats happening.. said pedro
        */
        QString xmlString = QString(xmlFile.readAll()).toUtf8();

        QXmlQuery query;
        query.setFocus(xmlString);
        //query.setFocus(&xmlFile); << Because file is not QTF8 using sting instead
        query.setQuery("PropertyList/sim");
        if (query.isValid()){

            QString res;
            query.evaluateTo(&res);
            xmlFile.close();

            QDomDocument dom;
            dom.setContent("" + res + "");
            QDomNodeList nodes = dom.elementsByTagName("sim");

            QDomNode n = nodes.at(0);
            mi.description = n.firstChildElement("description").text();
            mi.authors = n.firstChildElement("author").text().trimmed().replace(("\n"),"");
            mi.fdm = n.firstChildElement("flight-model").text();
            mi.ok = true;

        } /* !query.isValid() */
    } /*  xmlFile.open() */

    return mi;
}
