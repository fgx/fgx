#ifndef FLIGHTPOSITIONS_H
#define FLIGHTPOSITIONS_H

#include <QQueue>

#include "marble/GeoDataCoordinates.h"

using namespace Marble;


// Class to store a list of last positions..
// this could be self contained .. but for now we push at front to the max at end
class FlightPositions
{
public:
    FlightPositions();


    //QString callsign;

    QList<GeoDataCoordinates> cooordinates;

    //lat, lon, alt_ft, hdg, spd_kt
    void update_position(QString lat, QString lon, QString alt_ft, QString hdg, QString spd_ft);

};

#endif // FLIGHTPOSITIONS_H
