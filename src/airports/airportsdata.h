// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // airportsdata.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef AIRPORTSDATA_H
#define AIRPORTSDATA_H


#include <QWidget>
#include <QProgressDialog>

#include "xobjects/mainobject.h"

class AirportsData
{
public:
	//AirportsData();
	static bool import(QProgressDialog &progress, MainObject *mainObject);
};

#endif // AIRPORTSDATA_H
