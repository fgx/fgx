
#include <QtDebug>

#include <QScriptEngine>
#include <QScriptValue>
#include <QFileInfo>

#include "flightsmodel.h"

FlightsModel::FlightsModel(QObject *parent) :
    QStandardItemModel(parent)
{


    //===============================================================
    // Initialize le model and the columns
    setColumnCount(7);


    QStandardItem *item;

    item = new QStandardItem("Flight ID");
    setHorizontalHeaderItem(C_FLIGHT_ID, item);

    item = new QStandardItem("Callsign");
    setHorizontalHeaderItem(C_CALLSIGN, item);

    item = new QStandardItem("Model");
    setHorizontalHeaderItem(C_AERO, item);

    item = new QStandardItem("Spd");
    setHorizontalHeaderItem(C_SPEED, item);

    item = new QStandardItem("Hdg");
    setHorizontalHeaderItem(C_HEADING, item);

    item = new QStandardItem("Alt");
    setHorizontalHeaderItem(C_ALTITUDE, item);

    item = new QStandardItem("Lat");
    setHorizontalHeaderItem(C_LAT, item);
    item = new QStandardItem("Lon");
    setHorizontalHeaderItem(C_LON, item);


    item = new QStandardItem("Flag");
    setHorizontalHeaderItem(C_FLAG, item);

    item = new QStandardItem("Count");
    setHorizontalHeaderItem(C_COUNT, item);

    setSortRole(SORT_ROLE);

    //=============================================================
    // Request timer
    //
    // Not sure this timer works atmo its a fixed fire every x seconds,, eg one..
    // does not accomodate so far, DNS fail, and last reponse time, lag etc..
    // so this is up for grabs of how to deal with things.....
    this->timer = new QTimer(this);
    this->timer->setInterval(2000);
    this->timer->setSingleShot(false);
    this->connect(this->timer, SIGNAL(timeout()),
                  this, SLOT(fetch_server())
    );
    this->lag = 0;


    //===============================================================
    // http request
    //
    // The netmanager is our connection to the ajax crossfeed.
    this->netMan = new QNetworkAccessManager(this);
    this->connect(this->netMan, SIGNAL( finished(QNetworkReply *)),
           this, SLOT(on_server_finished(QNetworkReply *))
    ); //<< FAILS.. does not connect ??! NO Was DNS

    // Wwe have a problem with DNS requests..
    // and the timer start and send 1 request a second .. and dns replies and
    // suddenly bombard with
}



void FlightsModel::fetch_server()
{

    //server_string = "";

    //WTF DNS is slow..
    //QUrl url("http://http://crossfeed.fgx.ch/flights.json");
    // @todo: make this a setting
    // HACK TIME
    // This is the IP address.. Yes I know..
    // pete's machine was takign around 30+ seconds to do dns ?/ why wtf.. ?/ still a mystery
    // so the hack is hardcode ip address. and the http-header with "Host: crossfeed.fgx.ch"
    // so nginx knows how to proxy ;-)
    // todo maybe this request should be initialised in class ?? is it gonna change ?/
    QUrl url("http://5.35.249.200/flights.json");

    QNetworkRequest request;
    request.setUrl( url );
    request.setRawHeader("Host", "crossfeed.fgx.ch");

    // tag with current epoch to the millisends this request..
    // this "timestamp" is added to the request as our special USER attribute
    QString milli_timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    request.setAttribute(QNetworkRequest::User, milli_timestamp );

    // fetch.. said teh sheperd to the collie..
    netMan->get(request);

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
   // if(reply->readAll().length() < 10){
        // qDebug() << " error NO DATA";
     //   return; // hack to get rid of dead crud as experiment
    //}
    //bool ok;

    // Current time
    qint64 timm = QDateTime::currentMSecsSinceEpoch();

    // request send time
    qint64 ms = reply->request().attribute(QNetworkRequest::User).toLongLong();

    // calculate lag
    this->lag = timm - ms;

    // make into eopch seconds for expiry into model
    timm = timm / 1000;

    //=========================================
    // Parse the JSON from crossfeed, using script engine ..  example line below
    /*
     *"flights":[
     *    {"fid":1385156785000,"callsign":"dozor","lat":59.245209,"lon":29.946378,"alt_ft":323,"model":"Aircraft/D-100/Models/D-100.xml","spd_kts":0,"hdg":278,"dist_nm":0}
     * ]
     */

    // Should this be a class singleton ??
    QScriptEngine engine;

    // @toto if(engine.canEvaluate()
    QScriptValue data = engine.evaluate("(" + reply->readAll() + ")");

    // The json array we after is the "flights" property..
    QScriptValue nFlights = data.property("flights");

    // check its there and maybe this crash if not...
    if ( nFlights.isArray() ){

        QScriptValueIterator it(nFlights);
        while (it.hasNext()) {

            it.next();

            QString callsign = it.value().property("callsign").toString();
            FlightPositions *p;

            //= hack.. Avoids suprious json stuff that is happening - see pete
            if (callsign.length() > 2){

                // rip out some frequent vars
                QString alt_ft = it.value().property("alt_ft").toString();
                QString spd_kt = it.value().property("spd_kts").toString();
                QString hdg = it.value().property("hdg").toString();
                QString lat = it.value().property("lat").toString();
                QString lon = it.value().property("lon").toString();


                // Find Items matching this callsign
                // @TODO: Geoff?? should this be callsign or fid we search for ? (or the option).. for now callsign
                QList<QStandardItem *> fitems = this->findItems(callsign, Qt::MatchExactly, C_CALLSIGN);

                // This is the row we want to update or is inserted to model
                int row = -1;

                if (fitems.count() > 0){

                    //= Item found, so the row we want to update below
                    row = fitems.at(0)->index().row();
                    p = this->flightPositions[callsign];

                }else{


                    // Not found so add new row
                    QString aero_model = QFileInfo (it.value().property("model").toString() ).baseName(); // get model name
                    QList<QStandardItem *> insertItemsList;

                    QStandardItem *iCallsignn = new QStandardItem( callsign );
                    iCallsignn->setData(callsign.toUpper(), SORT_ROLE);

                    insertItemsList << new QStandardItem( it.value().property("fid").toString() )
                                    << iCallsignn

                                    << new QStandardItem( it.value().property("alt_ft").toString() )
                                    << new QStandardItem( it.value().property("hdg").toString() )
                                    << new QStandardItem( it.value().property("spd_kts").toString() )

                                    << new QStandardItem( aero_model )

                                    << new QStandardItem( it.value().property("lat").toString() )
                                    << new QStandardItem( it.value().property("lon").toString() )

                                     << new QStandardItem( QString::number(timm) );

                    // loop items to set uneditable, and align some cols to right
                    for(int i =0; i < insertItemsList.count(); i++){
                        QStandardItem *itl = insertItemsList.at(i);
                        itl->setEditable(false);
                        if(i == C_HEADING || i == C_ALTITUDE || i == C_SPEED){
                            itl->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
                        }else{
                            itl->setTextAlignment(Qt::AlignVCenter);
                        }
                    }

                    // append row item and then get the row index
                    this->appendRow( insertItemsList );
                    row = this->indexFromItem(iCallsignn).row();

                    // Create the positions
                    p = new FlightPositions();
                    this->flightPositions[callsign] = p;

                }

                // Update all the stuff, including the stuff newly added..
                // The model is sorted by SORT_ROLE, hence numbers are 0 padded
                this->item(row, C_ALTITUDE)->setText(  it.value().property("alt_ft").toString());

                this->item(row, C_ALTITUDE)->setData(  it.value().property("alt_ft").toString().rightJustified(6, QChar('0')), SORT_ROLE);

                this->item(row, C_HEADING)->setText(it.value().property("hdg").toString());
                this->item(row, C_HEADING)->setData(  it.value().property("hdg").toString().rightJustified(6, QChar('0')), SORT_ROLE);

                this->item(row, C_SPEED)->setText(it.value().property("spd_kts").toString());
                this->item(row, C_SPEED)->setData(  it.value().property("spd_kts").toString().rightJustified(6, QChar('0')), SORT_ROLE);

                this->item(row, C_LAT)->setText(it.value().property("lat").toString());
                this->item(row, C_LON)->setText(it.value().property("lon").toString());

                this->item(row, C_FLAG)->setText( QString::number(timm) );
                this->item(row, C_SPEED)->setData(  QString::number(timm) , SORT_ROLE);

                 p->update_position(lat, lon, alt_ft, hdg, spd_kt);
            } //valid callsign

        } // while
        emit update_done();
    }

    reply->deleteLater();
}


void FlightsModel::on_dns(const QHostInfo &host)
{
    qDebug() << "DNS" << host.addresses();
}


// QList <FlightPositions> FlightsModel::get_trails(QString callsign)
//{
//     return this->flightPositions[callsign]->cooordinates;
//}
