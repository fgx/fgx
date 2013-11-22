#ifndef CROSSFEEDMODEL_H
#define CROSSFEEDMODEL_H

#include <QStandardItemModel>


class CrossfeedModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CrossfeedModel(QObject *parent);


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


signals:

public slots:

};

#endif // CROSSFEEDMODEL_H
