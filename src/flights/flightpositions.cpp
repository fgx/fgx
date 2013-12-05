#include "flightpositions.h"

FlightPositions::FlightPositions()
{
}

void FlightPositions::update_position(QString lat, QString lon, QString alt_ft, QString hdg, QString spd_ft)
{
    Marble::GeoDataCoordinates blip(lat.toFloat(),lon.toFloat(), alt_ft.toInt(), Marble::GeoDataCoordinates::Degree);

    this->cooordinates.insert(0, blip);
}
