// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2016
//  Pete Morgan

#ifndef NETMANAGER_H
#define NETMANAGER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScriptValue>

#include "xreply.h"

class NetManager : public QObject
{
    Q_OBJECT
public:
    explicit NetManager();

     enum R_TYPE {
         JSON = 0,
         HTML,
         TEXT
     };

    QNetworkAccessManager *netMan;

signals:
    void reply(XReply);

public slots:

    void on_finished(QNetworkReply *qreply);

    void get(QObject *caller, QString url);
    void get(QObject *caller, QString url, R_TYPE);
};

#endif // NETMANAGER_H

