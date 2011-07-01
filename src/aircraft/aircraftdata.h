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
