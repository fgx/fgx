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
class MainObject;

#include "aircraft/modelinfo.h"
class ModelInfo;

class AircraftData
{
public:
    //AircraftData();

    static bool import(QProgressDialog &progress, MainObject *mainObject);

    static QFileInfoList get_xml_set_files(QString dir, bool recus);
    static ModelInfo read_model_xml(QString dir);

};

#endif // AIRCRAFTDATA_H
