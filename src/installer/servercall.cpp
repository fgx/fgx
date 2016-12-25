

#include <QtDebug>


#include <QUrl>
#include <QUrlQuery>

//TODO #include <QJSON>


#include "installer/servercall.h"

ServerCall::ServerCall( QObject *parent) :
    QObject(parent)
{
    //M = mob;
    netMan = new QNetworkAccessManager(this);

}

void ServerCall::get_record(QString url, int id)
{
    QString u = QString("/%1/%2").arg(url).arg(QString::number(id));
    get(u);

}

void ServerCall::get(QString url)
{
    QHash<QString, QString> vars;
    this->get(url, vars);
}

void ServerCall::get(QString url, QHash<QString, QString> vars)
{
    QUrl urlx(url);
    QUrlQuery query;
    //urlx.setScheme("http");
    //urlx.setHost("127.0.0.1");
    //urlx.setPort(5000);
    //urlx.setPath( QString("/rpc%1").arg(url) );
    QHashIterator<QString, QString> i(vars);
     while (i.hasNext()) {
         i.next();
         query.addQueryItem(i.key(), i.value());
     }
    urlx.setQuery(query);
    qDebug() << urlx.toString();

    serverString = "";
    QNetworkRequest request;
    request.setUrl(urlx);
    //return;
    reply = netMan->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(on_server_ready_read()) );
    connect(reply, SIGNAL(finished()), this, SLOT(on_server_read_finished()) );
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(on_server_error(QNetworkReply::NetworkError)));
}




void ServerCall::post(QString url, QHash<QString, QString> payload)
{

    //= Encode Payload
    QUrl urlPayloadEncoder = QUrl();
    QHashIterator<QString, QString> ip(payload);
    QUrlQuery q;
     while (ip.hasNext()) {
         ip.next();
         q.addQueryItem(ip.key(), ip.value());
     }
     urlPayloadEncoder.setQuery(q);

     //urlParamsEncoder->addQueryItem( k, str(self.params[k]) )
    //self.postData =  urlParamsEncoder.encodedQuery()

    QUrl urlx;
    urlx.setScheme("http");
    urlx.setHost("192.168.5.12");
    urlx.setPort(4444);
    urlx.setPath(url);

    qDebug() << urlx.toString();

    serverString = "";
    QNetworkRequest request;
    request.setUrl(urlx);
    // TODO said pedro
    //reply = netMan->post(request);
    //connect(reply, SIGNAL(readyRead()), this, SLOT(on_server_ready_read()) );
    //connect(reply, SIGNAL(finished()), this, SLOT(on_server_read_finished()) );
    //connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(on_server_error(QNetworkReply::NetworkError)));
}










void ServerCall::on_server_error(QNetworkReply::NetworkError code)
{
    qDebug() << "Error" << code << reply->errorString();
}

void ServerCall::on_server_ready_read()
{
    serverString.append(reply->readAll());
    //qDebug() << "Read"; // << serverString;
}

void ServerCall::on_server_read_finished()
{
     qDebug() << ">> got Data from server"; // << serverString;
     // TODO
      //QScriptValue json;
      //QScriptEngine engine;
      //QScriptValue json = engine.evaluate( "(" + serverString + ")");
      //emit data(json);
}










/*

void ServerCall::attach_statusbar(XStatusBar *sBar)
{
    this->statusBar = sBar;
}
*/
