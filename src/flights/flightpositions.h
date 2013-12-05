#ifndef FLIGHTPOSITIONS_H
#define FLIGHTPOSITIONS_H

#include <QQueue>

#include "marble/GeoDataCoordinates.h"


#include "flights/flightsmodel.h"
class FlightsModel;

using namespace Marble;


class XPosition {
public:
    GeoDataCoordinates coord;
    QColor alt_color;
    QString alt_ft;
    QString spd_kt;
    QString hdg;

};



// Class to store a list of last positions..
// this could be self contained .. but for now we push at front to the max at end

class FlightPositions
{
public:
    FlightPositions();


    //QString callsign;

    QList<XPosition> positions;

    //lat, lon, alt_ft, hdg, spd_kt
    void update_position(QString lat, QString lon, QString alt_ft, QString hdg, QString spd_kt);

};

#endif // FLIGHTPOSITIONS_H
