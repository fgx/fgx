#ifndef MAPVIEWSMODEL_H
#define MAPVIEWSMODEL_H

#include <QStandardItemModel>

class MapViewsModel : public QStandardItemModel
{
    Q_OBJECT
public:

    static const QString SETTINGS_TAG;

    enum COLS{
        C_LABEL = 0,
        C_LAT,
        C_LON,
        C_ZOOM,
        C_DELETE
    };

    explicit MapViewsModel(QObject *parent = 0);


signals:

public slots:

    void add_view(QString label, QString lat, QString lon, int zoom);
    //void update_view(QString label, QString lat, QString lon, int zoom);
    void remove_view(QString label);

    void save();
    void load();
};

#endif // MAPVIEWSMODEL_H
