// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher //
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2016
// Pete Morgan
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QtDebug>
#include <QScriptEngine>
#include <QScriptValueIterator>


#include "netman.h"

NetManager::NetManager()
{
    netMan = new QNetworkAccessManager();

    connect(netMan, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_finished(QNetworkReply*))
            );

}



void NetManager::get(QObject *caller, QString url){
    this->get(caller, url, JSON);
}
void NetManager::get(QObject *caller, QString url_str, R_TYPE type){

    QUrl url(url_str);

    QNetworkRequest req;
    req.setUrl(url);
    req.setOriginatingObject(caller);

    switch(type){
        case JSON:
            req.setAttribute(QNetworkRequest::User, "json");
            break;

        case HTML:
        default:
            req.setAttribute(QNetworkRequest::User, "text");
    }


    this->netMan->get(req);

}


void NetManager::on_finished(QNetworkReply *qreply){

    XReply xreply;
    xreply.error = true;

    xreply.raw = qreply->readAll();

    QScriptEngine engine;

    if( engine.canEvaluate("(" + xreply.raw + ")") ){
        xreply.data = engine.evaluate( "(" + xreply.raw + ")");
        xreply.error = false;
        /*
        QScriptValueIterator it(json);
        while (it.hasNext()) {
            it.next();
            qDebug() << it.name() << "=" << it.value().toString() ;
        }*/
    } else {
        xreply.errorMess = "Cannot decode JSON";
    }
    emit reply(xreply);
}

