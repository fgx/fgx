// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xaero.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef XAERO_H
#define XAERO_H

#include <QString>

class XAero
{
public:
	XAero(QString callsign);

	QString callsign;

	QString altitude;
	QString heading;
    QString speed;

    QString lat;
    QString lon;

};

#endif // XAERO_H
