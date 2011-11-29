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
