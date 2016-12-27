#ifndef AIRCRAFTMODEL_H
#define AIRCRAFTMODEL_H


#include <QStringList>
#include <QStandardItemModel>


#include "xobjects/mainobject.h"
class MainObject;

class AircraftModel : public QStandardItemModel
{
    Q_OBJECT
public:

    enum COLS{
        C_DIR,
        C_AERO,
        C_DESCRIPTION,
        C_FDM,
        C_AUTHOR,
        C_XML_FILE,
        C_FILE_PATH,
        C_FILTER_PATH,
        C_FILTER,
        C_BASE
    };

    explicit AircraftModel(MainObject *mOb);

    MainObject *mainObject;



    QList<QStandardItem*> create_model_row();
signals:

public slots:
    void load_aircraft(bool reload_cache);
    void load_custom_aircraft();
};

#endif // AIRCRAFTMODEL_H
