
#include <QtDebug>

#include <QScriptEngine>
#include <QScriptValue>


#include "flightsmodel.h"

FlightsModel::FlightsModel(QObject *parent) :
    QStandardItemModel(parent)
{

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
    //
    //QTimer::singleShot( 2 * 1000, this, SLOT(fetch_server()) );
     //qDebug() << "init";
    timer->setInterval(1000);
    this->connect(this->timer, SIGNAL(timeout()),
                  this, SLOT(fetch_server()) );
    //timer->start();

}



void FlightsModel::fetch_server()
{

    server_string = "";

    //WTF DNS is slow..
    //QUrl url("http://http://crossfeed.fgx.ch/flights.json");
    QUrl url("http://5.35.249.200/flights.json"); //http://crossfeed.fgx.ch/flights.json");

    QNetworkRequest request;
    request.setUrl( url );
    request.setRawHeader("Host", "crossfeed.fgx.ch");

    //reply = netMan->get(request);
    QNetworkReply *reply = netMan->get(request);
    qDebug() << "fetch  >" << QDateTime::currentDateTimeUtc().toString();
    /*
    connect(reply, SIGNAL( error(QNetworkReply::NetworkError)),
            this, SLOT(on_server_error(QNetworkReply::NetworkError))
    );
    connect(reply, SIGNAL( readyRead()),
            this, SLOT(on_server_ready_read())
    );
    connect(reply, SIGNAL( finished()),
            this, SLOT(on_server_read_finished())
    );
    */
    //statusBar->showMessage("Request");
    //QHostInfo::lookupHost("crossfeed.fgx.ch", this, SLOT(on_dns(const QHostInfo&)) );
    //this->timer->stop();
    //qDebug() << "fetchssss";

}


//==========================================================
//= Server Events
//==========================================================
void FlightsModel::on_server_error(QNetworkReply::NetworkError error){
    qDebug() << "error" << error;
    //Q_UNUSED(error);
    //outLog("FGx: PilotsWidget::on_server_error()");
}

void FlightsModel::on_server_ready_read(){
   // QString s(netMan->reply->readAll());
    //this->server_string.append(s);
    qDebug() << "read";
}

//=============================================
// Server call finished.. so parse to tree
//=============================================
/* we do not want to clear the tree and reload as user would loose focus..
   Also the last "communicate tiem means a pilot does not clear of the list"
   A net conection might drop and reappear later = crash or terrain terrain etc..
   SO current plan is to make a time stamp when a new pilot is gr
*/

void FlightsModel::on_server_read_finished(){

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    //qDebug() << "done "; //<< reply->readAll();
    //qDebug() << "reply  >" << QDateTime::currentDateTimeUtc().toString();
}


void FlightsModel::on_server_finished(QNetworkReply *reply){
    //qDebug() << "done_server" ;
    qDebug() << "reply  <" << QDateTime::currentDateTimeUtc().toString();
    reply->deleteLater();



}


void FlightsModel::on_dns(const QHostInfo &host)
{
    qDebug() << "DNS" << host.addresses();
}

