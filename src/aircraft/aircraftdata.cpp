
//#include <QtDebug>

#include <QFile>
#include <QTextStream>
#include <QProgressDialog>
#include <QStringList>
#include <QDir>
#include <QXmlQuery>
#include <QDomDocument>
#include <QDomNode>

#include "aircraft/aircraftdata.h"

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

	//= Cache File
	QFile cacheFile( mainObject->data_file("aircraft.txt") );
	if(!cacheFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		//qDebug() << "TODO Open error cachce file=";
		return true;
	}



	QTextStream out(&cacheFile);

	//= Get files Entries from Aircaft/ directory
	QDir aircraftDir( mainObject->X->aircraft_path() );
	aircraftDir.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

	QStringList entries = aircraftDir.entryList();
	progress.setRange(0, entries.size() + 20);

	for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry ){

		// Filter out default dir names, should be a QDir name filter?
		if (*entry != "Instruments" &&  *entry != "Instruments-3d" && *entry != "Generic") {

			progress.setValue(c);
			progress.setLabelText(*entry);
			progress.repaint();

			//** get the List of *-set.xml files in dir
			QDir dir( mainObject->X->aircraft_path(*entry) );
			QStringList filters;
			filters << "*-set.xml";
			QStringList list_xml = dir.entryList(filters);

			if(list_xml.count() > 0){ // << Scan MOdels
				QString directory;
				QString description;
				QString author;
				QString fdm;
				QString xml_file;
				QString aero;

				//** Add Path Node
				directory = QString(*entry);
				//** Add Models
				for (int i = 0; i < list_xml.size(); ++i){

					xml_file = QString(list_xml.at(i));
					aero = QString(xml_file);
					aero.chop(8);

					//*=parse the Xml file - f&*& long winded
					QString file_path =  mainObject->X->aircraft_path(*entry);
					file_path.append("/");
					file_path.append(list_xml.at(i));
					QFile xmlFile( file_path);
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
							description = n.firstChildElement("description").text();
							author = n.firstChildElement("author").text().trimmed().replace(("\n"),"");
							fdm = n.firstChildElement("flight-model").text();
						} /* !query.isValid() */
					} /*  xmlFile.open() */

					QStringList lines;
					lines  << directory << aero << xml_file << description << fdm << author << file_path;
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
