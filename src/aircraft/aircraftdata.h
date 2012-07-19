// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // aircraftdata.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef AIRCRAFTDATA_H
#define AIRCRAFTDATA_H

#include <QWidget>
#include <QProgressDialog>

#include "xobjects/mainobject.h"


class AircraftData
{
public:
	//AircraftData();

	static bool import(QProgressDialog &progress, MainObject *mainObject);

};

#endif // AIRCRAFTDATA_H
