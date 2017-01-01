#ifndef AIRCRAFTMODEL_H
#define AIRCRAFTMODEL_H


#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileInfoList>

#include "xobjects/mainobject.h"
class MainObject;

//#include "aircraft/modelinfo.h"

// container for the xml data
class ModelInfo
{
public:
    bool ok; // is true is xml parsed and data read
    QString dir; // short parentdir
    QString fdm;
    QString authors;
    QString description;
    QString full_path;
    QString filter_dir;

};


// Storage for aircraft
class AircraftModel : public QStandardItemModel
{
    Q_OBJECT
public:

    enum COLS{
        C_DIR,
        C_AERO,
        C_DESCRIPTION,
        C_FDM,
        C_AUTHORS,
        C_XML_FILE,
        C_FILE_PATH,
        C_FILTER_DIR,
        C_FILTER
    };

    explicit AircraftModel(MainObject *mOb);

    MainObject *mainObject;

    QString cacheFileName();
    QList<QStandardItem*> create_append_row();

signals:

public slots:

    void load(bool reload_cache);
    bool cache_exists();
    bool read_cache();

    static QFileInfoList get_xml_set_files(QString dir, bool recus);
    static ModelInfo read_model_xml(QString dir);


};

#endif // AIRCRAFTMODEL_H
