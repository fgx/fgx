
#include <QtDebug>



#include "flightsmodel.h"

FlightsModel::FlightsModel(QObject *parent) :
    QStandardItemModel(parent)
{

    timmy = new QTimer(this);
    netMan = new QNetworkAccessManager(this);


    setColumnCount(7);

    //QStringList headers;
    //headers  << "Flight ID" << "Callsign" << "Alt" << "Heading" << "Speed" << "Aircraft" << "Foo";
    //setHorizontalHeaderLabels(headers);

    QStandardItem *item;

    item = new QStandardItem("Flight ID");
    this->setHorizontalHeaderItem(C_FLIGHT_ID, item);

    item = new QStandardItem("Callsign");
    this->setHorizontalHeaderItem(C_FLIGHT_ID, item);

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
    timmy->setInterval(1000);
    timmy->start();
}



void FlightsModel::fetch_server()
{
    qDebug() << "fetch";

    server_string = "";
    QUrl url("http://crossfeed.fgx.ch/flights.json");
    QNetworkRequest request;
    request.setUrl(url );

    reply = netMan->get(request);
    connect(reply, SIGNAL( error(QNetworkReply::NetworkError)),
            this, SLOT(on_server_error(QNetworkReply::NetworkError))
    );
    connect(reply, SIGNAL( readyRead()),
            this, SLOT(on_server_ready_read())
    );
    connect(reply, SIGNAL( finished()),
            this, SLOT(on_server_read_finished())
    );
    //statusBar->showMessage("Request");

}


//==========================================================
//= Server Events
//==========================================================
void FlightsModel::on_server_error(QNetworkReply::NetworkError error){
    qDebug() << "error" << error;
    Q_UNUSED(error);
    //outLog("FGx: PilotsWidget::on_server_error()");
}

void FlightsModel::on_server_ready_read(){
    QString s(reply->readAll());
    this->server_string.append(s);
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
    qDebug() << "done" << server_string;

}
