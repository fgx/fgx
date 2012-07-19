// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // latlng.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#include "latlng.h"

LatLng::LatLng()
{
}

LatLng::LatLng(float lat, float lng)
{
    _lat = lat;
    _lng = lng;
}

float LatLng::lat(){
    return _lat;
}

float LatLng::lng(){
    return _lng;
}

void LatLng::setLat(float lat){
    _lat = lat;
}

void LatLng::setLng(float lng){
	_lng = lng;
}

