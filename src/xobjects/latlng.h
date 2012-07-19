// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // latlng.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef LATLNG_H
#define LATLNG_H

class LatLng
{
public:
    LatLng();
    LatLng(float lat, float lng);
    float lat();
    float lng();
    void setLat(float lat);
    void setLng(float lng);


private:
    float _lat;
    float _lng;
};

#endif // LATLNG_H
