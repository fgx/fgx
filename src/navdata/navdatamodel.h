#ifndef NAVDATAMODEL_H
#define NAVDATAMODEL_H

#include <QStandardItemModel>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>


class NavDataModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit NavDataModel(QObject *parent = 0);

    enum COLS{

        C_NTYPE = 0,
        C_IDENT,
        C_NAME,

        C_LAT,
        C_LON

    };

    QNetworkAccessManager *netMan;

signals:
    void load_finished();

public slots:

    void fetch(QString type);
    void on_server_finished(QNetworkReply *);

};

#endif // NAVDATAMODEL_H
