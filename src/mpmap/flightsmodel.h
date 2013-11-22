#ifndef FLIGHTSMODEL_H
#define FLIGHTSMODEL_H

#include <QStandardItemModel>
#include <QTimer>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FlightsModel : public QStandardItemModel
{
Q_OBJECT

public:
    explicit FlightsModel(QObject *parent);


    enum COLS{
        C_FLIGHT_ID = 0,
        C_CALLSIGN,
        C_AIRCRAFT,
        C_ALTITUDE,
        C_HEADING,
        C_PITCH,
        C_LAT,
        C_LON,
        C_FLAG,
        C_COUNT
    };

    QTimer *timer;

    QNetworkAccessManager *netMan;
    //QNetworkReply  *reply;
    QString server_string;






signals:
    //

public slots:

 void fetch_server();
 void on_server_error(QNetworkReply::NetworkError);
 void on_server_ready_read();
 void on_server_read_finished();

 void on_server_finished(QNetworkReply *reply);


};

#endif // FLIGHTSMODEL_H
