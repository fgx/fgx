#include "flightpositions.h"

FlightPositions::FlightPositions()
{
}

void FlightPositions::update_position(QString lat, QString lon, QString alt_ft, QString hdg, QString spd_kt)
{
    XPosition p;
    p.coord =  Marble::GeoDataCoordinates(lon.toFloat(), lat.toFloat(),
                                    alt_ft.toInt(),
                                    Marble::GeoDataCoordinates::Degree);
    p.alt_ft = alt_ft;
    p.alt_color = FlightsModel::get_altitude_color(alt_ft);
    p.spd_kt = spd_kt;
    p.hdg  = hdg;

    this->positions.insert(0, p);
    //qDebug() << this->cooordinates.length();
}
