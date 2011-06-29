#ifndef AIRPORTSDATA_H
#define AIRPORTSDATA_H


#include <QWidget>

#include "xobjects/mainobject.h"

class AirportsData
{
public:
	//AirportsData();

	static void import(QWidget *parent, MainObject *mainObject, bool inc_icao);
};

#endif // AIRPORTSDATA_H
