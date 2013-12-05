#ifndef FLIGHTSMODEL_H
#define FLIGHTSMODEL_H

#include <QStandardItemModel>

#include <QTimer>
#include <QDateTime>

// @todo:  qt5 this could be QJson..
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>


#include <QHostInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


#include "marble/GeoDataCoordinates.h"

using namespace Marble;

// Class to store a list of last positions..
// this could be self contained .. but for now we push at front to the max at end
class XPositions
{
public:
    QList<GeoDataCoordinates> coordinates;

};


class FlightsModel : public QStandardItemModel
{
Q_OBJECT

public:
    explicit FlightsModel(QObject *parent=0);


    enum COLS{
        // The yet to test atomic geoff.cf.Id..
        C_FLIGHT_ID = 0,
        C_CALLSIGN,       
        C_ALTITUDE,
        C_HEADING,
        C_SPEED,
        C_AERO,
        C_LAT,
        C_LON,
        C_FLAG,
        C_COUNT
    };

    enum ROLES{
        SORT_ROLE = Qt::UserRole + 1
    };

    int lag;

    QTimer *timer;

    QNetworkAccessManager *netMan;
    //QNetworkReply  *reply;
    //QString server_string;
    // QHash<QString, QList<Marble::GeoDataCoordinates>> positions;
    QHash<QString, XPositions> trailPositions;


signals:
    void update_done();

public slots:

 void fetch_server();
 //void on_server_error(QNetworkReply::NetworkError);
// void on_server_ready_read();
 //void on_server_read_finished();

 void on_server_finished(QNetworkReply *reply);

 void on_dns(const QHostInfo &host);
};

#endif // FLIGHTSMODEL_H
