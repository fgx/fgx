
#include <QtDebug>

#include "navdatamodel.h"

NavDataModel::NavDataModel(QObject *parent) :
    QStandardItemModel(parent)
{
    this->netMan = new QNetworkAccessManager(this);
    this->connect(this->netMan, SIGNAL( finished(QNetworkReply *)),
           this, SLOT(on_server_finished(QNetworkReply *))
    ); //<< FAILS.. does not connect ??! NO Was DNS

    setColumnCount(5);

    QStandardItem *item;

    item = new QStandardItem("Type");
    this->setHorizontalHeaderItem(C_NTYPE, item);

    item = new QStandardItem("Ident");
    this->setHorizontalHeaderItem(C_IDENT, item);

    item = new QStandardItem("Name");
    this->setHorizontalHeaderItem(C_NAME, item);

    item = new QStandardItem("Lat");
    this->setHorizontalHeaderItem(C_LAT, item);
    item = new QStandardItem("Lon");
    this->setHorizontalHeaderItem(C_LON, item);

}


void NavDataModel::fetch(QString search)
{
    QUrl url("http://127.0.0.1:8080/fix");
    url.addQueryItem("search", search);
    QNetworkRequest request;
    request.setUrl( url );
    //request.setRawHeader("Host", "crossfeed.fgx.ch");

    this->removeRows(0, this->rowCount());
    this->netMan->get(request);
      qDebug() << request.url().toString();
}

void NavDataModel::on_server_finished(QNetworkReply *reply)
{

    qDebug() << "got reply" ; //reply->readAll();
    if ( reply->error() != QNetworkReply::NoError){

        // @todo: handle error
       // qDebug() << " net error > " << reply->errorString() << " - " << QDateTime::currentDateTimeUtc().toString();
        reply->deleteLater();
        return;
    }

    QScriptEngine engine;
    QScriptValue data = engine.evaluate("(" + reply->readAll() + ")");

    QScriptValue nRows = data.property("rows");
    qDebug() << "YES" <<  nRows.isArray();
    if ( nRows.isArray() ){

        qDebug() << "YES";
        QScriptValueIterator it(nRows);
        while (it.hasNext()) {

            it.next();
                 qDebug() << "YES" << it.value().property("ident").toString();
                QStandardItem *iType = new QStandardItem( it.value().property("ntype").toString() );
                QStandardItem *iIdent = new QStandardItem( it.value().property("ident").toString() );
                QStandardItem *iName = new QStandardItem( it.value().property("name").toString() );

                QStandardItem *iLat = new QStandardItem( it.value().property("lat").toString() );
                QStandardItem *iLon = new QStandardItem( it.value().property("lon").toString() );


                QList<QStandardItem *> insertItemsList;
                insertItemsList << iType <<  iIdent << iName << iLat << iLon;
                for(int i =0; i < insertItemsList.count(); i++){
                    insertItemsList.at(i)->setEditable(false);
                }
                this->appendRow( insertItemsList );



        } // while
        emit load_finished();
    }


    reply->deleteLater();
}
