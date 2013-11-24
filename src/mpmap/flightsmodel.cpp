
#include <QtDebug>

#include <QScriptEngine>
#include <QScriptValue>


#include "flightsmodel.h"

FlightsModel::FlightsModel(QObject *parent) :
    QStandardItemModel(parent)
{

    lag = 0;

    timer = new QTimer(this);
    this->netMan = new QNetworkAccessManager(this);
    this->connect(this->netMan, SIGNAL( finished(QNetworkReply *)),
           this, SLOT(on_server_finished(QNetworkReply *))
    ); //<< FAILS.. does not connect ??! NO Was DNS

    setColumnCount(7);

    //QStringList headers;
    //headers  << "Flight ID" << "Callsign" << "Alt" << "Heading" << "Speed" << "Aircraft" << "Foo";
    //setHorizontalHeaderLabels(headers);

    QStandardItem *item;

    item = new QStandardItem("Flight ID");
    this->setHorizontalHeaderItem(C_FLIGHT_ID, item);

    item = new QStandardItem("Callsign");
    this->setHorizontalHeaderItem(C_CALLSIGN, item);



    item = new QStandardItem("Speed");
    this->setHorizontalHeaderItem(C_SPEED, item);

    item = new QStandardItem("Heading");
    this->setHorizontalHeaderItem(C_HEADING, item);

    item = new QStandardItem("Altitude");
    this->setHorizontalHeaderItem(C_ALTITUDE, item);



    item = new QStandardItem("Lat");
    this->setHorizontalHeaderItem(C_LAT, item);
    item = new QStandardItem("Lon");
    this->setHorizontalHeaderItem(C_LON, item);



    /*
    tree->header()->setStretchLastSection(true);
    tree->header()->setResizeMode(QHeaderView::Stretch);
    tree->headerItem()->setText(C_CALLSIGN, "Callsign");
    tree->headerItem()->setText(C_AIRCRAFT, "Aircraft");
    tree->headerItem()->setText(C_ALTITUDE, "Alt");
    tree->headerItem()->setText(C_HEADING, "Hdg");
    tree->headerItem()->setText(C_PITCH, "Pitch");
    tree->headerItem()->setText(C_LAT, "Lat");
    tree->headerItem()->setText(C_LON, "Lon");
    tree->headerItem()->setText(C_FLAG, "Flag");
    tree->headerItem()->setText(C_COUNT, "Count");

    tree->headerItem()->setTextAlignment(C_ALTITUDE, Qt::AlignRight);
    tree->headerItem()->setTextAlignment(C_HEADING, Qt::AlignRight);
    tree->headerItem()->setTextAlignment(C_LAT, Qt::AlignRight);
    tree->headerItem()->setTextAlignment(C_LON, Qt::AlignRight);
    */



    timer->setInterval(1000);
    timer->setSingleShot(false);
    this->connect(this->timer, SIGNAL(timeout()),
                  this, SLOT(fetch_server()) );

}



void FlightsModel::fetch_server()
{

    //server_string = "";

    //WTF DNS is slow..
    //QUrl url("http://http://crossfeed.fgx.ch/flights.json");
    // @todo: make this a setting
    QUrl url("http://5.35.249.200/flights.json");

    QNetworkRequest request;
    request.setUrl( url );
    request.setRawHeader("Host", "crossfeed.fgx.ch");

    // tag with current epoch ms
    QString no = QString::number(QDateTime::currentMSecsSinceEpoch());
    request.setAttribute(QNetworkRequest::User, no );

    //reply = netMan->get(request);
    netMan->get(request);
    //qDebug() << "fetch  >" << QDateTime::currentDateTimeUtc() << no << QDateTime::currentMSecsSinceEpoch();


}








//=============================================
// Server call finished.. so parse to tree
//=============================================
/* we do not want to clear the tree and reload as user would loose focus..
   Also the last "communicate tiem means a pilot does not clear of the list"
   A net conection might drop and reappear later = crash or terrain terrain etc..
   SO current plan is to make a time stamp when a new pilot is gr
*/
void FlightsModel::on_server_finished(QNetworkReply *reply){
    //qDebug() << "done_server" ;
    //qDebug() << "reply  <" << QDateTime::currentDateTimeUtc().toString();
    //qDebug() << reply->errorString();


    if ( reply->error() != QNetworkReply::NoError){

        // @todo: handle error
        qDebug() << " net error > " << reply->errorString() << " - " << QDateTime::currentDateTimeUtc().toString();
        reply->deleteLater();
        return;
    }
    //bool ok;
    qint64 ms = reply->request().attribute(QNetworkRequest::User).toLongLong();
    //qDebug() << ok << ms << QDateTime::currentMSecsSinceEpoch();
    //if (ok){
    this->lag = QDateTime::currentMSecsSinceEpoch() - ms;
    //    qDebug() << "round trip" << lag;
    //}else{
     //   lag = 0;
    //}

    // Parse the JSON from crossfeed
    /*
     *"flights":[
     *    {"fid":1385156785000,"callsign":"dozor","lat":59.245209,"lon":29.946378,"alt_ft":323,"model":"Aircraft/D-100/Models/D-100.xml","spd_kts":0,"hdg":278,"dist_nm":0}
     * ]
     */

    QScriptEngine engine;
    QScriptValue data = engine.evaluate("(" + reply->readAll() + ")");

    QScriptValue nFlights = data.property("flights");

    if ( nFlights.isArray() ){

        //qDebug() << "YES";
        /*
         * C_FLIGHT_ID = 0,
        C_CALLSIGN,
        C_ALTITUDE,
        C_HEADING,
        C_SPEED,
        C_AIRCRAFT,
        C_LAT,
        C_LON,
        C_FLAG,
        C_COUNT */
        QScriptValueIterator it(nFlights);
        while (it.hasNext()) {

            it.next();
            //qDebug() << it.name() << ": " << it.value().toString();
            //qDebug() << it.value().property("callsign").toString();
            QString callsign = it.value().property("callsign").toString();

            // @TODO: Geoff?? should this be callsign or fid we search for ? (or the option).. for now callsign
            QList<QStandardItem *> fitems = this->findItems(callsign, Qt::MatchExactly, C_CALLSIGN);
            if (fitems.count() == 0){

                    QStandardItem *iFid = new QStandardItem( it.value().property("fid").toString() );
                    QStandardItem *iCallsign = new QStandardItem( it.value().property("callsign").toString() );
                    QStandardItem *iModel = new QStandardItem( it.value().property("model").toString() );

                    QStandardItem *iAltitude = new QStandardItem( it.value().property("alt_ft").toString() );
                    iAltitude->setTextAlignment(Qt::AlignRight);

                    QStandardItem *iHeading = new QStandardItem( it.value().property("hdg").toString() );
                    iHeading->setTextAlignment(Qt::AlignRight);

                    QStandardItem *iSpeed = new QStandardItem( it.value().property("spd_kts").toString() );
                    iSpeed->setTextAlignment(Qt::AlignRight);

                    QStandardItem *iLat = new QStandardItem( it.value().property("lat").toString() );
                    QStandardItem *iLon = new QStandardItem( it.value().property("lon").toString() );

                // todo@ at and FGx platform level, we can insert or insert index..
                // this means fid here and index key almost.. is the problem pete has..
                QList<QStandardItem *> insertItemsList;
                insertItemsList << iFid <<  iCallsign << iAltitude << iHeading << iSpeed << iModel << iLat << iLon;
                for(int i =0; i < insertItemsList.count(); i++){
                    insertItemsList.at(i)->setEditable(false);
                }
                this->appendRow( insertItemsList );
            }else{
               // qDebug() << "update";
                int row = fitems.at(0)->index().row();
                this->item(row, C_ALTITUDE)->setText(it.value().property("alt_ft").toString());
                this->item(row, C_HEADING)->setText(it.value().property("hdg").toString());
                this->item(row, C_SPEED)->setText(it.value().property("spd_kts").toString());

                this->item(row, C_LAT)->setText(it.value().property("lat").toString());
                this->item(row, C_LON)->setText(it.value().property("lon").toString());
            }


        } // while
        emit update_done();
    }


    reply->deleteLater();
}


void FlightsModel::on_dns(const QHostInfo &host)
{
    qDebug() << "DNS" << host.addresses();
}

