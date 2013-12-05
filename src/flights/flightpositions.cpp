#include "flightpositions.h"

FlightPositions::FlightPositions()
{
}

void FlightPositions::update_position(QString lat, QString lon, QString alt_ft, QString hdg, QString spd_ft)
{
    Marble::GeoDataCoordinates blip(lat.toFloat(), lon.toFloat(), alt_ft.toInt(), Marble::GeoDataCoordinates::Degree);

    this->blips.insert(0, blip);
    //qDebug() << this->cooordinates.length();
}
