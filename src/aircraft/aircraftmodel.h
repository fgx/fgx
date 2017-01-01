#ifndef AIRCRAFTMODEL_H
#define AIRCRAFTMODEL_H


#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
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
        C_AUTHORS,
        C_XML_FILE,
        C_FILE_PATH,
        C_FILTER_DIR,
        C_FILTER
    };

    explicit AircraftModel(MainObject *mOb);
    /*
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    int rowCount() const;
    int columnCount(const QModelIndex &idx) const;
    int columnCount() const;
    QModelIndex parent(const QModelIndex &child) const;

    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    */

    MainObject *mainObject;

    QString cacheFileName();

    QList<ModelInfo> modelInfoList;


    QList<QStandardItem*> create_append_row();

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
