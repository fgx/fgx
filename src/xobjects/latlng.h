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
