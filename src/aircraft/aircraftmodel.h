#ifndef AIRCRAFTMODEL_H
#define AIRCRAFTMODEL_H


#include <QStringList>
#include <QStandardItemModel>
#include <QFileInfoList>


#include "aircraft/modelinfo.h"
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


    QList<ModelInfo> modelInfoList;

    QList<QStandardItem*> create_model_row();
signals:

public slots:
    void load_aircraft(bool reload_cache);

    bool scan_dir(QString dir);



    static QFileInfoList get_xml_set_files(QString dir, bool recus);
    static ModelInfo read_model_xml(QString dir);

    bool write_cache();
    bool read_cache();
};

#endif // AIRCRAFTMODEL_H
