#ifndef AIRCRAFTMODEL_H
#define AIRCRAFTMODEL_H


#include <QStringList>
#include <QAbstractItemModel>
#include <QFileInfoList>


#include "aircraft/modelinfo.h"
#include "xobjects/mainobject.h"
class MainObject;

class AircraftModel : public QAbstractItemModel
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

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    int rowCount();
    int columnCount(const QModelIndex &idx) const;
    QModelIndex parent(const QModelIndex &child) const;

    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    MainObject *mainObject;
    QStringList headerLabels;

    QString cacheFileName;

    QList<ModelInfo> modelInfoList;

    //QList<QStandardItem*> create_model_row();
signals:

public slots:
    void load(bool reload_cache);

    bool scan_dir(QString dir);



    static QFileInfoList get_xml_set_files(QString dir, bool recus);
    static ModelInfo read_model_xml(QString dir);

    bool cache_exists();
    bool write_cache();
    bool read_cache();
};

#endif // AIRCRAFTMODEL_H
