
#include <QDebug>

#include <QtCore/QDir>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QtGui/QProgressDialog>

#include <QtXmlPatterns/QXmlQuery>
#include <QtXml/QDomDocument>
#include <QtScript/QScriptEngine>

#include "aerotools.h"
#include "utilities/utilities.h"

#undef AEROTOOLS_DEBUG1 // to output a BIG list of aircraft read

AeroTools::AeroTools(QObject *parent, MainObject *mOb) :
    QObject(parent)
{
	mainObject = mOb;
}




//========================================================
//*** Walk XML - sets
//========================================================

/* This function walks the /Aircraft/ directory
 its find files in a directory maching "-set.xml"
   and therby engquires the xml fo
*/

void AeroTools::scan_xml_sets(){

	int c = 0;
	int found = 0;
	QProgressDialog progress(tr("Scanning Aircraft to Database"), tr("Cancel"), 0, 0);
	progress.setWindowModality(Qt::WindowModal);
	progress.show();


	//* Clear any records in the database
	QSqlQuery sql("delete from aircraft;", mainObject->db);
	sql.exec();
        rows.clear(); // remove all current rows
        QStringList list;
	//* Insert Aircraft query			
	QSqlQuery sqlAero(mainObject->db);
	sqlAero.prepare("INSERT INTO aircraft(aero, directory, xml_file, description, fdm, author)VALUES(?,?,?,?,?,?)");

	//* Get Entries from Aircaft/ directory
	QDir aircraftDir( mainObject->settings->aircraft_path() );
	aircraftDir.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

	QStringList entries = aircraftDir.entryList();
	progress.setMaximum(entries.size());

	for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry ){

		// Filter out default dir names, should be a QDir name filter?
		if (*entry != "Instruments" &&  *entry != "Instruments-3d" && *entry != "Generic") {

			progress.setValue(progress.value() + 1);

			//** get the List of *-set.xml files in dir
			QDir dir( mainObject->settings->aircraft_path(*entry) );
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

					//** parse the Xml file - f&*& long winded
					QString file_path =  mainObject->settings->aircraft_path(*entry);
					file_path.append("/");
					file_path.append(list_xml.at(i));
					QFile xmlFile( file_path);
					if (xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){

						/* The file content id converted to UTF-8.
							 Some files are Windows, encoding and throw error with QxmlQuery etc
							 Its a hack and dont quite understand whats happening.. said pedro
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
							author = n.firstChildElement("author").text();
							fdm = n.firstChildElement("flight-model").text();
						} /* !query.isValid() */
					} /*  xmlFile.open() */

					//## TODO Here we go with stuff to cache - Later DB
					//#1 This version saves to settings and an embedded dam dirty delimiter string stuff..

					//aero,path, description, fdm, author
					sqlAero.bindValue(0, aero);
					sqlAero.bindValue(1, directory);
					sqlAero.bindValue(2, xml_file);
					sqlAero.bindValue(3, description);
					sqlAero.bindValue(4, fdm);
					sqlAero.bindValue(5, author);
					if(!sqlAero.exec()){
						qDebug() << mainObject->db.lastError();
					}

					found++;
                                        list.clear();
                                        list += aero;
                                        list += directory;
                                        list += xml_file;
                                        list += description;
                                        list += fdm;
                                        list += author;
                                        rows += list;
#ifdef AEROTOOLS_DEBUG1                 // if on, very noisy debug output
                                        outLog("aero=["+aero+"] dir=["+directory+"] file=["+xml_file+"] desc=["+description+"] fdm=["+fdm+"] author=["+author+"]");
#endif
					if(progress.wasCanceled()){
						break; // TODO ?? why..
					}
					c++;
				} // <Add models

			} /* list_xml.count() > 0 */
		} /* entry != INstruments etc */
	} /* loop entries.() */	

}


