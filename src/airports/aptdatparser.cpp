
/*
  Peel of the robin and gets out the airports and runways and minimal hopefully

  TODO incomplete list

  step 1

  only load the ICAO aiports, ignore heliports and seway etc.. lite wiegt and wise..

  assumed is ils etc..
  Other aiports whousl be available on demand as a parse etc..
  eg integration with FG

  Make a way to send new data online for verification

  */

#include <QtGui/QProgressDialog>
#include <QtGui/QIcon>


#include <QtCore/QDebug>

#include <QtCore/QFile>
#include <QtCore/QIODevice>

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QByteArray>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


#include "aptdatparser.h"

//#include <zip.h>



/* From the guide >>

Each row of data has a numeric ‘row code’ as its first field, to define its content.
The top level of this hierarchy defines an individual airport, defined by an airport header row (a row code of ‘1’, ‘16’ or ‘17’).

1  = Land airport header
16 = Seaplane base header
17 = Heliport header
---
100 runway

--
 14 airport viewpoint
18 lightbeacon
19 windsock
---
50 - 56
Comms Freq

*/


AptDatParser::AptDatParser(QObject *parent) :
    QObject(parent)
{
    estimated_lines = 1510000;
    line_counter = -1;
}



void AptDatParser::import_aptdat(QString tarball_fullpath, QWidget *parentWidget){


    QFile file(tarball_fullpath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("OOPS: file problem");
        return;
     }

    bool success;
    QSqlQuery queryCreate;

    //** Create airports table
    if( !queryCreate.exec("DROP TABLE IF EXISTS airports") ){
		qDebug() << queryCreate.lastError();
        return;
    }
    if( !queryCreate.exec("CREATE TABLE airports(airport varchar(10) NOT NULL PRIMARY KEY, name varchar(50) NULL, elevation int, tower tinyint NULL) ")){
        qDebug() << queryCreate.lastError();
        return;
    }

    //** create Runways table
    if( !queryCreate.exec("DROP TABLE IF EXISTS runways")){
        qDebug() << queryCreate.lastError();
        return;
    }
    if( !queryCreate.exec("CREATE TABLE runways(airport varchar(10) NULL, runway varchar(15), width numeric(2,2), length numeric(4,0), lat numeric(3,8), lng numeric(3,8), heading numeric(3,2) )")){
        qDebug() << queryCreate.lastError();
        return;
    }
    line_counter = 0;
    QRegExp rxICAOAirport("[A-Z]{4}");

    QSqlQuery queryAirportInsert;
    queryAirportInsert.prepare("insert into airports( airport, name, elevation, tower)values(?, ?, ?, ?)");

    //QSqlQuery queryRwySel;
    //queryRwySel.prepare("select * from runways where airport=? and runway=?");
    //QSqlQuery queryRunwayInsert;
    //queryRunwayInsert.prepare("insert into runways(  airport, runways, width, lat1, lng1, lat2, lng2)values(?, ?, ?, ?, ?, ?, ?)");


    bool is_icao;

	QProgressDialog progress("Importing Airports", "Cancel", 0, estimated_lines, parentWidget);
    progress.setWindowTitle("Importing Airports");
    progress.setWindowModality(Qt::WindowModal);
    progress.setFixedWidth(400);
    progress.setWindowIcon(QIcon(":/icons/import"));
    progress.setMinimumDuration(0);
	//progress.show();
	//progress.repaint();

    //* ignore first line
    file.readLine();

    //** second line contains the version string
    QString credits = file.readLine();
    int version = 999;
    if(credits.startsWith("810 Version")){
        version = 810;
    }
    if(version == 0){
        return;
    }
    qDebug() << "version=" << version;
    //qDebug() << "1" << file.readLine();
    //qDebug() << "2" << file.readLine();
   // qDebug() << "3" << file.readLine();
    //qDebug() << "4" << file.readLine();
    //return;
     QString airport_code;
     QString airport_name;
     int elevation;
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

            /** http://data.x-plane.com/file_specs/Apt715.htm
              0 = airport code
              1 = elevation
              2 = has tower
              3 = not approp
              4 = code
              5+ description
            **/
            airport_code = parts[4];
            elevation = parts[1].toInt();
            tower =  parts[2] == "1" ? "1" : "";
            airport_name.clear();
            for(int p = 5; p < parts.size(); p++){ //** TODO WTF ?
                airport_name.append(parts[p]).append(" ");
            }
            is_icao = rxICAOAirport.exactMatch(airport_code);

            if(is_icao){
                //qDebug() << "##" << airport_code << "=" << airport_name << " @ " << elevation << tower;
				queryAirportInsert.addBindValue( airport_code );
                queryAirportInsert.addBindValue( airport_name.trimmed() );
				queryAirportInsert.addBindValue( elevation );
                queryAirportInsert.addBindValue( parts[2] == "1" ? "1" : NULL );
                success = queryAirportInsert.exec();
                if(!success){
                    qDebug() << queryAirportInsert.lastError();
                    qDebug() << "DIE queryApt";
                    return;
                }
            } /* if(is_icao) */
        } /* if(row_code == "1") airport */

        //**********************************************************************8
        //*** Runway                                                                      mark tdz
        //      width surface/shou/sm/   No   lat           lng          disp_len  blast_len light  f f f
        //100   49.99   1   0 0.25 1 2 0 09L  51.47747035 -000.48962591  306.02    0.00 5  4 1 1 27R  51.47767520 -000.43326100    0.00   21.03 5  4 1 1
        //100   49.99   1   0 0.25 1 2 0 09R  51.46477398 -000.48694615  306.93    0.00 5  4 1 1 27L  51.46495200 -000.43407800    0.00   21.03 5  4 1 1

        if(row_code == "10" ){
            QString lat;
            QString lng;
            QString runway;
           // QString runways;
            QString heading;
            QString length;
            QString width;


            if(is_icao){
                if(version > 0){
                    /*
                      0 = 10 = runway code
                      1= lat
                      2=lon
                      3 = rwyno
                      4 = true heading
                      5 = length
                      6 = len of displaced
                      */
                      lat = parts[1];
                      lng = parts[2];
                      runway = parts[3];
                      heading = parts[4];
                      length = parts[5];

                      if(runway == "xxx"){
                        //qDebug() << "uh?" << runway;
                      }else{
                          if(runway.endsWith("x")){
                              runway = runway.left(runway.length() - 1);
                          }
                        //qDebug() << line;
						//qDebug() << "Runway= " << airport_code << runway << lat << lng << "\n\n";
                        QSqlQuery queryRunwayInsert;
                        queryRunwayInsert.prepare("insert into runways(  airport, runway, width, length, lat, lng, heading)values(?, ?, ?, ?, ?, ?, ?)");
                       // qDebug() << airport << " - " << runways << " - " <<  parts[1] << " - " << parts[9] << " - " << parts[10] << " - " << parts[18] << " - " << parts[19];
                         //qDebug() << "runways" << runways;
                        queryRunwayInsert.addBindValue( airport_code);
                        queryRunwayInsert.addBindValue( runway );
                        queryRunwayInsert.addBindValue( width );
                        queryRunwayInsert.addBindValue( length );
                        queryRunwayInsert.addBindValue( lat );
                        queryRunwayInsert.addBindValue( lng );
                        queryRunwayInsert.addBindValue( heading );
                        success = queryRunwayInsert.exec();
                        if(!success){
                            qDebug() << queryRunwayInsert.lastError();
						   // qDebug() << "DIE queryRwyIns";
                            return;
                        }else{
						   //qDebug() << "runway ok";
                        }
                      }
                }else{
                    //QString runways = parts[8];
                    //runways.append("-").append(parts[17]);
                    //qDebug() << runways;
                } /* version */



//                QString runwayOp = parts[17];
//                queryRunwayInsert.addBindValue( airport);
//                queryRunwayInsert.addBindValue( runwayOp );
//                success = queryRunwayInsert.exec();
//                if(!success){
//                    qDebug() << queryRunwayInsert.lastError();
//                    qDebug() << "DIE queryRwyIns";
//                    return;
//                }
            } /* is_icao */
        } /* if(row_code == "100") Runway */


        if (progress.wasCanceled()){
            return;
        }
        line_counter++;
		if(line_counter % 100 == 0){
            qDebug() <<  line_counter;
            progress.setValue(line_counter);
			QString prog_text = QString("%1 of approx %2").arg(line_counter).arg(estimated_lines);
            progress.setLabelText(prog_text);
            progress.repaint();
        }
		/*
        if(line_counter == 20000){
			qDebug() << "had enough";
            return;
        }
		*/

        } /* end while readline */

    queryCreate.exec("CREATE INDEX idx_airport_name on airports(name)");
    queryCreate.exec("CREATE INDEX idx_airport_icao on runways(airport)");
}
